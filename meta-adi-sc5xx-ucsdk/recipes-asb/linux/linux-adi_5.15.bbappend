FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "\
            file://sram.cfg \
            file://REMOTEPROC.cfg \
            file://disable_soc_snd.cfg \
            file://0003-312-Set-SRAM-MMAPs-to-Non-cached.patch \
"

SRC_URI:append:adsp-sc598-som-ezkit = "\
            file://0001-312-IOCTL-to-get-last-used-physical-address.patch \
            file://0002-312-SC598-L2-sram-start-at-0x20044000.patch \
"
SRC_URI:append:adsp-sc594-som-ezkit = "\
            file://0001-312-ARM32-get-last-use-physical-address-IOCTL.patch \
            file://0002-312-SC594-L2-sram-start-at-0x20044000.patch \
"
KERNEL_FEATURES:remove:adsp-sc573-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc589-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc584-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc594-som-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc598-som-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"