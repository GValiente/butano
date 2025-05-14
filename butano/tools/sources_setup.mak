
#---------------------------------------------------------------------------------------------------------------------
# Any extra libraries we wish to link with the project:
#---------------------------------------------------------------------------------------------------------------------
LIBS        := -lgcc -lstdc++ $(USERLIBS)

ifeq ($(strip $(AUDIOBACKEND)),maxmod)
	LIBS		+=	-lmm
endif

ifeq ($(strip $(AUDIOBACKEND)),aas)
	LIBS		+=	-lAAS
endif

#---------------------------------------------------------------------------------------------------------------------
# List of directories containing libraries, this must be the top level containing include and lib directories:
#---------------------------------------------------------------------------------------------------------------------
LIBDIRS     :=	$(LIBBUTANOABS) $(LIBBUTANOABS)/hw/3rd_party/libtonc $(LIBBUTANOABS)/hw/3rd_party/libugba \
				$(USERLIBDIRS)

ifeq ($(strip $(AUDIOBACKEND)),maxmod)
	LIBDIRS		+=	$(LIBBUTANOABS)/hw/3rd_party/maxmod
endif

ifeq ($(strip $(AUDIOBACKEND)),aas)
	LIBDIRS		+=	$(LIBBUTANOABS)/hw/3rd_party/aas
endif

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
				$(LIBBUTANOABS)/hw/3rd_party/cult-of-gba-bios/src

ifeq ($(strip $(DMGAUDIOBACKEND)),default)
	BNSOURCES	+=	$(LIBBUTANOABS)/hw/3rd_party/gbt-player/src \
					$(LIBBUTANOABS)/hw/3rd_party/vgm-player/src
endif
