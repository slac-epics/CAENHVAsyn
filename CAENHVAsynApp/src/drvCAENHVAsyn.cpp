/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : drvCAENHVAsyn.cpp
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

#include "drvCAENHVAsyn.h"

// Default value for the EPICS record prefix is an empty string,
// which means that the autogeration is disabled.
std::string CAENHVAsyn::epicsPrefix;
std::string CAENHVAsyn::crateInfoFilePath = "/tmp/";

template <typename T>
void CAENHVAsyn::createParamFloat(T p, std::map<int, T>& list)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string desc       = p->getEpicsDesc();
    std::string mode       = p->getMode();
    std::string egu        = p->getUnits();
    float       min        = p->getMinVal();
    float       max        = p->getMaxVal();

    int index;
    createParam(paramName.c_str(), asynParamFloat64, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of parameter to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",DESC="  << desc;
        dbParamsLocal << ",EGU="   << egu;
        dbParamsLocal << ",LOPR="  << min;
        dbParamsLocal << ",HOPR="  << max;

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            dbParamsLocal << ",SCAN=1 second";
            dbParamsLocal << ",R=" << recordName << ":Rd";
            dbLoadRecords("db/ai.template", dbParamsLocal.str().c_str());
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            dbParamsLocal << ",DRVL=" << min;
            dbParamsLocal << ",DRVH=" << max;
            dbParamsLocal << ",R="    << recordName << ":St";
            dbLoadRecords("db/ao.template", dbParamsLocal.str().c_str());
        }

    }
}

// This is a particular case as the System parameter of type REAL doesn't have
// max, min, and units values
template <>
void CAENHVAsyn::createParamFloat(SystemPropertyFloat p, std::map<int, SystemPropertyFloat>& list)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string desc       = p->getEpicsDesc();
    std::string mode       = p->getMode();

    int index;
    createParam(paramName.c_str(), asynParamFloat64, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of parameter to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",DESC="  << desc;
        dbParamsLocal << ",EGU=";
        dbParamsLocal << ",LOPR=";
        dbParamsLocal << ",HOPR=";

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            dbParamsLocal << ",SCAN=1 second";
            dbParamsLocal << ",R=" << recordName << ":Rd";
            dbLoadRecords("db/ai.template", dbParamsLocal.str().c_str());
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            dbParamsLocal << ",DRVL=";
            dbParamsLocal << ",DRVH=";
            dbParamsLocal << ",R="    << recordName << ":St";
            dbLoadRecords("db/ao.template", dbParamsLocal.str().c_str());
        }

    }
}

template <typename T>
void CAENHVAsyn::createParamBinary(T p, std::map<int, T>& list)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string desc       = p->getEpicsDesc();
    std::string mode       = p->getMode();
    std::string onLabel    = p->getOnState();
    std::string offLabel   = p->getOffState();

    int index;
    createParam(paramName.c_str(), asynParamUInt32Digital, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of parameter to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",DESC="  << desc;
        dbParamsLocal << ",ZNAM="  << offLabel;
        dbParamsLocal << ",ONAM="  << onLabel;
        dbParamsLocal << ",MASK=1";

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            dbParamsLocal << ",SCAN=1 second";
            dbParamsLocal << ",R=" << recordName << ":Rd";
            dbLoadRecords("db/bi.template", dbParamsLocal.str().c_str());
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            dbParamsLocal << ",R="    << recordName << ":St";
            dbLoadRecords("db/bo.template", dbParamsLocal.str().c_str());
        }
    }
}

