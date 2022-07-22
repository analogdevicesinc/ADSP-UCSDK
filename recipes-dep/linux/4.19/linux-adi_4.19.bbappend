FILESEXTRAPATHS_prepend := "${THISDIR}/../${PN}:"
SRC_URI_append = "\ 
            file://dep-4_19.cfg \
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
"
SRC_URI_append_adsp-sc598-som-ezkit = "\
            file://0001-sc59x-hrtimer-support_5_4.patch \
"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"