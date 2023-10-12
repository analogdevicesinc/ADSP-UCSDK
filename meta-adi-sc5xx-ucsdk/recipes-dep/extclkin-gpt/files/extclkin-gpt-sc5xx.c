/*
 * extclkin-gpt.c
 * External clock input driver - SC5xx GPT version
 *
 * This is a character device driver which returns a pair of times in nanoseconds
 * when read from - an external clock time and the linux monotonic raw time. 
 * This version uses an external clock input pin into a SC5xx General Purpose Timer
 * (GPT) to act as a counter and determine an accurate and tracking (to external 
 * clocking circuitry) time, with an arbitrary offset. The "host" (linux) timestamp
 * provides a reference to determine this offset and track differences. 
 * 
 * See the README.md file for more information. 
 * 
 * This software is Copyright (c) 2020, Analog Devices Ltd and/or its licensors
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/timekeeping.h>
#include <linux/mutex.h>
#include <linux/preempt.h>
#include <linux/uaccess.h>
#include <linux/soc/adi/cpu.h>


#define ROLLOVER_DETECTION_BY_IRPT_LATCH   (0)
#define PRINTK_DBG(...)  printk(__VA_ARGS__)  

//#define DEV_DBG(...)  dev_dbg(__VA_ARGS__)
#define DEV_DBG(...) 

/* Requires GPL compatible license for module */
#define DRIVER_LICENSE "GPL"

#define DRIVER_AUTHOR "Thirumaleshwara Prasanna <prasanna.thirumaleshwara@analog.com>"
#define DRIVER_DESC "External clock input driver using ADSP SC5xx general purpose timer (GPT)"
/* Used throughout, eg as device file name */
#define DEVICE_NAME "extclkin"

// For C preprocessor stringification
#define xstr(s) str(s)
#define str(s) #s

// Using 64-bit unsigned values for nano second counts, giving a max value of 46744018473709551615
// TODO: more preprocessor to get 20 digits from u64_MAX
#define NS_VALUE_MAX_DIGITS   20
#define MAX_LINE_LENGTH       (2 * NS_VALUE_MAX_DIGITS + 2)
#define SECONDS_PER_ROLLOVER  429
#define NS_PER_ROLLOVER       ((u64)SECONDS_PER_ROLLOVER * (u64)1000000000UL)
#define GPT_NO                 3
#define GPT_MASK               (1<<GPT_NO)
#define SUCCESS                0
#define READ_BUFFER_SIZE       (MAX_LINE_LENGTH + 1)    


struct sc57x_extclkin_gpt {
    /* This state must be protected against multiple users */
    u32 rolloverCount;
    int deviceMajor;
    dev_t devNo;
    struct class *devClass;
    struct device *thisDev;
    void __iomem *io_base;
    u32 currCnt;
};


static struct sc57x_extclkin_gpt extclkin_gpt;
static struct sc5xx_gptimer *sc5xx_gpt;
static char readBuf[READ_BUFFER_SIZE];

/* Prototypes */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
//static inline volatile u32 reg_read(u32 address);
//static inline void reg_write(u32 address, u32 value);
static inline u32 get_count(u8 *rollover);


/* Protect state against multiple readers */
DEFINE_MUTEX(accessTimer);


/* Default timer input frequency: 10 MHz */
static unsigned long frequency = 10000000UL;
module_param(frequency, ulong, 0644);
MODULE_PARM_DESC(frequency, "Frequency of clock input to timer in Hz as unsigned long");

struct file_operations fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release
};


/*
 * Setups and enables the GPT to:
 *   - use external clock
 *   - count to 429s
 *   - run in wait mode
 * Performs an initial software reset on the timer. 
 */
static inline void setup_gpt(void)
{
    u32 regVal = 0;

/* 
PB_03 set in sc598-som-ezkit.dts 
*/

    //Confirming the whether timer is running or not
    regVal = gptimer_is_running(sc5xx_gpt);	
    PRINTK_DBG("\n Timer Run read  %d ", regVal);	
    if(regVal)
    {
        PRINTK_DBG("\n Timer %d is running. Disable it.", GPT_NO);
        // Timer Run Clear
		gptimer_disable(sc5xx_gpt);
    }

    // Configure GPT CFG, external clock mode
    regVal = TIMER_MODE_EXT_CLK | TIMER_IRQ_PER;    
    set_gptimer_config(sc5xx_gpt, regVal);    
    PRINTK_DBG("\n write GPT CFG Reg value : 0x%x", regVal);      

    // Set Period
    regVal = SECONDS_PER_ROLLOVER * frequency;
    set_gptimer_period(sc5xx_gpt, regVal);
    set_gptimer_pwidth(sc5xx_gpt, 0);	
    PRINTK_DBG("\n write GPT PERIOD Reg value : 0x%x, WIDTH=0", regVal); 

#if ROLLOVER_DETECTION_BY_IRPT_LATCH
    //Data IMASK
    regVal = get_gptimer_imask();	_to_be_added_ to kernel	
    regVal |= (GPT_MASK);
    set_gptimer_imask(sc5xx_gpt);	_to_be_added_ to kernel
#endif

    // Timer Run SET
    gptimer_enable(sc5xx_gpt);
    PRINTK_DBG("\n write GPT Run to TIMER0_TMR[%d]", GPT_NO);

    //Confirming the whether timer is running or not
    regVal = gptimer_is_running(sc5xx_gpt);	
    if(regVal)
    {
        PRINTK_DBG("\n Timer %d is running", GPT_NO);
        PRINTK_DBG("\n Current Timer value :  %d ", get_gptimer_count(sc5xx_gpt));
        PRINTK_DBG("\n Counter set up success");
    }

}

