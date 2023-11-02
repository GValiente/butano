#---------------------------------------------------------------------------------------------------------------------
# Any extra libraries we wish to link with the project:
#---------------------------------------------------------------------------------------------------------------------
LIBS        := -lgcc $(USERLIBS)

#---------------------------------------------------------------------------------------------------------------------
# List of directories containing libraries, this must be the top level containing include and lib directories:
#---------------------------------------------------------------------------------------------------------------------
LIBDIRS     :=	$(LIBBUTANOABS) $(LIBBUTANOABS)/hw/3rd_party/libtonc $(LIBBUTANOABS)/hw/3rd_party/libugba \
				$(LIBBUTANOABS)/hw/3rd_party/maxmod $(USERLIBDIRS)

#---------------------------------------------------------------------------------------------------------------------
# List of directories containing all butano source files:
#---------------------------------------------------------------------------------------------------------------------
BNSOURCES	:=	$(LIBBUTANOABS)/src $(LIBBUTANOABS)/hw/src \
				$(LIBBUTANOABS)/hw/3rd_party/libtonc/asm \
				$(LIBBUTANOABS)/hw/3rd_party/libtonc/src \
				$(LIBBUTANOABS)/hw/3rd_party/libtonc/src/font \
				$(LIBBUTANOABS)/hw/3rd_party/libtonc/src/tte \
				$(LIBBUTANOABS)/hw/3rd_party/libugba/src \
				$(LIBBUTANOABS)/hw/3rd_party/posprintf/src \
				$(LIBBUTANOABS)/hw/3rd_party/agbabi/src \
				$(LIBBUTANOABS)/hw/3rd_party/gba-modern/src \
				$(LIBBUTANOABS)/hw/3rd_party/cult-of-gba-bios/src \
				$(LIBBUTANOABS)/hw/3rd_party/gbt-player/src \
				$(LIBBUTANOABS)/hw/3rd_party/vgm-player/src \
				$(LIBBUTANOABS)/hw/3rd_party/maxmod/source \
				$(LIBBUTANOABS)/hw/3rd_party/maxmod/source_gba
