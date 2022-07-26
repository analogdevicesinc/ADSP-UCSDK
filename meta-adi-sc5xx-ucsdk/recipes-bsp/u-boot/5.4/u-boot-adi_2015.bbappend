FILESEXTRAPATHS_prepend := "${THISDIR}/../${PN}:"

SRC_URI += "\
    file://0002-Set-usbboot-as-default-on-sc573-ezkit.patch \
    file://0001-Add-USB-boot-to-u-boot.patch \
"

