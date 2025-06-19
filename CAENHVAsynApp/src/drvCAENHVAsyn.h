#ifndef DRV_CAEN_HV_ASYN_H
#define DRV_CAEN_HV_ASYN_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : drvCAENHVAsyn.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-07-23
 * ----------------------------------------------------------------------------
 * Description:
 * EPICS Module for CAEN HV Power supplies
 * ----------------------------------------------------------------------------
 * This file is part of l2MpsAsyn. It is subject to
 * the license terms in the LICENSE.txt file found in the top-level directory
 * of this distribution and at:
    * https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html.
 * No part of l2MpsAsyn, including this file, may be
 * copied, modified, propagated, or distributed except according to the terms
 * contained in the LICENSE.txt file.
 * ----------------------------------------------------------------------------
**/

#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <arpa/inet.h>
#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>
#include <dbAccess.h>
#include <dbStaticLib.h>
#include "asynPortDriver.h"
#include <epicsExport.h>
#include "epicsAtomic.h"

#include "CAENHVWrapper.h"
#include "common.h"
#include "crate.h"

#define MAX_SIGNALS (3)
#define NUM_PARAMS (1500)

// Map used to generated binary records for system parameters of type 'PARAM_TYPE_CHSTATUS'.
// There will be a bi and or bo record for each bit status.
// This maps contains MASK, a suffix appended to the record name, Record description.
// - This map is for Board parameters of type 'PARAM_TYPE_CHSTATUS'
typedef const std::map< int, std::pair< std::string, std::string > > statusRecordMap_t;
statusRecordMap_t recordFieldBdParamChStatus =
{
    { 0x001, std::pair<std::string,std::string>( "_ON", "Ch is on"                   ) },
    { 0x002, std::pair<std::string,std::string>( "_RU", "Ch is ramping up"           ) },
    { 0x004, std::pair<std::string,std::string>( "_RD", "Ch is ramping down"         ) },
    { 0x008, std::pair<std::string,std::string>( "_OC", "Ch is in overcurrent"       ) },
    { 0x010, std::pair<std::string,std::string>( "_OV", "Ch is in overvoltage"       ) },
    { 0x020, std::pair<std::string,std::string>( "_UV", "Ch is in undervoltage"      ) },
    { 0x040, std::pair<std::string,std::string>( "_ET", "Ch is in external trip"     ) },
    { 0x080, std::pair<std::string,std::string>( "_MV", "Ch is in max V"             ) },
    { 0x100, std::pair<std::string,std::string>( "_ED", "Ch is in external disable"  ) },
    { 0x200, std::pair<std::string,std::string>( "_IT", "Ch is in internal trip"     ) },
    { 0x400, std::pair<std::string,std::string>( "_CE", "Ch is in calibration error" ) },
    { 0x800, std::pair<std::string,std::string>( "_UN", "Ch is unplugged"            ) },
};
// - This map is for Channel parameters of type 'PARAM_TYPE_CHSTATUS'
statusRecordMap_t   recordFieldChParamChStatus =
{
    { 0x0001, std::pair<std::string,std::string>( "_ON", "Ch is on"                         ) },
    { 0x0002, std::pair<std::string,std::string>( "_RU", "Ch is ramping up"                 ) },
    { 0x0004, std::pair<std::string,std::string>( "_RD", "Ch is ramping down"               ) },
    { 0x0008, std::pair<std::string,std::string>( "_OC", "Ch is in overcurrent"             ) },
    { 0x0010, std::pair<std::string,std::string>( "_OV", "Ch is in overvoltage"             ) },
    { 0x0020, std::pair<std::string,std::string>( "_UV", "Ch is in undervoltage"            ) },
    { 0x0040, std::pair<std::string,std::string>( "_ET", "Ch is in external trip"           ) },
    { 0x0080, std::pair<std::string,std::string>( "_MV", "Ch is in max V"                   ) },
    { 0x0100, std::pair<std::string,std::string>( "_ED", "Ch is in external disable"        ) },
    { 0x0200, std::pair<std::string,std::string>( "_IT", "Ch is in internal trip"           ) },
    { 0x0400, std::pair<std::string,std::string>( "_CE", "Ch is in calibration error"       ) },
    { 0x0800, std::pair<std::string,std::string>( "_UN", "Ch is unplugged"                  ) },
    { 0x2000, std::pair<std::string,std::string>( "_OVP", "Ch is in OverVoltage Protection" ) },
    { 0x4000, std::pair<std::string,std::string>( "_PF", "Ch is in Power Fail"              ) },
    { 0x8000, std::pair<std::string,std::string>( "_TE", "Ch is in Temperature Error"       ) },
};

