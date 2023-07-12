SUMMARY = "Flamegrah Yocto recipe"
DESCRIPTION = "Flamegraph are a visualization tool for profiled application logs"
LICENSE = "CLOSED"

S = "${WORKDIR}/git"

SRC_URI = "git://github.com/brendangregg/FlameGraph.git;protocol=https;branch=master"
SRCREV="d9fcc272b6a08c3e3e5b7919040f0ab5f8952d65"

RDEPENDS:flamegraph = " perl perl-module-getopt-long perl-module-open "

FILES:${PN} += "flamegraph/"
do_install() {
     install -d ${D}/flamegraph
     install -m 0755 ${S}/*.pl ${D}/flamegraph
}