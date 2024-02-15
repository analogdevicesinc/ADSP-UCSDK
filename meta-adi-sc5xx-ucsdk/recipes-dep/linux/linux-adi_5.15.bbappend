FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = "\ 
            file://dep-5_15.cfg \
            file://0001-5_15-Diable-remoteproc-autoboot.patch \
            file://0001-5_15-Provide-gptimer-API-access-to-extclkingpt-modul.patch \
            file://0004-5_15-Add-headers-that-fix-build-errors-on-yocto-3.0.0.patch \
"

SRC_URI:append:adsp-sc598-som-ezkit = "\
            file://0002-5_15-SC598-extclkin-gpt-configure-gptimer-i2c-dts.patch \
"

SRC_URI:append:adsp-sc594-som-ezkit = "\
            file://0001-5_15-SC594-configure-gptimer-and-i2c-for-extclkingpt.patch \
            file://0001-Fix-DELAY_REQ-error-for-dwmacs-above-v3_70.patch \
"

SRC_URI:append:adsp-sc573-ezkit = "\
            file://0005-5_15-SC573-configure-gptimer-on-TMR5-and-PC15-dtb.patch \
            file://0001-Fix-DELAY_REQ-error-for-dwmacs-above-v3_70.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"