// Map used to generate binary records for the system parameters of type 'PARAM_TYPE_BDSTATUS'.
// There will be a bi and or bo record for each bit status.
// This maps contains MASK, a suffix appended to the record name, Record description.
// - This map is for Board parameters of type 'PARAM_TYPE_CHSTATUS'
statusRecordMap_t recordFieldBdParamBdStatus =
{
    { 0x001, std::pair<std::string,std::string>( "_PF",   "Bd is in power-fail status"         ) },
    { 0x002, std::pair<std::string,std::string>( "_FCE",  "Bd has a firmware checksum error"   ) },
    { 0x004, std::pair<std::string,std::string>( "_CEHV", "Bd has calibration error on HV"     ) },
    { 0x008, std::pair<std::string,std::string>( "_CET",  "Bd has a calibration error on temp" ) },
    { 0x010, std::pair<std::string,std::string>( "_UT",   "Bd is in under-temperature status"  ) },
    { 0x020, std::pair<std::string,std::string>( "_OT",   "Bd is in over-temperature status"   ) },
};

class CAENHVAsyn : public asynPortDriver
{
    public:
        CAENHVAsyn(const std::string& portName, int systemType, const std::string& ipAddr, const std::string& userName, const std::string& password);

        // Methods that we override from asynPortDriver
        virtual asynStatus readFloat64        (asynUser *pasynUser, epicsFloat64 *value);
        virtual asynStatus writeFloat64       (asynUser *pasynUser, epicsFloat64 value);
        virtual asynStatus readUInt32Digital  (asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);
        virtual asynStatus writeUInt32Digital (asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
        virtual asynStatus readOctet          (asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
        virtual asynStatus writeOctet         (asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
        virtual asynStatus readInt32          (asynUser *pasynUser, epicsInt32 *value);
        virtual asynStatus writeInt32         (asynUser *pasynUser, epicsInt32 value);

        //Connection monitor task to be called inside epicsThread
        void connMon();

        // EPICS record prefix. Use for autogeneration of PVs.
        static std::string epicsPrefix;
        // Crate information output file location
        static std::string crateInfoFilePath;

    private:


        // Methods to create EPICS asyn parameters and records for all system, board, and channel parameters
        template<typename T>
        void createParamFloat(T p, std::map<int, T>& list);
        template <typename T>
        void createParamBinary(T p, std::map<int, T>& list);
        template <typename T>
        void createParamMBinary(T p, std::map<int, T>& list, const statusRecordMap_t& recordMap);
        template <typename T>
        void createParamInteger(T p, std::map<int, T>& list);
        template <typename T>
        void createParamString(T p, std::map<int, T>& list);

        const std::string driverName_;
        std::string portName_;

        // Crate object
        Crate crate;
        int failed_gets{0};

       // System property lists
       std::map<int, SystemPropertyInteger> systemPropertyIntegerList;
       std::map<int, SystemPropertyString>  systemPropertyStringList;
       std::map<int, SystemPropertyFloat>   systemPropertyFloatList;

       // Board parameter lists
       std::map<int, BoardParameterNumeric>  boardParameterNumericList;
       std::map<int, BoardParameterOnOff>    boardParameterOnOffList;
       std::map<int, BoardParameterChStatus> boardParameterChStatusList;
       std::map<int, BoardParameterBdStatus> boardParameterBdStatusList;

       // Channel parameter lists
       std::map<int, ChannelParameterNumeric>  channelParameterNumericList;
       std::map<int, ChannelParameterOnOff>    channelParameterOnOffList;
       std::map<int, ChannelParameterChStatus> channelParameterChStatusList;
       std::map<int, ChannelParameterBinary>   channelParameterBinaryList;
};

#endif
