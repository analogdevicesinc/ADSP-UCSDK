FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = "\ 
            file://dep-5_4.cfg \            
"

SRC_URI:append:adsp-sc594-som-ezkit = "\
			file://0001-SC594-5_15-configure-gptimer-and-i2c-for-extclkingpt.patch \
            file://0001-5_15-Provide-gptimer-API-access-to-extclkingpt-modul.patch \
            file://0001-5_15-Diable-remoteproc-autoboot.patch \
            file://0001-Change-IRQMODE-to-Width-delay.patch \
            file://0001-Fix-DELAY_REQ-error-for-dwmacs-above-v3_70.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"