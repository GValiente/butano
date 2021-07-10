#---------------------------------------------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# LIBBUTANO is the main directory of butano library (https://github.com/GValiente/butano).
# PYTHON is the path to the python interpreter.
# SOURCES is a list of directories containing source code.
# INCLUDES is a list of directories containing extra header files.
# DATA is a list of directories containing binary data.
# GRAPHICS is a list of directories containing files to be processed by grit.
# AUDIO is a list of directories containing files to be processed by mmutil.
# ROMTITLE is a uppercase ASCII, max 12 characters text string containing the output ROM title.
# ROMCODE is a uppercase ASCII, max 4 characters text string containing the output ROM code.
# USERFLAGS is a list of additional compiler flags:
#     Pass -flto to enable link-time optimization.
#     Pass -O0 to improve debugging.
# EXTTOOL is an optional command executed before processing audio, graphics and code files.
#
# All directories are specified relative to the project directory where the makefile is found.
#---------------------------------------------------------------------------------------------------------------------
TARGET      :=  $(notdir $(CURDIR))
BUILD       :=  build
LIBBUTANO   :=  ../../butano
PYTHON      :=  python
SOURCES     :=  src
INCLUDES    :=  include
DATA        :=
GRAPHICS    :=  graphics graphics/shape_group_textures
AUDIO       :=  audio
ROMTITLE    :=  VAROOOM 3D
ROMCODE     :=  SV3D
USERFLAGS   :=  -DBN_CFG_AUDIO_MIXING_RATE=BN_AUDIO_MIXING_RATE_21_KHZ \
                                        -DBN_CFG_GAME_PAK_WAIT_STATE_FIRST=BN_GAME_PAK_WAIT_STATE_FIRST_3 \
                                        -DBN_CFG_GAME_PAK_WAIT_STATE_SECOND=BN_GAME_PAK_WAIT_STATE_SECOND_1
EXTTOOL     :=

#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak
