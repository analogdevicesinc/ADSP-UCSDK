do_install_append () {
    cat >> ${D}${sysconfdir}/fstab <<EOF

# Mount CGROUP for DEP
cgroup /sys/fs/cgroup cgroup defaults 0 0

EOF
}