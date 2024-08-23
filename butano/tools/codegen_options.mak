#---------------------------------------------------------------------------------------------------------------------
# Options for code generation:
#---------------------------------------------------------------------------------------------------------------------
ARCH        :=	-mthumb -mthumb-interwork $(BN_TOOLCHAIN_CFLAGS)

ifdef STACKTRACE
	ARCH		+=	-funwind-tables -mpoke-function-name -DBN_STACKTRACE
endif

CWARNINGS   :=	-Wall -Wextra -Wpedantic -Wshadow -Wundef -Wunused-parameter -Wmisleading-indentation \
				-Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wswitch-default \
				-Wstack-usage=16384

CFLAGS      :=	$(CWARNINGS) -gdwarf-4 -O2 -mcpu=arm7tdmi -mtune=arm7tdmi -ffast-math -ffunction-sections \
				-fdata-sections $(ARCH)
CFLAGS      +=	$(INCLUDE)
CFLAGS      +=	$(USERFLAGS)

CPPWARNINGS	:=	-Wuseless-cast -Wnon-virtual-dtor -Woverloaded-virtual
CXXFLAGS    :=	$(CFLAGS) $(CPPWARNINGS) -std=c++23 -fno-rtti -fno-exceptions -fno-threadsafe-statics \
				-fuse-cxa-atexit $(USERCXXFLAGS)

ASFLAGS     :=	-gdwarf-4 $(ARCH) $(USERASFLAGS)

ifndef DEFAULTLIBS
	BN_NODEFAULT_LIBS	:=	-nodefaultlibs
else
	BN_NODEFAULT_LIBS	:=	
endif
