TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_CXXFLAGS += -std=c++0x

# CAENHVAsyn.dbd will be created from CAENHVAsynInclude.dbd and installed
DBD += CAENHVAsyn.dbd

INC += drvCAENHVAsyn.h

LIBRARY_IOC += CAENHVAsyn
LIB_SRCS += drvCAENHVAsyn.cpp
LIB_SRCS += common.cpp
LIB_SRCS += crate.cpp
LIB_SRCS += system_property.cpp
LIB_SRCS += board.cpp
LIB_SRCS += board_parameter.cpp
LIB_SRCS += channel.cpp
LIB_SRCS += channel_parameter.cpp
LIB_LIBS += asyn

#=====================================================
# Path to "NON EPICS" External PACKAGES: USER INCLUDES
#======================================================

USR_INCLUDES = $(addprefix -I,$(CAENHVWRAPPER_INCLUDE))
caenhvwrapper_DIR = $(CAENHVWRAPPER_LIB)
USR_LIBS_Linux += caenhvwrapper

LIB_LIBS_WIN32 += caenhvwrapper
BIN_INSTALLS_WIN32 += $(wildcard $(CAENHVWRAPPER_DLL)/*.dll)
LIB_INSTALLS_WIN32 += $(wildcard $(CAENHVWRAPPER_LIB)/*.lib)
LIB_SYS_LIBS_WIN32 += ws2_32
LIB_LIBS_WIN32 += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

