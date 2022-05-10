FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = "\ 
            file://dep-5_4.cfg \
            file://0001-sc57x-hrtimer-support.patch \
            file://0005-sc58x-hrtimer-support.patch \
            file://0001-Changing-core-timer-from-0-1-to-2-3.patch \
            file://0001-sc59x-hrtimer-support_5_4.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"