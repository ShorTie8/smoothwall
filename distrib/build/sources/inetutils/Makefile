# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = inetutils
VERSION = 1.5

BASE_URL = ftp://ftp.gnu.org/gnu/inetutils/

CONFIG_OPTS = -sysconfdir=/etc --localstatedir=/var --disable-logger \
	--disable-syslogd --disable-whois --disable-servers  --disable-talk \
	--disable-rcp --disable-rlogin --disable-rsh --disable-ping \
	--disable-ifconfig

include ../Makefile.rules
