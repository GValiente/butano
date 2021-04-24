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
SOURCES     :=  src ../../common/src
INCLUDES    :=  include ../../common/include
DATA        :=
GRAPHICS    :=  graphics ../../common/graphics
AUDIO       :=  audio ../../common/audio
ROMTITLE    :=  BUTANO AUDIO
ROMCODE     :=  SBTP
USERFLAGS   :=
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
