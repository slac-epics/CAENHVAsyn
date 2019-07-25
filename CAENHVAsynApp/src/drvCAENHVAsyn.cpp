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

//const std::string CAENHVAsyn::DriverName_ = "CAENHVAsyn";

CAENHVAsyn::CAENHVAsyn(const char* portName, char* ipAddr, const char* userName, const char* password)
:
    asynPortDriver(
        portName,
        MAX_SIGNALS,
        NUM_PARAMS,
        asynInt32Mask | asynDrvUserMask | asynInt16ArrayMask | asynInt32ArrayMask | asynOctetMask | \
        asynFloat64ArrayMask | asynUInt32DigitalMask | asynFloat64Mask,                             // Interface Mask
        asynInt16ArrayMask | asynInt32ArrayMask | asynInt32Mask | asynUInt32DigitalMask,            // Interrupt Mask
        ASYN_MULTIDEVICE | ASYN_CANBLOCK,                                                           // asynFlags
        1,                                                                                          // Autoconnect
        0,                                                                                          // Default priority
        0),                                                                                         // Default stack size
    DriverName_("CAENHVAsyn"),
    portName_(portName),
    handler(-1)
{
    std::cout << "Initiliziting " << DriverName_ << "..." << std::endl;

    // CAENHV_InitSystem
    std::cout << std::endl;
    std::cout << "Calling CAENHV_InitSystem..." << std::endl;
    CAENHVRESULT ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)3, LINKTYPE_TCPIP, static_cast<void*>(ipAddr), userName, password, &handler);

    std::stringstream retMessage; 
    retMessage << "CAENHV_InitSystem: " << CAENHV_GetError(handler) << " (num. " << ret << ")";

    if( ret != CAENHV_OK )
        throw std::runtime_error(retMessage.str().c_str());

    std::cout << "Success!" << std::endl;
    std::cout << retMessage.str() << std::endl;

    // Get Crate Map
    std::cout << std::endl;
    std::cout << "Calling CAENHV_GetCrateMap...." << std::endl;
    unsigned short NrOfSlot;
    unsigned short *NrOfChList;
    char *ModelList;
    char *DescriptionList;
    unsigned short *SerNumList;
    unsigned char *FmwRelMinList;
    unsigned char *FmwRelMaxList;
    ret = CAENHV_GetCrateMap(handler, &NrOfSlot, &NrOfChList, &ModelList, &DescriptionList, &SerNumList, &FmwRelMinList, &FmwRelMaxList);

    retMessage.str("");
    retMessage << "CAENHV_GetCrateMap: " << CAENHV_GetError(handler) << " (num. " << ret << ")";

    if( ret != CAENHV_OK )
        throw std::runtime_error(retMessage.str().c_str());

    std::cout << "Success!" << std::endl;
    std::cout << retMessage.str() << std::endl;

    std::cout << "Create Map:" << std::endl;
    char *m = ModelList, *d = ModelList;
    for (std::size_t i(0); i < NrOfSlot; ++i, m += strlen(m) + 1, d += strlen(d) + 1)
    {
        std::cout << "Board " << std::setfill('0')  << std::setw(2) << i << ": ";
        if ( *m == '\0' )
            std::cout << "Not present." << std::endl;
        else
            std::cout << "Model: " << m << ", Description: " <<  d << ", Number of channels: " << unsigned(NrOfChList[i]) << ", Serial Number: " << unsigned(SerNumList[i]) << ", Firmware release: " << unsigned(FmwRelMaxList[i]) << "." << unsigned(FmwRelMinList[i]) << std::endl;
    }

    // Deallocate memory (Use RAII in the future for this)
    free(NrOfChList);
    free(ModelList);
    free(DescriptionList);
    free(SerNumList);
    free(FmwRelMinList);
    free(FmwRelMaxList);

    // Done
    std::cout << std::endl;
    std::cout << "Done initilizing " << DriverName_ << std::endl;
}
////////////////////////////////////
// Driver configuration functions //
////////////////////////////////////

// CAENHVAsynConfig
extern "C" int CAENHVAsynConfig(const char* portName, char* ipAddr, const char* userName, const char* password)
{
    int status = 0;
    
    // Check parameters 
    
    new CAENHVAsyn(portName, ipAddr, userName, password);

    return (status==0) ? asynSuccess : asynError;
}

static const iocshArg confArg0 =    { "portName",  iocshArgString };
static const iocshArg confArg1 =    { "ipAddr",    iocshArgString };
static const iocshArg confArg2 =    { "userName",  iocshArgString };
static const iocshArg confArg3 =    { "password",  iocshArgString};

static const iocshArg * const confArgs[] =
{
    &confArg0,
    &confArg1,
    &confArg2,
    &confArg3
};

static const iocshFuncDef configFuncDef = {"CAENHVAsynConfig", 4, confArgs};

static void configCallFunc(const iocshArgBuf *args)
{
    CAENHVAsynConfig(args[0].sval, args[1].sval, args[2].sval, args[3].sval);
}

// iocshRegister
void drvCAENHVAsynRegister(void)
{
    iocshRegister(&configFuncDef, configCallFunc);
}

extern "C" 
{
    epicsExportRegistrar(drvCAENHVAsynRegister);
}