/*
 * Stops the GPT
 */
static inline void stop_gpt(void)
{
    // Timer Clear
    gptimer_disable(sc5xx_gpt);
    //Clear CFG
    set_gptimer_config(sc5xx_gpt, 0);
}

/*
 * Get the timer count value. 
 * Sets rollover to be 1 if rollover status bit set. 
 * Else rollover is set to 0. 
 * Rollover status bit cleared if set. 
 */ 
static inline u32 get_count(u8 *rollover)
{
    u32 count;
    // Get count from timer
    count = get_gptimer_count(sc5xx_gpt);
    
#if ROLLOVER_DETECTION_BY_IRPT_LATCH  
    u32 status;
    // Check rollover
    status = get_gptimer_status(sc5xx_gpt);	_to_be_added_ to kernel	
    if (status & GPT_MASK) {
        *rollover = 1;
        // clear flag by writing 1 for rollover, and 0's for others as not clearing any other flags
	set_gptimer_status(sc5xx_gpt);	_to_be_added_ to kernel	
        // ensure it's clear
	status = get_gptimer_status(sc5xx_gpt);	_to_be_added_ to kernel	
        if (status & GPT_MASK) {
            dev_err(extclkin_gpt.thisDev, "can't clear rollover flag.\n");
        }
    }
#else
   if(count < extclkin_gpt.currCnt)
   {
        *rollover = 1;
   }
#endif 
    else {
        *rollover = 0;
    }
    extclkin_gpt.currCnt = count;
    return count;
}

/*
 * Initialise the clock input driver, GPT variant. 
 */
static int __init gpt_clkin_init(void)
{
    pr_debug("Driver init in %s\n", __FILE__);
    
    extclkin_gpt.deviceMajor = register_chrdev(0, DEVICE_NAME, &fops);

    if (extclkin_gpt.deviceMajor < 0) {
        pr_err("Registering char device %s failed with %d\n", DEVICE_NAME, extclkin_gpt.deviceMajor);
        return extclkin_gpt.deviceMajor;
    }

    extclkin_gpt.devNo = MKDEV(extclkin_gpt.deviceMajor, 0);

    extclkin_gpt.devClass = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(extclkin_gpt.devClass)) {
        pr_err("Can't create class %s err: %ld\n", DEVICE_NAME, PTR_ERR(extclkin_gpt.devClass));
        return -EINVAL;
    }

    if (IS_ERR(extclkin_gpt.thisDev = device_create(extclkin_gpt.devClass, NULL, extclkin_gpt.devNo, NULL, DEVICE_NAME))) {
        pr_err("Can't create device /dev/%s err: %ld\n", DEVICE_NAME, PTR_ERR(extclkin_gpt.thisDev));
        class_destroy(extclkin_gpt.devClass);
        return -EINVAL;
    }
    
    dev_dbg(extclkin_gpt.thisDev, "Driver %s got major number %d. Create a dev file with 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, extclkin_gpt.deviceMajor, DEVICE_NAME, extclkin_gpt.deviceMajor);

    sc5xx_gpt = gptimer_request(GPT_NO);

    if (sc5xx_gpt == NULL) {
        dev_err(extclkin_gpt.thisDev, "Can't ioremap memory for GPT\n");
        device_destroy(extclkin_gpt.devClass, extclkin_gpt.devNo);
        class_destroy(extclkin_gpt.devClass);
        return -ENOMEM;
    }

    // setup timer/counter
    extclkin_gpt.currCnt = 0;
    setup_gpt();
    
    dev_info(extclkin_gpt.thisDev, "driver initialised\n");

    // Returning non-zero indicates module can't be loaded
    return SUCCESS;
}

/*
 * Exit out of the driver / module. 
 * De-register, destroy, cleanup, stop, ... all the things
 */
static void __exit gpt_clkin_exit(void)
{
    dev_dbg(extclkin_gpt.thisDev, "exiting...\n");

    stop_gpt();
	
    device_destroy(extclkin_gpt.devClass, extclkin_gpt.devNo);
    class_destroy(extclkin_gpt.devClass);
    unregister_chrdev(extclkin_gpt.deviceMajor, DEVICE_NAME);
}

/*
 * Called when a process tries to open the device file.
 * eg cat /dev/DEVICE_NAME
 */
