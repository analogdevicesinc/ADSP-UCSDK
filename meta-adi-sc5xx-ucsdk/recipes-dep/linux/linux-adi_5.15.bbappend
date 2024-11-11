FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = "\ 
            file://dep-5_15.cfg \
            file://dep_latency.cfg \
            file://0001-5_15-Diable-remoteproc-autoboot.patch \
            file://0002-312-Provide-gptimer-API-to-extclkinpgt-module.patch \
"

SRC_URI:append:adsp-sc598-som-ezkit = "\
            file://0002-5_15-SC598-extclkin-gpt-configure-gptimer-i2c-dts.patch \
            file://0003-312-SC598-Disable-i2s4-and-dai1-SRU.patch \
            file://0004-312-SC598-Disable-Sport-DMA.patch \
"

SRC_URI:append:adsp-sc594-som-ezkit = "\
            file://0001-5_15-SC594-configure-gptimer-and-i2c-for-extclkingpt.patch \
            file://0001-Set-TX-RX-queues-to-1.patch \
"

SRC_URI:append:adsp-sc573-ezkit = "\
            file://0005-5_15-SC573-configure-gptimer-on-TMR5-and-PC15-dtb.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"