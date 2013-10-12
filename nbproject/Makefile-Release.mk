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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1869484693/SDL_png.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_bmp2png.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_comparetiles.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_converter.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_deletetiles.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_extract.o \
	${OBJECTDIR}/_ext/1869484693/TSTool_info.o \
	${OBJECTDIR}/_ext/1869484693/Tileset.o \
	${OBJECTDIR}/_ext/1869484693/ToolManager.o \
	${OBJECTDIR}/_ext/1869484693/jsoncpp.o \
	${OBJECTDIR}/_ext/1869484693/tstool.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk tstool.exe

tstool.exe: ${OBJECTFILES}
	${LINK.cc} -o tstool ${OBJECTFILES} ${LDLIBSOPTIONS} -static-libgcc -static-libstdc++ -lpng -lz -lSDL -lmingw32 -mconsole

${OBJECTDIR}/_ext/1869484693/SDL_png.o: /C/prog/c++/tstool/src/SDL_png.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.c) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/SDL_png.o /C/prog/c++/tstool/src/SDL_png.c

${OBJECTDIR}/_ext/1869484693/TSTool_bmp2png.o: /C/prog/c++/tstool/src/TSTool_bmp2png.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_bmp2png.o /C/prog/c++/tstool/src/TSTool_bmp2png.cpp

${OBJECTDIR}/_ext/1869484693/TSTool_comparetiles.o: /C/prog/c++/tstool/src/TSTool_comparetiles.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_comparetiles.o /C/prog/c++/tstool/src/TSTool_comparetiles.cpp

${OBJECTDIR}/_ext/1869484693/TSTool_converter.o: /C/prog/c++/tstool/src/TSTool_converter.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_converter.o /C/prog/c++/tstool/src/TSTool_converter.cpp

${OBJECTDIR}/_ext/1869484693/TSTool_deletetiles.o: /C/prog/c++/tstool/src/TSTool_deletetiles.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_deletetiles.o /C/prog/c++/tstool/src/TSTool_deletetiles.cpp

${OBJECTDIR}/_ext/1869484693/TSTool_extract.o: /C/prog/c++/tstool/src/TSTool_extract.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_extract.o /C/prog/c++/tstool/src/TSTool_extract.cpp

${OBJECTDIR}/_ext/1869484693/TSTool_info.o: /C/prog/c++/tstool/src/TSTool_info.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/TSTool_info.o /C/prog/c++/tstool/src/TSTool_info.cpp

${OBJECTDIR}/_ext/1869484693/Tileset.o: /C/prog/c++/tstool/src/Tileset.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/Tileset.o /C/prog/c++/tstool/src/Tileset.cpp

${OBJECTDIR}/_ext/1869484693/ToolManager.o: /C/prog/c++/tstool/src/ToolManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/ToolManager.o /C/prog/c++/tstool/src/ToolManager.cpp

${OBJECTDIR}/_ext/1869484693/jsoncpp.o: /C/prog/c++/tstool/src/jsoncpp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/jsoncpp.o /C/prog/c++/tstool/src/jsoncpp.cpp

${OBJECTDIR}/_ext/1869484693/tstool.o: /C/prog/c++/tstool/src/tstool.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1869484693
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/C/MinGW/include/SDL -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1869484693/tstool.o /C/prog/c++/tstool/src/tstool.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} tstool.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