static int device_open(struct inode *inode, struct file *file)
{

    DEV_DBG(extclkin_gpt.thisDev, "Device %s opened. Max line length: %u + 1\n", DEVICE_NAME, MAX_LINE_LENGTH);

    /* Increment usage count to protect against module removal. */
    try_module_get(THIS_MODULE);
    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
    /* Decrement usage count so module can be removed. */
    module_put(THIS_MODULE);
    DEV_DBG(extclkin_gpt.thisDev, "Device %s released.\n", DEVICE_NAME);
    return SUCCESS;
}

/*
 * Called when a process, which has the device file open, attempts to read from it.
 */
static ssize_t device_read(struct file *filp,   /* ref: include/linux/fs.h   */
                           char *buffer,        /* buffer to fill with data  */
                           size_t length,       /* length of the buffer      */
                           loff_t *offset       /* offset into file for read */
                           )
{
    ssize_t bytesRead = 0;      // retval
    u8 rollover;            // rollover occurred?
    u32 count;              // GPT count
    u64 host_ns, audio_ns;  // times
    u64 nsPerCount;         // frequency multiplier
    unsigned long flags;    // flags for saving IRQ state


    DEV_DBG(extclkin_gpt.thisDev, "Device %s read length %lu pos %lld offset %lld\n", DEVICE_NAME, length, filp->f_pos, *offset);
    //dev_info(extclkin_gpt.thisDev, "Device %s read length %lu pos %lld offset %lld\n", DEVICE_NAME, length, filp->f_pos, *offset);

    /* quick return for invalid parameters */
    if (*offset != 0 || filp->f_pos != 0) {
        return 0;
    }

    DEV_DBG(extclkin_gpt.thisDev,"locking mutex ");
    /* Prevent access to timer and rollover status changing overflow count from multiple accessors */
    mutex_lock(&accessTimer);
    /* Want to ensure the clock access are as close to each other as possible */
    preempt_disable();
    /* Let's also disable interrupts, just to be safe */
    local_irq_save(flags);

    
    /* Capture host monotonic raw clock to reference timer value against */
    /* Do this first as it's quick */
    host_ns = ktime_get_raw_ns();
    /* Get the timer/counter value and rollover status */
    count = get_count(&rollover);
    
    /* and re-enable them */
    local_irq_restore(flags);
    /* Can be pre-empted again now */
    preempt_enable();
    

    /* handle rollover condition */
    if (rollover) {
        extclkin_gpt.rolloverCount++;
        DEV_DBG(extclkin_gpt.thisDev, "Device %s: rollover detected, count %u.\n", DEVICE_NAME, extclkin_gpt.rolloverCount);
    }
    /* Finished with exclusivity */
    mutex_unlock(&accessTimer);
    DEV_DBG(extclkin_gpt.thisDev,"Mutex unlocked");

    /* Derived from frequency as 1s / 1ns / frequency = 10^9 / 10^7 = 100 for freq = 10 MHz */
    nsPerCount = 1000000000UL / frequency;

    /* Calculate the audio time using overflow, count and above value */
    audio_ns = (u64)((u64)extclkin_gpt.rolloverCount * (u64)NS_PER_ROLLOVER ) + (u64)((u64) count * nsPerCount);

    DEV_DBG(extclkin_gpt.thisDev,"Host Time : %llu, Audio time %llu ",host_ns, audio_ns);
    bytesRead = snprintf(readBuf, READ_BUFFER_SIZE, "%" xstr(NS_VALUE_MAX_DIGITS) "llu\t%" xstr(NS_VALUE_MAX_DIGITS) "llu\n", host_ns, audio_ns);
    if(buffer != NULL)
    {
        // if user buffer length isn't enough, log this and return no bytes
        if (length < READ_BUFFER_SIZE) 
        {
           dev_warn(extclkin_gpt.thisDev, "Device %s: read request had insufficient buffer size of %ld. Minimum required is %ld.\n", DEVICE_NAME, length,(long) READ_BUFFER_SIZE);
            return -EINVAL;
        }
    
        // write back to user buffer, check for error
        //PRINTK_DBG("Host Time : %llu, Audio time %llu, roll over count %u, rollvalue : %llu , Bare count :%u ",host_ns, audio_ns, extclkin_gpt.rolloverCount, NS_PER_ROLLOVER, count);
        if (copy_to_user(buffer, readBuf, bytesRead)) 
        {
           dev_err(extclkin_gpt.thisDev, "Device %s: couldn't write to device read buffer.\n", DEVICE_NAME);
           return -EFAULT;
        }
    }
    else
    {
        dev_err(extclkin_gpt.thisDev, "Device %s: invalid (null) user buffer.\n", DEVICE_NAME);
        return -EINVAL;
    }
    
    DEV_DBG(extclkin_gpt.thisDev, "Device %s: bytes read: %d\n ", DEVICE_NAME, bytesRead);
    return bytesRead;
}

module_init(gpt_clkin_init);
module_exit(gpt_clkin_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
