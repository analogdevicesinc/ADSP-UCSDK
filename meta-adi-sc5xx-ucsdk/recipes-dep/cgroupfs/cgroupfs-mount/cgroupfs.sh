#!/bin/sh
case "$1" in
  start)
	echo "Mounting cgroupfs hierarchy: "
	/usr/bin/cgroupfs-mount
	;;
  stop)
    printf "Unmounting cgroupfs hierarchy: "
	/usr/bin/cgroupfs-umount
	;;
  restart|reload)
	stop
	start
	;;
  *)
	echo "Usage: $0 {start|stop|restart}"
	exit 1
esac

exit $?
