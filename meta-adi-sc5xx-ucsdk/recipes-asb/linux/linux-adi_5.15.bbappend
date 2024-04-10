FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "\
            file://sram.cfg \
            file://REMOTEPROC.cfg \
            file://disable_soc_snd.cfg \
            file://0001-5_15_IOCTL-call-to-get-last-used-physical-address.patch \           
"

SRC_URI:append:adsp-sc598-som-ezkit = "\
            file://0001-SC598-Move-sram-start-address-to-20044000.patch \
"
SRC_URI:append:adsp-sc573-ezkit = "\
            file://0001-SC573-L2-start-address-to-0x20044000-so-ASB-handshak.patch \
"

KERNEL_FEATURES:remove:adsp-sc573-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc589-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc584-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc594-som-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"
KERNEL_FEATURES:remove:adsp-sc598-som-ezkit = " ${WORKDIR}/feature/snd_ezkit.scc"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"