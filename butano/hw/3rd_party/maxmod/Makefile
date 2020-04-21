export MAXMOD_MAJOR	:= 1
export MAXMOD_MINOR	:= 0
export MAXMOD_PATCH	:= 12

VERSTRING	:=	$(MAXMOD_MAJOR).$(MAXMOD_MINOR).$(MAXMOD_PATCH)

.PHONY:	gba ds7 ds9 ds9e

all: install

clean:
	rm -fr lib build_gba build_ds7 build_ds9 build_ds9e

install: install-gba install-nds install-ndse

install-gba: gba
	mkdir -p $(DESTDIR)$(DEVKITPRO)/libgba/include
	mkdir -p $(DESTDIR)$(DEVKITPRO)/libgba/lib
	cp lib/libmm.a $(DESTDIR)$(DEVKITPRO)/libgba/lib
	cp include/maxmod.h include/mm_types.h $(DESTDIR)$(DEVKITPRO)/libgba/include
	cp maxmod_license.txt $(DESTDIR)$(DEVKITPRO)/libgba

install-nds: ds7 ds9
	mkdir -p $(DESTDIR)$(DEVKITPRO)/libnds/include
	mkdir -p $(DESTDIR)$(DEVKITPRO)/libnds/lib
	cp lib/libmm7.a lib/libmm9.a $(DESTDIR)$(DEVKITPRO)/libnds/lib
	cp include/maxmod7.h include/maxmod9.h include/mm_types.h $(DESTDIR)$(DEVKITPRO)/libnds/include
	cp maxmod_license.txt $(DESTDIR)$(DEVKITPRO)/libnds

install-ndse: ds9e

dist: dist-gba dist-nds dist-nds9e dist-src

dist-gba:	gba
	@tar --exclude=.svn -cjvf  maxmod-gba-$(VERSTRING).tar.bz2 include/maxmod.h include/mm_types.h lib/libmm.a maxmod_license.txt

dist-nds:	ds7 ds9
	@tar --exclude=.svn -cjvf maxmod-nds-$(VERSTRING).tar.bz2 include/maxmod7.h include/maxmod9.h include/mm_types.h lib/libmm7.a lib/libmm9.a maxmod_license.txt

dist-nds9e: ds9e

dist-src:
	@tar --exclude=.svn -cvjf maxmod-src-$(VERSTRING).tar.bz2 \
	asm_include include source* Makefile maxmod.mak maxmod_license.txt

gba:
	$(MAKE) -f maxmod.mak SYSTEM=GBA

ds9e:
	$(MAKE) -f maxmod.mak SYSTEM=DS9E

ds7:
	$(MAKE) -f maxmod.mak SYSTEM=DS7

ds9:
	$(MAKE) -f maxmod.mak SYSTEM=DS9
