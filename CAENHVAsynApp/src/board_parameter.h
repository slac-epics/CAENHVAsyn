#ifndef BOARD_PARAMETER_H
#define BOARD_PARAMETER_H

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
#include <algorithm>
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

template<typename T>
class BoardParameterBase;

class IBoardParameterNumeric;
class IBoardParameterOnOff;
class IBoardParameterChStatus;
class IBoardParameterBdStatus;

// Shared pointer types
typedef std::shared_ptr< IBoardParameterNumeric  > BoardParameterNumeric;
typedef std::shared_ptr< IBoardParameterOnOff    > BoardParameterOnOff;
typedef std::shared_ptr< IBoardParameterChStatus > BoardParameterChStatus;
typedef std::shared_ptr< IBoardParameterBdStatus > BoardParameterBdStatus;

// Base class for all parameter types
template<typename T>
class BoardParameterBase
{
public:
    BoardParameterBase(int h, std::size_t s, const std::string&  p, uint32_t m);
    virtual ~BoardParameterBase() {};

    std::string getMode()            { return modeStr;         };
    std::string getEpicsParamName()  { return epicsParamName;  };
    std::string getEpicsRecordName() { return epicsRecordName; };
    std::string getEpicsDesc()       { return epicsDesc;       };

    virtual void printInfo(std::ostream& stream) const;

    virtual T    getVal()        const;
    virtual void setVal(T value) const;

protected:
    int         handle;
    std::size_t slot;
    std::string param;
    uint32_t    mode;
    std::string modeStr;
    std::string epicsParamName;
    std::string epicsRecordName;
    std::string epicsDesc;
};

// Class for Numeric parameters
class IBoardParameterNumeric : public BoardParameterBase<float>
{
public:
    IBoardParameterNumeric(int h, std::size_t s, const std::string&  p, uint32_t m);
    ~IBoardParameterNumeric() {};

    // Factory method
    static BoardParameterNumeric create(int h, std::size_t s, const std::string&  p, uint32_t m);

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
class  IBoardParameterOnOff : public BoardParameterBase<uint32_t>
{
public:
    IBoardParameterOnOff(int h, std::size_t s, const std::string&  p, uint32_t m);
    ~IBoardParameterOnOff() {};

    // Factory method
    static BoardParameterOnOff create(int h, std::size_t s, const std::string&  p, uint32_t m);

    const std::string& getOnState()  const { return onState;  };
    const std::string& getOffState() const { return offState; };

    virtual void printInfo(std::ostream& stream) const;

private:
    std::string onState;
    std::string offState;
};

// Class for ChStatus parameters
class IBoardParameterChStatus : public BoardParameterBase<uint32_t>
{
public:
    IBoardParameterChStatus(int h, std::size_t s, const std::string&  p, uint32_t m);
    virtual ~IBoardParameterChStatus() {};

    // Factory method
    static BoardParameterChStatus create(int h, std::size_t s, const std::string&  p, uint32_t m);
};

// Class for BdStatus parameters
class IBoardParameterBdStatus : public BoardParameterBase<uint32_t>
{
public:
    IBoardParameterBdStatus(int h, std::size_t s, const std::string&  p, uint32_t m);
    virtual ~IBoardParameterBdStatus() {};

    // Factory method
    static BoardParameterBdStatus create(int h, std::size_t s, const std::string&  p, uint32_t m);
};

#endif
