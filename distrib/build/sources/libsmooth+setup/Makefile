# SmoothWall Build system.
#
# (c) SmoothWall Ltd 2005
#
# This code is distributed under the terms of the GPL v2.

include ../Makefile.conf

PACKAGE = libsmooth+setup
VERSION = 0.0

compile:
	@make -C $(COMPILE_DIR)

install: compile
	@install -D $(COMPILE_DIR)/setup/setup $(PKG_ROOT)/usr/sbin/setup
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.so $(PKG_ROOT)/usr/lib/libsmooth.so
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.h /usr/include/libsmooth.h
	@install -D $(COMPILE_DIR)/libsmooth/langs.h /usr/include/langs.h
	@install -D $(COMPILE_DIR)/libsmooth/libsmooth.o /usr/lib/libsmooth.o

all: install
	@tar -zcvf $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz -C $(PKG_ROOT) .

clean:
	@make -C $(COMPILE_DIR) clean
	@rm -rf $(PKG_ROOT)
	@rm -f $(TARGET_DIR)/smoothwall-$(PACKAGE).tar.gz

download:
	@true

packageinfo.html: clean
	@echo "<li><span style='font-size:large;'>$(PACKAGE) $(VERSION)</span><br>" >>/tmp/packageinfo.html
	@tar --exclude .svn -zcvf /tmp/downloads/$(TARBALL).tar.gz $(DIR)
	@echo "Download: <a href='downloads/$(TARBALL).tar.gz'>$(TARBALL).tar.gz</a><br>" >>/tmp/packageinfo.html
