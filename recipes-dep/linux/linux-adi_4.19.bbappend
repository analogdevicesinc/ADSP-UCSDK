FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = "\ 
            file://dep.cfg \
            file://0001-sc57x-hrtimer-support.patch \
            file://0005-sc58x-hrtimer-support.patch \
            file://0001-Changing-core-timer-from-0-1-to-2-3.patch \
"