template <typename T>
void CAENHVAsyn::createParamMBinary(T p, std::map<int, T>& list, const statusRecordMap_t& recordMap)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string mode       = p->getMode();

    int index;
    createParam(paramName.c_str(), asynParamUInt32Digital, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of paramater to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",ZNAM=Off";
        dbParamsLocal << ",ONAM=On";

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            for (statusRecordMap_t::const_iterator it = recordMap.begin(); it != recordMap.end(); ++it)
            {
                std::stringstream dbParamsLocal2;
                dbParamsLocal2.str("");
                dbParamsLocal2 <<  dbParamsLocal.str();
                dbParamsLocal2 << ",SCAN=1 second";
                dbParamsLocal2 << ",MASK=" << it->first;
                dbParamsLocal2 << ",DESC=" << it->second.second;
                dbParamsLocal2 << ",R="    << recordName << it->second.first << ":Rd";
                dbLoadRecords("db/bi.template", dbParamsLocal2.str().c_str());
            }
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            for (statusRecordMap_t::const_iterator it = recordMap.begin(); it != recordMap.end(); ++it)
            {
                std::stringstream dbParamsLocal2;
                dbParamsLocal2.str("");
                dbParamsLocal2 <<  dbParamsLocal.str();
                dbParamsLocal2 << ",SCAN=Passive";
                dbParamsLocal2 << ",MASK=" << it->first;
                dbParamsLocal2 << ",DESC=" << it->second.second;
                dbParamsLocal2 << ",R="    << recordName << it->second.first << ":Rd";
                dbLoadRecords("db/bo.template", dbParamsLocal2.str().c_str());
            }
        }

    }
}

template <typename T>
void CAENHVAsyn::createParamInteger(T p, std::map<int, T>& list)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string desc       = p->getEpicsDesc();
    std::string mode       = p->getMode();

    int index;
    createParam(paramName.c_str(), asynParamInt32, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of paramater to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",DESC="  << desc;

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            dbParamsLocal << ",R=" << recordName << ":Rd";
            dbParamsLocal << ",SCAN=1 second";
            dbLoadRecords("db/longin.template", dbParamsLocal.str().c_str());
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            dbParamsLocal << ",R=" << recordName << ":St";
            dbLoadRecords("db/longout.template", dbParamsLocal.str().c_str());
        }
    }
}

template <typename T>
void CAENHVAsyn::createParamString(T p, std::map<int, T>& list)
{
    std::string paramName  = p->getEpicsParamName();
    std::string recordName = p->getEpicsRecordName();
    std::string desc       = p->getEpicsDesc();
    std::string mode       = p->getMode();

    int index;
    createParam(paramName.c_str(), asynParamOctet, &index);

    list.insert( std::make_pair(index, p) );

    if (!epicsPrefix.empty())
    {
        std::stringstream dbParamsLocal;

        // Create list of paramater to pass to the  dbLoadRecords function
        dbParamsLocal.str("");
        dbParamsLocal << "P="      << CAENHVAsyn::epicsPrefix;
        dbParamsLocal << ",PORT="  << portName_;
        dbParamsLocal << ",PARAM=" << paramName;
        dbParamsLocal << ",DESC="  << desc;
        dbParamsLocal << ",NELM=4096";

        if ( (!mode.compare("RW")) || (!mode.compare("RO")) )
        {
            dbParamsLocal << ",R=" << recordName << ":Rd";
            dbParamsLocal << ",SCAN=1 second";
            dbLoadRecords("db/stringin.template", dbParamsLocal.str().c_str());
        }

        if ( (!mode.compare("RW")) || (!mode.compare("WO")) )
        {
            dbParamsLocal << ",R=" << recordName << ":St";
            dbLoadRecords("db/stringout.template", dbParamsLocal.str().c_str());
        }
    }
}

