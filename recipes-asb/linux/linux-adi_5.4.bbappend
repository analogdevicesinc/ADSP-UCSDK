FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

# SRC_URI_append = "\
#            file://sram.cfg \
#            file://REMOTEPROC.cfg \
#            file://0001-Expand-sram1-to-end-of-L2-memory.patch \
#            file://0002-Added-IOCTL-to-read-last-used-physical-address.patch \
#            file://0003-Expand-L2-space-for-SC58x.patch \
#            file://0001-Enable-Sram-mmap-on-SC594_5_4.patch \
#            file://0001-Update-physical-address-IOCTL-for-Kernel-5.2.patch \
#            file://disable_soc_snd.cfg \
#"

SRC_URI_append = "\
            file://sram.cfg \
            file://REMOTEPROC.cfg \
            file://disable_soc_snd.cfg \
            file://0001-IOCTL-call-to-get-last-used-physical-address.patch \
"

SRC_URI_append_adsp-sc594-som-ezkit = "\
            file://0001-SC594-Enable-Sram-mmap.patch \
"
SRC_URI_append_adsp-sc589-ezkit = "\
            file://0001-SC58x-Expand-L2-space.patch \
"
SRC_URI_append_adsp-sc584-ezkit = "\
            file://0001-SC58x-Expand-L2-space.patch \
"
SRC_URI_append_adsp-sc573-ezkit = "\
            file://0001-SC57x-Expand-sram1-to-end-of-L2-memory.patch \
"
SRC_URI_append_adsp-sc598-som-ezkit = "\
            file://0001-SC598-family-move-L2-region-to-20104000.patch \
"

KERNEL_FEATURES_remove_adsp-sc573-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc589-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc584-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc594-som-ezkit = " snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc598-som-ezkit = " snd_ezkit.scc"

COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"