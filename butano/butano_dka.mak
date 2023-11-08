#---------------------------------------------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
    $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

BN_TOOLS	:=	$(LIBBUTANOABS)/tools
include $(DEVKITARM)/gba_rules

BN_TOOLCHAIN_CFLAGS	:=	
BN_GRIT				:=	grit
BN_MMUTIL			:=	mmutil

#---------------------------------------------------------------------------------------------------------------------
# Butano custom base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/custom_base_rules.mak

#---------------------------------------------------------------------------------------------------------------------
# Butano custom .gba rule with spaces allowed:
#---------------------------------------------------------------------------------------------------------------------
%.gba: %.elf
	$(SILENTCMD)$(OBJCOPY) -O binary $< $@
	@echo Fixing $(notdir $@)...
	$(SILENTCMD)gbafix -t"$(ROMTITLE)" -c"$(ROMCODE)" $@

#---------------------------------------------------------------------------------------------------------------------
# Butano custom link rules for avoiding issues when linking too much object files:
#---------------------------------------------------------------------------------------------------------------------
%_mb.elf:
	$(SILENTMSG) Linking multiboot...
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) end
endif
	@echo $(OFILES) > bn_ofiles.txt
	$(SILENTCMD)$(LD) $(LDFLAGS) -specs=gba_mb.specs @bn_ofiles.txt $(LIBPATHS) $(LIBS) -o $@
	
%.elf:
	$(SILENTMSG) Linking ROM...
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) end
endif
	@echo $(OFILES) > bn_ofiles.txt
	$(SILENTCMD)$(LD) $(LDFLAGS) -specs=gba.specs @bn_ofiles.txt $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------------------------------------------
# Options for code generation:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/codegen_options.mak

LDFLAGS	=	-gdwarf-4 $(ARCH) $(BN_NODEFAULT_LIBS) -Wl,-Map,$(notdir $*.map) $(USERLDFLAGS)

#---------------------------------------------------------------------------------------------------------------------
# Sources setup:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/sources_setup.mak

ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------------------------------------------
# Common setup:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/common_setup.mak

#---------------------------------------------------------------------------------------------------------------------
else
 
#---------------------------------------------------------------------------------------------------------------------
# Main targets:
#---------------------------------------------------------------------------------------------------------------------

$(OUTPUT).gba       :   $(OUTPUT).elf

$(OUTPUT).elf       :	$(OFILES)

$(OFILES_SOURCES)   :   $(HFILES)

#---------------------------------------------------------------------------------------------------------------------
# The bin2o rule should be copied and modified for each extension used in the data directories:
#---------------------------------------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension:
#---------------------------------------------------------------------------------------------------------------------
%.bin.o	%_bin.h : %.bin
#---------------------------------------------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------------------------------------