CAENHVAsyn::CAENHVAsyn(const std::string& portName, int systemType, const std::string& ipAddr, const std::string& userName, const std::string& password)
:
    asynPortDriver(
        portName.c_str(),
        MAX_SIGNALS,
        NUM_PARAMS,
        asynInt32Mask | asynDrvUserMask | asynInt16ArrayMask | asynInt32ArrayMask | asynOctetMask | \
        asynFloat64ArrayMask | asynUInt32DigitalMask | asynFloat64Mask,                             // Interface Mask
        asynInt16ArrayMask | asynInt32ArrayMask | asynInt32Mask | asynUInt32DigitalMask,            // Interrupt Mask
        ASYN_MULTIDEVICE | ASYN_CANBLOCK,                                                           // asynFlags
        1,                                                                                          // Autoconnect
        0,                                                                                          // Default priority
        0),                                                                                         // Default stack size
    driverName_("CAENHVAsyn"),
    portName_(portName)
{
    // Check parameters
    if ( portName_.empty() )
        throw std::runtime_error("The port name must be defined");

    if ( userName.empty() )
        throw std::runtime_error("The user name must be defined");

    if ( ipAddr.empty() )
        throw std::runtime_error("The IP address must be defined");
    else
    {
        unsigned char buf[sizeof(struct in6_addr)];
        if (!inet_pton(AF_INET, ipAddr.c_str(), buf))
            throw std::runtime_error("Invalid IP address");
    }

    // Only SYx527 and SMART HV are supported at the moment
    if ( ( (systemType < 0) || (systemType > 3) ) && systemType != 13 )
        throw std::runtime_error("Unsupported system type. Only supported types are SYx527 (0-3) and SMART HV (13)");

    // Create a Crate object
    crate = ICrate::create(systemType, ipAddr, userName, password);

    // Print the crate map to the IOC shell
    std::cout << std::endl;
    crate->printCrateMap(std::cout);
    std::cout << std::endl;

    // Print Crate information
    //crate->printInfo(std::cout);

    // Print Crate information to a temporal file
    // This need to be reimplemented using RAII...
    // Also, the user should be able to override the output location
    std::string infoFileName(crateInfoFilePath + this->driverName_ + "_" + this->portName_ + "_crateInfo.txt");
    std::ofstream infoFile;

    std::cout << "Dumping crate information on '" << infoFileName << "'... ";
    infoFile.open(infoFileName);
    crate->printInfo(infoFile);
    infoFile.close();
    std::cout  << "Done" << std::endl;

    if (epicsPrefix.empty())
        std::cout << "Autogeneration of PVs is disabled." << std::endl;
    else
        std::cout << "Autogeneration of PVs is enabled with prefix '" << epicsPrefix << "'" << std::endl;

    // System properties
    {
        std::vector<SystemPropertyInteger> s = crate->getSystemPropertyIntegers();
        for (std::vector<SystemPropertyInteger>::iterator it = s.begin(); it != s.end(); ++it)
            createParamInteger<SystemPropertyInteger>(*it, systemPropertyIntegerList);
    }

    {
        std::vector<SystemPropertyFloat> s = crate->getSystemPropertyFloats();
        for (std::vector<SystemPropertyFloat>::iterator it = s.begin(); it != s.end(); ++it)
            createParamFloat<SystemPropertyFloat>(*it, systemPropertyFloatList);
    }

    {
        std::vector<SystemPropertyString> s = crate->getSystemPropertyStrings();
        for (std::vector<SystemPropertyString>::iterator it = s.begin(); it != s.end(); ++it)
            createParamString<SystemPropertyString>(*it, systemPropertyStringList);
    }

    // Boards
    std::vector<Board> b = crate->getBoards();

    for (std::vector<Board>::iterator boardIt = b.begin(); boardIt != b.end(); ++boardIt)
    {
        std::vector<BoardParameterNumeric> pn = (*boardIt)->getBoardParameterNumerics();

        for (std::vector<BoardParameterNumeric>::iterator paramIt = pn.begin(); paramIt != pn.end(); ++paramIt)
            createParamFloat<BoardParameterNumeric>(*paramIt, boardParameterNumericList);

        std::vector<BoardParameterOnOff> po = (*boardIt)->getBoardParameterOnOffs();

        for (std::vector<BoardParameterOnOff>::iterator paramIt = po.begin(); paramIt != po.end(); ++paramIt)
            createParamBinary<BoardParameterOnOff>(*paramIt, boardParameterOnOffList);

        std::vector<BoardParameterChStatus> pcs = (*boardIt)->getBoardParameterChStatuses();

        for (std::vector<BoardParameterChStatus>::iterator paramIt = pcs.begin(); paramIt != pcs.end(); ++paramIt)
            createParamMBinary<BoardParameterChStatus>(*paramIt, boardParameterChStatusList, recordFieldBdParamChStatus);

        std::vector<BoardParameterBdStatus> pbs = (*boardIt)->getBoardParameterBdStatuses();

        for (std::vector<BoardParameterBdStatus>::iterator paramIt = pbs.begin(); paramIt != pbs.end(); ++paramIt)
            createParamMBinary<BoardParameterBdStatus>(*paramIt, boardParameterBdStatusList, recordFieldBdParamBdStatus);

        std::vector<Channel> c = (*boardIt)->getChannels();

        for(std::vector<Channel>::iterator channelIt = c.begin(); channelIt != c.end(); ++channelIt)
        {
            std::vector<ChannelParameterNumeric> cpn = (*channelIt)->getChannelParameterNumerics();
            for (std::vector<ChannelParameterNumeric>::iterator paramIt = cpn.begin(); paramIt != cpn.end(); ++paramIt)
                createParamFloat<ChannelParameterNumeric>(*paramIt, channelParameterNumericList);

            std::vector<ChannelParameterOnOff> cpo = (*channelIt)->getChannelParameterOnOffs();
            for (std::vector<ChannelParameterOnOff>::iterator paramIt = cpo.begin(); paramIt != cpo.end(); ++paramIt)
                createParamBinary<ChannelParameterOnOff>(*paramIt, channelParameterOnOffList);

            std::vector<ChannelParameterChStatus> cpcs = (*channelIt)->getChannelParameterChStatuses();
            for (std::vector<ChannelParameterChStatus>::iterator paramIt = cpcs.begin(); paramIt != cpcs.end(); ++paramIt)
                createParamMBinary<ChannelParameterChStatus>(*paramIt, channelParameterChStatusList, recordFieldChParamChStatus);

            std::vector<ChannelParameterBinary> cpb = (*channelIt)->getChannelParameterBinaries();
            for (std::vector<ChannelParameterBinary>::iterator paramIt = cpb.begin(); paramIt != cpb.end(); ++paramIt)
                createParamInteger<ChannelParameterBinary>(*paramIt, channelParameterBinaryList);
        }
    }
}

