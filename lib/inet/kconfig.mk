########################################################################
## Machine Type management
########################################################################

ifeq (,$(MACH))
	MACH=3354L
endif
ifeq (255,$(MACH))
	CROSS=/work/pxa255/arm-linux/bin/arm-linux-
	CXXFLAGS=-Wall -fpic -rdynamic -DD_ARM -DD_NOWRITE_FALSH
	LIBFLAGS=-shared -s
endif
ifeq (270,$(MACH))
	CROSS=arm-softfloat-linux-gnu-
	CXXFLAGS=-Wall -fpic -rdynamic -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270
	LIBFLAGS=-shared -s
endif
ifeq (3354A,$(MACH))
	CROSS=arm-arago-linux-gnueabi-
	CXXFLAGS=-Wall -fpic -s -rdynamic -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 -DD_3354 -DD_32BIT -DD_SOCKETCAN
	LIBFLAGS=-shared -s
endif
ifeq (3354L,$(MACH))
	CROSS=arm-linux-gnueabihf-
	CXXFLAGS=-Wall  -fpermissive  -fpic  -s -rdynamic -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 -DD_3354 -DD_32BIT -DD_SOCKETCAN
	LIBFLAGS=-shared -s
endif
ifeq (x86,$(MACH))
	CROSS=
	CXXFLAGS=-Wall -fpic -rdynamic -DD_X86 -DD_ATOM -g
	LIBFLAGS=-shared 
endif

########################################################################
## tools and some constant directories
########################################################################

CC=$(CROSS)gcc
CXX=$(CROSS)g++
OBJCOPY=$(CROSS)objcopy
NM=$(CROSS)nm
SED=sed
INSTALL=install
COPY=cp -Rf
ECHO=echo
SED=sed

PWD=$(shell pwd)
DEPDIR=.deps/
CURDIR=$(addsuffix /, $(notdir $(shell pwd)))
INSTALLDIR:=$(addprefix $(INSTALLDIR), $(CURDIR))

########################################################################
## Compile arguments
########################################################################

CXXFLAGS+=$(INCLUDES)
INCLUDES=-I/usr/src/rtlinux/rtlinux-3.2-pre2/include
LDFLAGS=-lpthread -lpng -ldl ./lib/simplefs/libsimplefs.so
DEBUGFLAGS=-g -DDEBUG -DD_DEBUG_TM
export MACH INSTALLDIR INSTALL

########################################################################
## NOTE: ultimate users can change this part only!
########################################################################

SUBDIRS=
EXTRADIRS=Dynamic Static $(LIBS)
EXTRAFILES=iNetData.map ProtocolConf.ini 
DELFILES= *.bak *~ *.so *.o
LIBS=tmTaskiNet.so \
