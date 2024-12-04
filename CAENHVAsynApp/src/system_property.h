#ifndef SYSTEM_PROPERTY_H
#define SYSTEM_PROPERTY_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : system_property.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-09-04
 * ----------------------------------------------------------------------------
 * Description:
 * CAEN HV Power supplies System Property Class
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

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#include <iostream>

#include "CAENHVWrapper.h"
#include "common.h"

class ISystemPropertyInteger;
class ISystemPropertyFloat;
class ISystemPropertyString;

typedef std::shared_ptr< ISystemPropertyInteger > SystemPropertyInteger;
typedef std::shared_ptr< ISystemPropertyFloat   > SystemPropertyFloat;
typedef std::shared_ptr< ISystemPropertyString  > SystemPropertyString;

class SystemPropertyBase
{
public:
    SystemPropertyBase(int h, const std::string&  p, uint32_t m);
    virtual ~SystemPropertyBase() {};

    std::string getMode()            { return modeStr;    };
    std::string getEpicsParamName()  { return epicsParamName;  };
    std::string getEpicsRecordName() { return epicsRecordName; };
    std::string getEpicsDesc()       { return epicsDesc;       };

    void printInfo(std::ostream& stream) const;

protected:
    int         handle;
    std::string prop;
    uint32_t    mode;
    std::string modeStr;
    std::string epicsParamName;
    std::string epicsRecordName;
    std::string epicsDesc;
};

class ISystemPropertyString : public SystemPropertyBase
{
public:
    ISystemPropertyString(int h, const std::string&  p, uint32_t m);
    ~ISystemPropertyString() {};

    // Factory method
    static SystemPropertyString create(int h, const std::string&  p, uint32_t m);

    std::string getVal()                     const;
    void        setVal(const std::string& v) const;
};

class ISystemPropertyFloat : public SystemPropertyBase
{
public:
    ISystemPropertyFloat(int h, const std::string&  p, uint32_t m);
    ~ISystemPropertyFloat() {};

    // Factory method
    static SystemPropertyFloat create(int h, const std::string&  p, uint32_t m);

    float getVal()        const;
    void  setVal(float v) const;
};

// Base clase for integer parameters
class ISystemPropertyInteger : public SystemPropertyBase
{
public:
    ISystemPropertyInteger(int h, const std::string&  p, uint32_t m)  : SystemPropertyBase(h,p,m) {};
    virtual ~ISystemPropertyInteger() {};

    virtual int32_t getVal()              const = 0;
    virtual void    setVal(int32_t value) const = 0;
};

// Integer parameter class template
template<typename T>
class ISystemPropertyIntegerTemplate : public ISystemPropertyInteger
{
public:
    ISystemPropertyIntegerTemplate(int h, const std::string&  p, uint32_t m) : ISystemPropertyInteger(h,p,m) {};
    virtual ~ISystemPropertyIntegerTemplate() {};

    // Factory method
    static std::shared_ptr< ISystemPropertyIntegerTemplate > create(int h, const std::string&  p, uint32_t m);

    virtual int32_t getVal()              const;
    virtual void    setVal(int32_t value) const;
};

#endif
