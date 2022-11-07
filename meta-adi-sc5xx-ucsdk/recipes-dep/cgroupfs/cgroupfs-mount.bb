inherit update-rc.d

DESCRIPTION = "Light-weight package to set up cgroupfs mounts"
DEPENDS = "linux-adi"
LICENSE = "GPLv3+"
LIC_FILES_CHKSUM = "file://LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

CGROUPFS_GIT_URI ?= "git://github.com/tianon/cgroupfs-mount.git"
CGROUPFS_GIT_PROTOCOL = "https"

SRCREV = "190a219fcca93c352f280fa4f24a702f804d8eed"
SRC_URI = " \ 
${CGROUPFS_GIT_URI};protocol=${CGROUPFS_GIT_PROTOCOL} \
"

S = "${WORKDIR}/git"

INITSCRIPT_NAME = "cgroupfs.sh"
INITSCRIPT_PARAMS = "start 30 3 4 5 ."

SRC_URI += " \
	file://${INITSCRIPT_NAME} \
"

do_install() {
    install -d ${D}/usr/bin
    install -m 755 ${S}/cgroupfs-mount ${D}/usr/bin/
    install -m 755 ${S}/cgroupfs-umount ${D}/usr/bin/

    install -d ${D}/${INIT_D_DIR}
    install -m 0755 ${WORKDIR}/${INITSCRIPT_NAME} ${D}/${INIT_D_DIR}/${INITSCRIPT_NAME}
}