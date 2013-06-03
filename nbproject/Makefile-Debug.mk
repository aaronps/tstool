#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW_4.7.2-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/2113088342/IMG_png.o \
	${OBJECTDIR}/_ext/2113088342/savepng.o \
	${OBJECTDIR}/_ext/1235180888/tstool.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk tstool-debug.exe

tstool-debug.exe: ${OBJECTFILES}
	${LINK.cc} -o tstool-debug ${OBJECTFILES} ${LDLIBSOPTIONS} -static-libgcc -static-libstdc++ -lpng -lSDL -lmingw32 -mconsole

${OBJECTDIR}/_ext/2113088342/IMG_png.o: /D/prog/tstool/src/others/IMG_png.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/2113088342
	${RM} $@.d
	$(COMPILE.c) -g -I/D/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2113088342/IMG_png.o /D/prog/tstool/src/others/IMG_png.c

${OBJECTDIR}/_ext/2113088342/savepng.o: /D/prog/tstool/src/others/savepng.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/2113088342
	${RM} $@.d
	$(COMPILE.c) -g -I/D/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2113088342/savepng.o /D/prog/tstool/src/others/savepng.c

${OBJECTDIR}/_ext/1235180888/tstool.o: /D/prog/tstool/src/tstool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1235180888
	${RM} $@.d
	$(COMPILE.cc) -g -I/D/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1235180888/tstool.o /D/prog/tstool/src/tstool.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} tstool-debug.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
