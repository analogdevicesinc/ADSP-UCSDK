/*
 * gptimer.h
 * Header to access gptimer devices on ADSP-SC5xx devices
 *
 * 
 * This software is Copyright (c) 2022, Analog Devices Ltd and/or its licensors
 * Harvin D Almeida <harvin.almeida@analog.com>
 */

#ifndef __SC5XX_GPTIMER
#define __SC5XX_GPTIMER

#include <linux/counter.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>

struct sc5xx_gptimer {
	int id;
	int irq;
	void __iomem *io_base;
};

struct gptimer_counter {
	struct counter_device counter;
};

struct clocksource_gptimer {
	struct clocksource cs;
	struct sc5xx_gptimer *timer;
};

struct clockevent_gptimer {
	struct clock_event_device evt;
	struct sc5xx_gptimer *timer;
};

struct sc5xx_gptimer_controller {
	void __iomem *base;
	struct clk *clk;
	struct clocksource_gptimer *cs;
	struct clockevent_gptimer *cevt;
	struct sc5xx_gptimer *timers;
	size_t num_timers;
	struct counter_device counter_dev;
};

/*
 * Shared gptimers registers
 */
#define GPTIMER_RUN								0x04
#define GPTIMER_RUN_SET							0x08
#define GPTIMER_RUN_CLR							0x0C
#define GPTIMER_STOP_CFG						0x10
#define GPTIMER_STOP_CFG_SET					0x14
#define GPTIMER_STOP_CFG_CLR					0x18
#define GPTIMER_DATA_IMSK						0x1C
#define GPTIMER_STAT_IMSK						0x20
#define GPTIMER_TRG_MSK							0x24
#define GPTIMER_TRG_IE							0x28
#define GPTIMER_DATA_ILAT						0x2C
#define GPTIMER_STAT_ILAT						0x30
#define GPTIMER_ERR_TYPE						0x34
#define GPTIMER_BCAST_PER						0x38
#define GPTIMER_BCAST_WID						0x3C
#define GPTIMER_BCAST_DLY						0x40

extern struct sc5xx_gptimer_controller gptimer_controller;

/*
static inline uint16_t get_gptimer_run(void)
{
	return readw(gptimer_controller.base + GPTIMER_RUN);
}
*/
static inline struct sc5xx_gptimer* gptimer_request(int id)
{
	return &gptimer_controller.timers[id];
}

extern void set_gptimer_period(struct sc5xx_gptimer *timer, uint32_t period);
extern void set_gptimer_config(struct sc5xx_gptimer *timer, uint16_t config);
extern uint32_t get_gptimer_count(struct sc5xx_gptimer *timer);
extern void gptimer_enable(struct sc5xx_gptimer *timer);
extern void gptimer_disable(struct sc5xx_gptimer *timer);
extern bool gptimer_is_running(struct sc5xx_gptimer *timer);

#endif