FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append = "\
            file://sram.cfg \
            file://REMOTEPROC.cfg \
            file://0001-Expand-sram1-to-end-of-L2-memory.patch \
            file://0002-Added-IOCTL-to-read-last-used-physical-address.patch \
            file://0003-Expand-L2-space-for-SC58x.patch \
            file://0001-Enable-Sram-mmap-on-SC594_5_4.patch \
            file://0001-Update-physical-address-IOCTL-for-Kernel-5.2.patch \
"
KERNEL_FEATURES_remove_adsp-sc573-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc589-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc584-ezkit = " feature/snd_ezkit.scc"
KERNEL_FEATURES_remove_adsp-sc594_som_ezkit = " feature/snd_ezkit.scc"
