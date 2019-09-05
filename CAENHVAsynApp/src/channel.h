#ifndef CHANNEL_H
#define CHANNEL_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : channel.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-09-04
 * ----------------------------------------------------------------------------
 * Description:
 * CAEN HV Power supplies Channel Class
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
#include "channel_parameter.h"

class Channel
{
public:
    Channel(int h, std::size_t s, std::size_t c);
    ~Channel() {};

    void printInfo() const;

    std::vector<ChannelParameterNumeric> getBoardParameterNumerics() { return channelParameterNumerics; };
    std::vector<ChannelParameterOnOff>   getBoardParameterOnOffs()   { return channelParameterOnOffs; };

private:

    void GetChannelParams();

    int                         handle;
    std::size_t                 slot;
    std::size_t                 channel;

    std::vector<ChannelParameterNumeric> channelParameterNumerics;
    std::vector<ChannelParameterOnOff>   channelParameterOnOffs;

};

#endif
