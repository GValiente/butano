#---------------------------------------------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
    $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

#---------------------------------------------------------------------------------------------------------------------
# Butano custom IWRAM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_iwram.o: %.bn_iwram.cpp
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_iwram.d $(CXXFLAGS) -fno-lto -marm -c $< -o $@ $(ERROR_FILTER)

%.bn_iwram.o: %.bn_iwram.c
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_iwram.d $(CFLAGS) -fno-lto -marm -c $< -o $@ $(ERROR_FILTER)
	
#---------------------------------------------------------------------------------------------------------------------
# Butano custom EWRAM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_ewram.o: %.bn_ewram.cpp
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_ewram.d $(CXXFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

%.bn_ewram.o: %.bn_ewram.c
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_ewram.d $(CFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------------------------------------------
# Butano custom ROM base rules without flto:
#---------------------------------------------------------------------------------------------------------------------
%.bn_noflto.o: %.bn_noflto.cpp
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.bn_noflto.d $(CXXFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

%.bn_noflto.o: %.bn_noflto.c
	$(SILENTMSG) $(notdir $<)
	$(SILENTCMD)$(CC) -MMD -MP -MF $(DEPSDIR)/$*.bn_noflto.d $(CFLAGS) -fno-lto -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------------------------------------------
# Options for code generation:
#---------------------------------------------------------------------------------------------------------------------
ARCH        :=	-mthumb -mthumb-interwork
CWARNINGS   :=	-Wall -Wextra -Wpedantic -Wshadow -Wundef -Wunused-parameter -Wmisleading-indentation \
				-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wswitch-default \
				-Wstack-usage=16384

CFLAGS      :=	$(CWARNINGS) -g -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -ffunction-sections -fdata-sections $(ARCH)
CFLAGS      +=	$(INCLUDE)
CFLAGS      +=	$(USERFLAGS)

CPPWARNINGS	:=	-Wuseless-cast -Wnon-virtual-dtor -Woverloaded-virtual
CXXFLAGS    :=	$(CFLAGS) $(CPPWARNINGS) -std=c++20 -fno-rtti -fno-exceptions

ASFLAGS     :=	-g $(ARCH)
LDFLAGS     =	-g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------------------------------------------
# Any extra libraries we wish to link with the project:
#---------------------------------------------------------------------------------------------------------------------
LIBS        := -lmm

#---------------------------------------------------------------------------------------------------------------------
# List of directories containing libraries, this must be the top level containing include and lib directories:
#---------------------------------------------------------------------------------------------------------------------
LIBDIRS     :=	$(LIBBUTANOABS) $(LIBBUTANOABS)/hw/3rd_party/libtonc $(LIBGBA)

#---------------------------------------------------------------------------------------------------------------------
# List of directories containing all butano source files:
#---------------------------------------------------------------------------------------------------------------------
BNSOURCES	:=	$(LIBBUTANOABS)/src $(LIBBUTANOABS)/hw/src \
                                $(LIBBUTANOABS)/hw/3rd_party/libtonc/asm \
                                $(LIBBUTANOABS)/hw/3rd_party/libtonc/src \
                                $(LIBBUTANOABS)/hw/3rd_party/libtonc/src/font \
                                $(LIBBUTANOABS)/hw/3rd_party/libtonc/src/tte \
                                $(LIBBUTANOABS)/hw/3rd_party/posprintf/src \
                                $(LIBBUTANOABS)/hw/3rd_party/gba-modern/src \
                                $(LIBBUTANOABS)/hw/3rd_party/dldi/src

#---------------------------------------------------------------------------------------------------------------------
# Don't remove intermediary files (avoid rebuilding graphics files more than once):
#---------------------------------------------------------------------------------------------------------------------
.SECONDARY:

ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------------------------------------------
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
export VPATH	:=  $(foreach dir,	$(SOURCES),	$(CURDIR)/$(dir)) \
                        $(foreach dir,	$(BNSOURCES),	$(dir)) \
                        $(foreach dir,	$(DATA),	$(CURDIR)/$(dir)) \
                        $(foreach dir,	$(GRAPHICS),	$(CURDIR)/$(dir))

export DEPSDIR	:=  $(CURDIR)/$(BUILD)

CFILES          :=  $(foreach dir,	$(SOURCES),	$(notdir $(wildcard $(dir)/*.c))) \
						$(foreach dir,	$(BNSOURCES),	$(notdir $(wildcard $(dir)/*.c)))
						
CPPFILES        :=	$(foreach dir,	$(SOURCES),	$(notdir $(wildcard $(dir)/*.cpp))) \
						$(foreach dir,	$(BNSOURCES),	$(notdir $(wildcard $(dir)/*.cpp)))
						
SFILES          :=	$(foreach dir,	$(SOURCES),	$(notdir $(wildcard $(dir)/*.s))) \
						$(foreach dir,	$(BNSOURCES),	$(notdir $(wildcard $(dir)/*.s)))
						
BINFILES        :=	$(foreach dir,	$(DATA),	$(notdir $(wildcard $(dir)/*.*))) \
						_bn_audio_soundbank.bin
						
GRAPHICSFILES	:=	$(foreach dir,	$(GRAPHICS),	$(notdir $(wildcard $(dir)/*.bmp)))

#---------------------------------------------------------------------------------------------------------------------
# Use CXX for linking C++ projects, CC for standard C:
#---------------------------------------------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------------------------------------------
    export LD   :=  $(CC)
#---------------------------------------------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------------------------------------------
    export LD	:=  $(CXX)
#---------------------------------------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------------------------------------

export OFILES_BIN       :=  $(addsuffix .o,$(BINFILES))

export OFILES_GRAPHICS	:=  $(GRAPHICSFILES:.bmp=_bn_graphics.o)

export OFILES_SOURCES   :=  $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
 
export OFILES           :=  $(OFILES_BIN) $(OFILES_GRAPHICS) $(OFILES_SOURCES)

#---------------------------------------------------------------------------------------------------------------------
# Don't generate header files from audio soundbank (avoid rebuilding all sources when audio files are updated):
#---------------------------------------------------------------------------------------------------------------------
export HFILES           :=  $(filter-out _bn_audio_soundbank_bin.h,$(addsuffix .h,$(subst .,_,$(BINFILES))))

export INCLUDE          :=  $(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
                                $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                                -I$(CURDIR)/$(BUILD)
 
export LIBPATHS         :=  $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean
 
#---------------------------------------------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(EXTTOOL)
	@$(PYTHON) -B $(LIBBUTANOABS)/tools/butano-audio-tool.py --audio="$(AUDIO)" --build=$(BUILD)
	@$(PYTHON) -B $(LIBBUTANOABS)/tools/butano-graphics-tool.py --graphics="$(GRAPHICS)" --build=$(BUILD)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).elfbin $(TARGET).gba 

#---------------------------------------------------------------------------------------------------------------------
else
 
#---------------------------------------------------------------------------------------------------------------------
# Main targets:
#---------------------------------------------------------------------------------------------------------------------

$(OUTPUT).gba       :   $(OUTPUT).elfbin
	@echo Fixing $(notdir $(OUTPUT).elfbin) ...
	$(SILENTCMD)gbafix -t"$(ROMTITLE)" -c"$(ROMCODE)" $(OUTPUT).elfbin
	$(SILENTCMD)@cp $(OUTPUT).elfbin $(OUTPUT).gba
	@echo Output file: $(notdir $(OUTPUT).gba)
	
$(OUTPUT).elfbin    :   $(OUTPUT).elf
	$(SILENTCMD)$(OBJCOPY) -O binary $< $@

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