////////////////////////////////////////////
// Methods overridden from asynPortDriver //
////////////////////////////////////////////
asynStatus CAENHVAsyn::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    static std::string method("readInt32");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, SystemPropertyInteger >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( spIt = systemPropertyIntegerList.find(function) ) != systemPropertyIntegerList.end() )
        {
            *value = spIt->second->getVal();
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::readInt32(pasynUser, value);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : read '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, *value);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while reading, status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    static std::string method("writeInt32");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, SystemPropertyInteger >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( spIt = systemPropertyIntegerList.find(function) ) != systemPropertyIntegerList.end() )
        {
            spIt->second->setVal(value);
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::writeInt32(pasynUser, value);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : set to '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while writting '%d', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    static std::string method("readFloat64");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, ChannelParameterNumeric >::iterator cpIt;
    std::map< int, BoardParameterNumeric   >::iterator bpIt;
    std::map< int, SystemPropertyFloat     >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( cpIt = channelParameterNumericList.find(function) ) != channelParameterNumericList.end() )
        {
            *value = cpIt->second->getVal();
            found = true;
        }
        else if ( ( bpIt = boardParameterNumericList.find(function) ) != boardParameterNumericList.end() )
        {
            *value = bpIt->second->getVal();
            found = true;
        }
        else if ( ( spIt = systemPropertyFloatList.find(function) ) != systemPropertyFloatList.end() )
        {
            *value = spIt->second->getVal();
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::readFloat64(pasynUser, value);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : read '%f'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, *value);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while reading, status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    static std::string method("writeFloat64");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, ChannelParameterNumeric >::iterator cpIt;
    std::map< int, BoardParameterNumeric   >::iterator bpIt;
    std::map< int, SystemPropertyFloat     >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( cpIt = channelParameterNumericList.find(function) ) != channelParameterNumericList.end() )
        {
            cpIt->second->setVal(value);
            found = true;
        }
        else if ( ( bpIt = boardParameterNumericList.find(function) ) != boardParameterNumericList.end() )
        {
            bpIt->second->setVal(value);
            found = true;
        }
        else if ( ( spIt = systemPropertyFloatList.find(function) ) != systemPropertyFloatList.end() )
        {
            spIt->second->setVal(value);
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::writeFloat64(pasynUser, value);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : set to '%f'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while writting '%f', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask)
{
    static std::string method("readUInt32Digital");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, BoardParameterOnOff      >::iterator bpoIt;
    std::map< int, BoardParameterChStatus   >::iterator bpcsIt;
    std::map< int, BoardParameterBdStatus   >::iterator bpbsIt;
    std::map< int, ChannelParameterOnOff    >::iterator cpoIt;
    std::map< int, ChannelParameterChStatus >::iterator cpcsIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( bpoIt = boardParameterOnOffList.find(function) ) != boardParameterOnOffList.end() )
        {
           uint32_t temp = bpoIt->second->getVal();
           temp &= mask;
           *value = temp;
           found = true;
        }
        else if ( ( bpcsIt = boardParameterChStatusList.find(function) ) != boardParameterChStatusList.end() )
        {
           uint32_t temp = bpcsIt->second->getVal();
           temp &= mask;
           *value = temp;
           found = true;
        }
        else if ( ( bpbsIt = boardParameterBdStatusList.find(function) ) != boardParameterBdStatusList.end() )
        {
           uint32_t temp = bpbsIt->second->getVal();
           temp &= mask;
           *value = temp;
           found = true;
        }
        else if ( ( cpoIt = channelParameterOnOffList.find(function) ) != channelParameterOnOffList.end() )
        {
           uint32_t temp = cpoIt->second->getVal();
           temp &= mask;
           *value = temp;
           found = true;
        }
        else if ( ( cpcsIt = channelParameterChStatusList.find(function) ) != channelParameterChStatusList.end() )
        {
           uint32_t temp = cpcsIt->second->getVal();
           temp &= mask;
           *value = temp;
           found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::readUInt32Digital(pasynUser, value, mask);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : read '%d', mask '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, *value, mask);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while reading, mask '%d', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, mask, status);

        return asynError;
    }
}


