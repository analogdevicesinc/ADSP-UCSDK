# Clock module for SC573, SC589 & SC584
IMAGE_INSTALL_append_adsp-sc573-ezkit = " extclkin-gpt "
IMAGE_INSTALL_append_adsp-sc589-ezkit = " extclkin-gpt "
IMAGE_INSTALL_append_adsp-sc584-ezkit = " extclkin-gpt "
IMAGE_INSTALL_append_adsp-sc598-som-ezkit = " extclkin-gpt "

#SC598 eMMC as disk
IMAGE_INSTALL_append_adsp-sc598-som-ezkit = " \
    e2fsprogs-resize2fs \
    gzip \
    adi-flash-emmc \
    mtd-utils \
    mtd-utils-ubifs \
    e2fsprogs-mke2fs \
"

# Packages to add
IMAGE_INSTALL_append = " \ 
    nano \
    ethtool \
    gdbserver \ 
	sram-mmap-test \
"

#Packages to remove
IMAGE_INSTALL_remove = " \
	pound \
	dbus \
	bluez5 \
	packagegroup-tools-bluetooth \
	expat \
	play \
	mplayer-common \
	mpv \
	video-test \
	v4l2-video-test \
	ffmpeg \
    tslib \
	tslib-calibrate \
	evtest \
"