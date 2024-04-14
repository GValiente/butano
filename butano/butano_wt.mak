#---------------------------------------------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------------------------------------------

ifeq ($(strip $(WONDERFUL_TOOLCHAIN)),)
    $(error "WONDERFUL_TOOLCHAIN not found")
endif

BN_TOOLS	:=	$(LIBBUTANOABS)/tools
include $(BN_TOOLS)/wt_setup.mak

BN_TOOLCHAIN_CFLAGS	:=	-specs=$(WF)/target/$(WF_TARGET)/$(WF_SUBTARGET)/gcc.specs \
						-DBN_EWRAM_BSS_SECTION=\".ewram_bss\" -DBN_IWRAM_START=__iwram_start \
						-DBN_IWRAM_TOP=__sp_sys -DBN_IWRAM_END=__iwram_end -DBN_ROM_START=__load \
						-DBN_ROM_END=__load_end -DBN_TOOLCHAIN_TAG=\"WT\"
BN_GRIT				:=	$(WONDERFUL_TOOLCHAIN)/thirdparty/blocksds/core/tools/grit/grit
BN_MMUTIL			:=	$(WONDERFUL_TOOLCHAIN)/thirdparty/blocksds/core/tools/mmutil/mmutil

#---------------------------------------------------------------------------------------------------------------------
# Butano custom base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/custom_base_rules.mak

#---------------------------------------------------------------------------------------------------------------------
# Butano custom .gba rule with spaces allowed:
#---------------------------------------------------------------------------------------------------------------------
%.gba:
	@echo Fixing $(notdir $@)...
	$(SILENTCMD)$(WONDERFUL_TOOLCHAIN)/bin/wf-gbatool fix $@ -l official -t "$(ROMTITLE)" --code "$(ROMCODE)"
	
#---------------------------------------------------------------------------------------------------------------------
# Butano custom link rule for avoiding issues when linking too many object files:
#---------------------------------------------------------------------------------------------------------------------
%.elf:
ifeq ($(patsubst %_mb,,$(lastword $(TARGET))),)
	$(SILENTMSG) Linking multiboot...
else
	$(SILENTMSG) Linking ROM...
endif
ifdef ADD_COMPILE_COMMAND
	$(ADD_COMPILE_COMMAND) end
endif
	@echo $(OFILES) > bn_ofiles.txt
	$(SILENTCMD)$(ROMLINK) -c $(BN_TOOLS)/wt_config.toml -o $(OUTPUT).gba --output-elf $(OUTPUT).elf $(ROMLINKFLAGS) \
	-- $(LIBPATHS) @bn_ofiles.txt $(LIBS) $(LDFLAGS)

#---------------------------------------------------------------------------------------------------------------------
# Options for code generation:
#---------------------------------------------------------------------------------------------------------------------
include $(BN_TOOLS)/codegen_options.mak

LDFLAGS	=	-gdwarf-4 $(ARCH) $(BN_NODEFAULT_LIBS) -Wl,--no-warn-rwx-segments,--gc-sections,-Map,$(notdir $*.map) \
			$(foreach path,$(WF_ARCH_LIBDIRS),-L$(path)/lib) $(USERLDFLAGS)

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
	@echo "$<"
	$(SILENTCMD)$(MKDIR) -p $(@D)
	$(SILENTCMD)$(WF)/bin/wf-bin2s -a 4 $(@D) $< $*_bin
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(CPPFLAGS) $(ASFLAGS) \
		-c -o $(@D)/$*.bin.o $(@D)/$*_bin.s $(ERROR_FILTER)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------------------------------------
