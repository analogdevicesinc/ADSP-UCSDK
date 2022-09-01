FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append_adsp-sc573-ezkit = "\
    file://0002-Set-usbboot-as-default-on-sc573-ezkit.patch \
    file://0001-Add-USB-boot-to-u-boot.patch \
"
COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"