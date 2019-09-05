#ifndef CHANNEL_PARAMETER_H
#define CHANNEL_PARAMETER_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : board_parameter.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-08-20
 * ----------------------------------------------------------------------------
 * Description:
 * CAEN HV Power supplies Board Parameter Class
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
#include <memory>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <arpa/inet.h>
#include <iostream>

#include "CAENHVWrapper.h"
#include "common.h"

#include "board_parameter.h"


class IChannelParameterNumeric;
class IChannelParameterOnOff;

// Shared pointer types
typedef std::shared_ptr< IChannelParameterNumeric > ChannelParameterNumeric;
typedef std::shared_ptr< IChannelParameterOnOff   > ChannelParameterOnOff;


class ChannelParameterBase
{
public: 
    ChannelParameterBase(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    virtual ~ChannelParameterBase() {};

    virtual void printInfo() const = 0;

    std::string getMode()            { return modeStr;    }; 
    std::string getEpicsParamName()  { return epicsParamName;  };
    std::string getEpicsRecordName() { return epicsRecordName; };
    std::string getEpicsDesc()       { return epicsDesc;       };

protected:
    int         handle;
    std::size_t slot;
    std::size_t channel;
    std::string param;
    uint32_t    mode;
    std::string type;
    std::string modeStr;
    std::string epicsParamName;
    std::string epicsRecordName;
    std::string epicsDesc;
};

class IChannelParameterNumeric : public ChannelParameterBase
{
public:
    IChannelParameterNumeric(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m); 
    ~IChannelParameterNumeric() {};

    // Factory method
    static ChannelParameterNumeric create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    float       getMinVal() const { return minVal; };
    float       getMaxVal() const { return maxVal; };
    std::string getUnits()  const { return units;  };

    virtual void printInfo() const;

    float getVal() const;
    void setVal(float v);

private:
    float       minVal;
    float       maxVal;
    std::string units;
};

class IChannelParameterOnOff : public ChannelParameterBase
{
public:
    IChannelParameterOnOff(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterOnOff() {};

    // Factory method
    static ChannelParameterOnOff create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    std::string getOnState()  const { return onState;  }; 
    std::string getOffState() const { return offState; }; 

    virtual void printInfo() const;

    std::string getVal() const;
    void setVal(const std::string& v);

private:
    std::string onState;
    std::string offState;
};

#endif
