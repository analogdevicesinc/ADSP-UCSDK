FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

#Disable RNDR, not available on our ARM64
SRC_URI:append:adsp-sc573-ezkit= " file://0001-Disable-RNDR-as-this-is-not-available-on-our-ARMv8.2.patch"
SRC_URI:append:adsp-sc584-ezkit= " file://0001-Disable-RNDR-as-this-is-not-available-on-our-ARMv8.2.patch"
SRC_URI:append:adsp-sc589-ezkit= " file://0001-Disable-RNDR-as-this-is-not-available-on-our-ARMv8.2.patch"
SRC_URI:append:adsp-sc589-mini= " file://0001-Disable-RNDR-as-this-is-not-available-on-our-ARMv8.2.patch"

#Disable jitter entropy generation/initialization (software based and takes too long)
EXTRA_OECONF:append:adsp-sc573-ezkit=" --disable-jitterentropy"
EXTRA_OECONF:append:adsp-sc584-ezkit=" --disable-jitterentropy"
EXTRA_OECONF:append:adsp-sc589-ezkit=" --disable-jitterentropy"
EXTRA_OECONF:append:adsp-sc589-mini=" --disable-jitterentropy"
EXTRA_OECONF:append:adsp-sc594-som-ezkit=" --disable-jitterentropy"