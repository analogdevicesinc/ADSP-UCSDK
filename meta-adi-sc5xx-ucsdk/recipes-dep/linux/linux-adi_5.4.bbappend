FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = "\ 
            file://dep-5_4.cfg \            
"

SRC_URI_append_adsp-sc589-ezkit = "\
            file://0005-sc58x-hrtimer-support.patch \
            file://0001-Changing-core-timer-from-0-1-to-2-3.patch \
"
SRC_URI_append_adsp-sc584-ezkit = "\
            file://0005-sc58x-hrtimer-support.patch \
            file://0001-Changing-core-timer-from-0-1-to-2-3.patch \
"
SRC_URI_append_adsp-sc573-ezkit = "\
            file://0001-sc57x-hrtimer-support.patch \
"
SRC_URI_append_adsp-sc594-som-ezkit = "\
            file://0001-sc59x-hrtimer-support_5_4.patch \
			file://0001-Enable-I2C-1.patch \
			file://0001-remoteproc-print-removal.patch \
"
SRC_URI_append_adsp-sc598-som-ezkit = "\
            file://0001-Backport-external-clock-support.patch \
            file://0001-SC598-External-Clock-support.patch \
            file://0002-sc598-extclkin-i2c1.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"