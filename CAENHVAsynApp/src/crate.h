#ifndef CRATE_H
#define CRATE_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : crate.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-08-20
 * ----------------------------------------------------------------------------
 * Description:
 * CAEN HV Power Supplies Crate Class
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
#include <vector>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <arpa/inet.h>
#include <iostream>

#include "CAENHVWrapper.h"
#include "common.h"
#include "board.h"
#include "system_property.h"

class SysProp;
template<typename T>
class SysPropT;

class ICrate;

typedef std::shared_ptr<ICrate> Crate;

class ICrate
{
public:
    ICrate(int systemType, const std::string& ipAddr, const std::string& userName, const std::string& password);
    ~ICrate();

    // Factory method
    static Crate create(int systemType, const std::string& ipAddr, const std::string& userName, const std::string& password);

    void printInfo(std::ostream& stream) const;
    void printCrateMap(std::ostream& stream) const;
    void ReinitSystem();

    std::vector<SystemPropertyInteger> getSystemPropertyIntegers() { return systemPropertyIntegers; };
    std::vector<SystemPropertyFloat>   getSystemPropertyFloats()   { return systemPropertyFloats;   };
    std::vector<SystemPropertyString>  getSystemPropertyStrings()  { return systemPropertyStrings;  };

    std::vector<Board> getBoards() { return boards; };

private:

    int  InitSystem() const;
    void GetPropList();
    void GetCrateMap();

    template <typename T>
    void printProperties(std::ostream& stream, const std::string& type, const T& pv) const;

    int handle;
    int systemType_;
    std::string ipAddr_, userName_, password_;

    // Number of slot in the crate
    std::size_t numSlots;

    // Slots in the crate
    std::vector<Board> boards;

    // Crate properties
    std::vector<SystemPropertyInteger> systemPropertyIntegers;
    std::vector<SystemPropertyFloat>   systemPropertyFloats;
    std::vector<SystemPropertyString>  systemPropertyStrings;
};

#endif
