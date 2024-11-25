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
class IChannelParameterChStatus;
class IChannelParameterBinary;
class IChannelParameterString;
class IChannelName;

// Shared pointer types
typedef std::shared_ptr< IChannelParameterNumeric  > ChannelParameterNumeric;
typedef std::shared_ptr< IChannelParameterOnOff    > ChannelParameterOnOff;
typedef std::shared_ptr< IChannelParameterChStatus > ChannelParameterChStatus;
typedef std::shared_ptr< IChannelParameterBinary   > ChannelParameterBinary;
typedef std::shared_ptr< IChannelParameterString   > ChannelParameterString;
typedef std::shared_ptr< IChannelName              > ChannelName;

template<typename T>
class ChannelParameterBase
{
public:
    ChannelParameterBase(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    virtual ~ChannelParameterBase() {};

    std::string getMode()            { return modeStr;    };
    std::string getEpicsParamName()  { return epicsParamName;  };
    std::string getEpicsRecordName() { return epicsRecordName; };
    std::string getEpicsDesc()       { return epicsDesc;       };

    virtual void printInfo(std::ostream& stream) const;

    virtual T    getVal()        const;
    virtual void setVal(T value) const;

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

// Class for Numeric parameters
class IChannelParameterNumeric : public ChannelParameterBase<float>
{
public:
    IChannelParameterNumeric(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterNumeric() {};

    // Factory method
    static ChannelParameterNumeric create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    float       getMinVal() const { return minVal; };
    float       getMaxVal() const { return maxVal; };
    std::string getUnits()  const { return units;  };

    virtual void printInfo(std::ostream& stream) const;

private:
    float       minVal;
    float       maxVal;
    std::string units;
};

// Class for OnOff parameters
class IChannelParameterOnOff : public ChannelParameterBase<uint32_t>
{
public:
    IChannelParameterOnOff(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterOnOff() {};

    // Factory method
    static ChannelParameterOnOff create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    std::string getOnState()  const { return onState;  };
    std::string getOffState() const { return offState; };

    virtual void printInfo(std::ostream& stream) const;

private:
    std::string onState;
    std::string offState;
};

// Class for ChStatus parameters
class IChannelParameterChStatus : public ChannelParameterBase<uint32_t>
{
public:
    IChannelParameterChStatus(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterChStatus() {};

    // Factory method
    static ChannelParameterChStatus create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    virtual void printInfo(std::ostream& stream) const;
};

// Class for Binary parameters
class IChannelParameterBinary : public ChannelParameterBase<int32_t>
{
public:
    IChannelParameterBinary(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterBinary() {};

    // Factory method
    static ChannelParameterBinary create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    virtual void printInfo(std::ostream& stream) const;
};

// Class for String Parameters
class IChannelParameterString : public ChannelParameterBase<std::string>
{
public:
    IChannelParameterString(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelParameterString() {};

    // Factory method
    static ChannelParameterString create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
};

// Class for Channel name (not really a parameter as CAEN defines it)
class IChannelName : public IChannelParameterString
{
public:
    IChannelName(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);
    ~IChannelName() {};

    // Factory method
    static ChannelName create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m);

    virtual std::string    getVal()        const;
    virtual void setVal(std::string value) const;
};

#endif