asynStatus CAENHVAsyn::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
    static std::string method("writeUInt32Digital");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    epicsUInt32 val(0);
    val &= ~mask;
    val |= value;

    // Iterators
    std::map< int, BoardParameterOnOff      >::iterator bpoIt;
    std::map< int, BoardParameterChStatus   >::iterator bpcsIt;
    std::map< int, BoardParameterBdStatus   >::iterator bpbsIt;
    std::map< int, ChannelParameterOnOff    >::iterator cpoIt;
    std::map< int, ChannelParameterChStatus >::iterator cpcsIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( bpoIt = boardParameterOnOffList.find(function) ) != boardParameterOnOffList.end() )
        {
            bpoIt->second->setVal(val);
            found = true;
        }
        else if ( ( bpcsIt = boardParameterChStatusList.find(function) ) != boardParameterChStatusList.end() )
        {
            bpcsIt->second->setVal(val);
            found = true;
        }
        else if ( ( bpbsIt = boardParameterBdStatusList.find(function) ) != boardParameterBdStatusList.end() )
        {
            bpbsIt->second->setVal(val);
            found = true;
        }
        else if ( ( cpoIt = channelParameterOnOffList.find(function) ) != channelParameterOnOffList.end() )
        {
            cpoIt->second->setVal(val);
            found = true;
        }
        else if ( ( cpcsIt = channelParameterChStatusList.find(function) ) != channelParameterChStatusList.end() )
        {
            cpcsIt->second->setVal(val);
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::writeUInt32Digital(pasynUser, value, mask);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : set to '%d', mask '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, mask);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while writting '%d', mask '%d', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, mask, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
{
    static std::string method("readOctet");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, SystemPropertyString >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( spIt = systemPropertyStringList.find(function) ) != systemPropertyStringList.end() )
        {
            std::string temp = spIt->second->getVal();
            strcpy(value, temp.c_str());
            *nActual = temp.length() + 1;
            found = true;
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::readOctet(pasynUser, value, maxChars, nActual, eomReason);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : read '%s', maxChars '%zu', nActual '%zu'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, maxChars, *nActual);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while reading, maxChars '%zu', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, maxChars, status);

        return asynError;
    }
}

