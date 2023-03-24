SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=12f884d2ae1ff87c09e5b7ccc2c4ca7e"

inherit module

SRC_URI = " file://Makefile \
            file://COPYING \
          "

SRC_URI_append_adsp-sc598-som-ezkit = " file://extclkin-gpt-sc5xx.c "
SRC_URI_append_adsp-sc594-som-ezkit = " file://extclkin-gpt-sc594.c "
SRC_URI_append_adsp-sc589-ezkit = " file://extclkin-gpt-sc58x.c "
SRC_URI_append_adsp-sc584-ezkit = " file://extclkin-gpt-sc58x.c "
SRC_URI_append_adsp-sc573-ezkit = " file://extclkin-gpt-sc57x.c "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-extclkin-gpt"
