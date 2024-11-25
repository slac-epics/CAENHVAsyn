/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : channel.cpp
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

#include "channel.h"

IChannel::IChannel(int h, std::size_t s, std::size_t c, bool readOnly)
:
    handle(h),
    slot(s),
    channel(c),
    readOnly(readOnly)
{
    GetChannelParams();
}

Channel IChannel::create(int h, std::size_t s, std::size_t c, bool readOnly)
{
    return std::make_shared<IChannel>(h, s, c, readOnly);
}

void IChannel::printInfo(std::ostream& stream) const
{
    stream << "      Slot = " << slot \
           << ", Channel = " << channel \
           << std::endl;

    stream << "        Number of Numeric parameters: " << channelParameterNumerics.size() << std::endl;
    for (std::vector<ChannelParameterNumeric>::const_iterator it = channelParameterNumerics.begin(); it != channelParameterNumerics.end(); ++it)
        (*it)->printInfo(stream);

    stream << "        Number of OnOff parameters: " << channelParameterOnOffs.size() << std::endl;
    for (std::vector<ChannelParameterOnOff>::const_iterator it = channelParameterOnOffs.begin(); it != channelParameterOnOffs.end(); ++it)
        (*it)->printInfo(stream);

    stream << "        Number of ChStatus parameters: " << channelParameterChStatuses.size() << std::endl;
    for (std::vector<ChannelParameterChStatus>::const_iterator it = channelParameterChStatuses.begin(); it != channelParameterChStatuses.end(); ++it)
        (*it)->printInfo(stream);

    stream << "        Number of Binary parameters: " << channelParameterBinaries.size() << std::endl;
    for (std::vector<ChannelParameterBinary>::const_iterator it = channelParameterBinaries.begin(); it != channelParameterBinaries.end(); ++it)
        (*it)->printInfo(stream);

}

void IChannel::GetChannelParams()
{
    // Get Channel Parameter Info
    std::string functionName("GetChannelParams");

    char *ParNameList = (char *)NULL;
    int ParNumber(0);
    CAENHVRESULT r = CAENHV_GetChParamInfo(handle, slot, channel, &ParNameList, &ParNumber);

    std::stringstream retMessage;
    retMessage << "CAENHV_GetChParamInfo (slot = " << slot << ") : " << CAENHV_GetError(handle) << " (num. " << r << ")";

    printMessage(functionName, retMessage.str().c_str());

    if ( r != CAENHV_OK )
        return;

    // Check if we the number of parameter is > 0
    if (ParNumber <= 0)
        return;

    // Create an unsigned version of the number of parameters
    std::size_t numParams(ParNumber);

    char (*p)[MAX_PARAM_NAME];
    p = (char (*)[MAX_PARAM_NAME])ParNameList;

    channelParameterNumerics.reserve(numParams);
    channelParameterOnOffs.reserve(numParams);
    for( std::size_t i(0) ; p[i][0] && i < numParams; i++ )
    {

        uint32_t type, mode;

        if ( CAENHV_GetChParamProp(handle, slot, channel, p[i], "Type", &type) != CAENHV_OK )
            throw std::runtime_error("CAENHV_GetChParamProp failed: " + std::string(CAENHV_GetError(handle)));

        if (CAENHV_GetChParamProp(handle, slot, channel, p[i], "Mode", &mode) != CAENHV_OK )
            throw std::runtime_error("CAENHV_GetChParamProp failed: " + std::string(CAENHV_GetError(handle)));

        if (readOnly) {
            if (mode == PARAM_MODE_RDWR) {
                mode = PARAM_MODE_RDONLY;
            }
            else if (mode == PARAM_MODE_WRONLY) {
                break;
            }
        }

        if (type == PARAM_TYPE_NUMERIC)
            channelParameterNumerics.push_back( IChannelParameterNumeric::create(handle, slot, channel, p[i], mode) );
        else if (type == PARAM_TYPE_ONOFF)
            channelParameterOnOffs.push_back( IChannelParameterOnOff::create(handle, slot, channel, p[i], mode) );
        else if (type == PARAM_TYPE_CHSTATUS)
            channelParameterChStatuses.push_back( IChannelParameterChStatus::create(handle, slot, channel, p[i], mode) );
        else if (type == PARAM_TYPE_BINARY)
            channelParameterBinaries.push_back( IChannelParameterBinary::create(handle, slot, channel, p[i], mode) );
        else
            //throw std::runtime_error("Parameter type not  supported!");
            std::cerr << "Error found when creating a Board Parameter object for pamater '" << p[i] << "'. Unsupported type = " << type << std::endl;
    }

    // Deallocate memory (Use RAII in the future for this)
    free(ParNameList);
}
