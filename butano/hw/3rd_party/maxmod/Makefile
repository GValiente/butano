export MAXMOD_MAJOR	:= 1
export MAXMOD_MINOR	:= 0
export MAXMOD_PATCH	:= 12

VERSTRING	:=	$(MAXMOD_MAJOR).$(MAXMOD_MINOR).$(MAXMOD_PATCH)

.PHONY:	all clean install gba install-gba ds7 ds9 install-nds

all: install

clean:
	rm -fr lib build_gba build_ds7 build_ds9

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

gba:
	$(MAKE) -f maxmod.mak SYSTEM=GBA

ds7:
	$(MAKE) -f maxmod.mak SYSTEM=DS7

ds9:
	$(MAKE) -f maxmod.mak SYSTEM=DS9