asynStatus CAENHVAsyn::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
{
    static std::string method("writeOctet");
    int function(pasynUser->reason);
    int status(0);

    int addr;
    this->getAddress(pasynUser, &addr);

    const char *name;
    getParamName(addr, function, &name);

    // Iterators
    std::map< int, SystemPropertyString >::iterator spIt;

    // Check if the function is found in out lists
    bool found = false;

    // Look for the function number in the parameter lists
    try
    {
        if ( ( spIt = systemPropertyStringList.find(function) ) != systemPropertyStringList.end() )
        {
            found = true;
            std::string temp(value);
            spIt->second->setVal(temp);
            *nActual = temp.size();
        }
    }
    catch(std::runtime_error& e)
    {
        status = -1;
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : exception caught '%s'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, e.what());
    }

    // If the function was not found, fall back to the base method
    if (!found)
        status = asynPortDriver::writeOctet(pasynUser, value, maxChars, nActual);

    // Log status and return
    if (0 == status)
    {
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : set to '%s', maxChars '%zu', nActual '%zu'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, maxChars, *nActual);

        return asynSuccess;
    }
    else
    {
        asynPrint(pasynUser, ASYN_TRACE_ERROR, \
                    "Driver '%s', Port '%s', Method '%s', Function number '%d', parameter '%s' : Error while writting '%s', maxChars '%zu', status '%d'\n", \
                    this->driverName_.c_str(), this->portName_.c_str(), method.c_str(), function, name, value, maxChars, status);

        return asynError;
    }
}

////////////////////////////////////
// Driver configuration functions //
////////////////////////////////////

// + CAENHVAsynConfig //
extern "C" int CAENHVAsynConfig(const char* portName, int systemType, char* ipAddr, const char* userName, const char* password)
{
    new CAENHVAsyn(portName, systemType, ipAddr, userName, password);

    return asynSuccess;
}

static const iocshArg confArg0 = { "portName",   iocshArgString };
static const iocshArg confArg1 = { "systemType", iocshArgInt    };
static const iocshArg confArg2 = { "ipAddr",     iocshArgString };
static const iocshArg confArg3 = { "userName",   iocshArgString };
static const iocshArg confArg4 = { "password",   iocshArgString };

static const iocshArg * const confArgs[] =
{
    &confArg0,
    &confArg1,
    &confArg2,
    &confArg3,
    &confArg4
};

static const iocshFuncDef configFuncDef = {"CAENHVAsynConfig", 5, confArgs};

static void configCallFunc(const iocshArgBuf *args)
{
    CAENHVAsynConfig(args[0].sval, args[1].ival, args[2].sval, args[3].sval, args[4].sval);
}
// - CAENHVAsynConfig //

// + CAENHVAsynSetEpicsPrefix //
extern "C" int CAENHVAsynSetEpicsPrefix(const char *prefix)
{
    if ( ( ! prefix ) || ( prefix[0] == '\0' ) )
        return 0;

    CAENHVAsyn::epicsPrefix = prefix;

    return 0;
}

static const iocshArg epicsPrefixArg0 = { "Prefix", iocshArgString };

static const iocshArg * const epicsPrefixArgs[] =
{
    &epicsPrefixArg0
};

static const iocshFuncDef epicsPrefixFuncDef = { "CAENHVAsynSetEpicsPrefix", 1, epicsPrefixArgs };

static void epicsPrefixCallFunc(const iocshArgBuf *args)
{
    CAENHVAsynSetEpicsPrefix(args[0].sval);
}
// - CAENHVAsynSetEpicsPrefix //

// iocshRegister
void drvCAENHVAsynRegister(void)
{
    iocshRegister( &configFuncDef,      configCallFunc      );
    iocshRegister( &epicsPrefixFuncDef, epicsPrefixCallFunc );
}

extern "C"
{
    epicsExportRegistrar(drvCAENHVAsynRegister);
}
