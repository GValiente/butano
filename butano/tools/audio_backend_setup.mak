#---------------------------------------------------------------------------------------------------------------------
# DMG audio backend setup:
#---------------------------------------------------------------------------------------------------------------------
ifeq ($(strip $(DMGAUDIOBACKEND)),)
    DMGAUDIOBACKEND = default
endif

ifeq ($(strip $(DMGAUDIOBACKEND)),null)
    BN_AUDIO_BACKEND_CFLAGS	:=	-DBN_DMG_AUDIO_BACKEND_NULL
else
ifeq ($(strip $(DMGAUDIOBACKEND)),default)
    BN_AUDIO_BACKEND_CFLAGS	:=	-DBN_DMG_AUDIO_BACKEND_DEFAULT
else
    $(error "Unsupported DMG audio backend: $(DMGAUDIOBACKEND)")
endif
endif
