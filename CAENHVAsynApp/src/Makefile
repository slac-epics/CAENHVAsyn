TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

USR_CXXFLAGS += -std=c++11

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
#======================================================

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

