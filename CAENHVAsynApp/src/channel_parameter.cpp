/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : board_parameter.cpp
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-08-30
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

#include "channel_parameter.h"

// Base class for all parameter types
template<typename T>
ChannelParameterBase<T>::ChannelParameterBase(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
:
    handle(h),
    slot(s),
    channel(c),
    param(p),
    mode(m)
{
    // Generate mode string representation
    modeStr = processMode(mode);

    std::stringstream temp;

    // Generate the EPICS parameter name
    temp.str("");
    temp << "S" << std::setfill('0') << std::setw(2) << s << "_" \
         << "C" << std::setfill('0') << std::setw(2) << channel << "_" \
         << processParamName(param);
    epicsParamName = temp.str();

    // Generate the EPICS record name
    temp.str("");
    temp << "S" << std::setfill('0') << std::setw(2) << s << ":" \
         << "C" << std::setfill('0') << std::setw(2) << channel << ":" \
         << processParamName(param);
    epicsRecordName = temp.str();

    // Generate the EPICS description
    temp.str("");
    temp << "'Slot " << s \
         << ", Ch " << channel \
         <<  ", " << param \
         << " (" << modeStr << ")'";
    epicsDesc = temp.str();
}

template<typename T>
T ChannelParameterBase<T>::getVal() const
{
    if (mode == PARAM_MODE_WRONLY)
        return T();

    T temp;

    uint16_t temp_chs = channel;
    if ( CAENHV_GetChParam(handle, slot, param.c_str(), 1, &temp_chs, &temp) != CAENHV_OK )
           throw std::runtime_error("CAENHV_GetChParam failed: " + std::string(CAENHV_GetError(handle)));

    return temp;
}

template<typename T>
void ChannelParameterBase<T>::setVal(T value) const
{
    if (mode == PARAM_MODE_RDONLY)
        return;

    uint16_t temp_chs = channel;
    if ( CAENHV_SetChParam(handle, slot, param.c_str(), 1, &temp_chs, &value) != CAENHV_OK )
           throw std::runtime_error("CAENHV_SetChParam failed: " + std::string(CAENHV_GetError(handle)));
}

template<typename T>
void ChannelParameterBase<T>::printInfo(std::ostream& stream) const
{
    stream << "          Param = "   << param \
           << ", Mode  = "           << modeStr \
           << ", Value = "           << getVal() \
           << ", epicsParamName = "  << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// Class for Numeric parameters
ChannelParameterNumeric IChannelParameterNumeric::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelParameterNumeric>(h, s, c, p, m);
}

IChannelParameterNumeric::IChannelParameterNumeric(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
:
    ChannelParameterBase<float>(h, s, c, p, m)
{
   float temp;

   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Minval", &temp ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

   minVal = temp;

   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Maxval", &temp ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

   maxVal = temp;

   // Extract uints
   uint16_t u;
   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Unit", &u ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

   int8_t e;
   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Exp", &e ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

     units = processUnits(u, e);
}

void IChannelParameterNumeric::printInfo(std::ostream& stream) const
{
    stream << "          Param = "   << param \
           << ", Mode  = "           << modeStr \
           << ", Minval = "          << getMinVal() \
           << ", Maxval = "          <<  getMaxVal() \
           << ", Units = "           << units.c_str() \
           << ", Value = "           << getVal() \
           << ", epicsParamName = "  << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// Class for OnOff parameters
ChannelParameterOnOff IChannelParameterOnOff::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelParameterOnOff>(h, s, c, p, m);
}

IChannelParameterOnOff::IChannelParameterOnOff(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
:
    ChannelParameterBase<uint32_t>(h, s, c, p, m)
{
   char temp[30];

   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Onstate", temp ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetChParamProp failed: " + std::string(CAENHV_GetError(handle)));

   onState = temp;

   if ( CAENHV_GetChParamProp(handle, slot, channel, param.c_str(), "Offstate", temp ) != CAENHV_OK )
       throw std::runtime_error("CAENHV_GetChParamProp failed: " + std::string(CAENHV_GetError(handle)));

    offState = temp;

}

void IChannelParameterOnOff::printInfo(std::ostream& stream) const
{
    stream << "          Param = "   << param \
           << ", Mode = "            << modeStr \
           << ", On state = "        << getOnState() \
           << ", Off state = "       << getOffState() \
           << ", Value = "           << getVal() \
           << ", epicsParamName = "  << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// Class for ChStatus parameters
IChannelParameterChStatus::IChannelParameterChStatus(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
:
    ChannelParameterBase<uint32_t>(h, s, c, p, m)
{
}

ChannelParameterChStatus IChannelParameterChStatus::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelParameterChStatus>(h, s, c, p, m);
}

void IChannelParameterChStatus::printInfo(std::ostream& stream) const
{
    stream << "          Param = "   << param \
           << ", Mode  = "           << modeStr \
           << ", Value = "           << getVal() \
           << ", epicsParamName = "  << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// Class for Binary parameters
IChannelParameterBinary::IChannelParameterBinary(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
:
    ChannelParameterBase<int32_t>(h, s, c, p, m)
{
}

ChannelParameterBinary IChannelParameterBinary::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelParameterBinary>(h, s, c, p, m);
}

void IChannelParameterBinary::printInfo(std::ostream& stream) const
{
    stream << "          Param = "   << param \
           << ", Mode  = "           << modeStr \
           << ", Value = "           << getVal() \
           << ", epicsParamName = "  << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// Class for String parameters
IChannelParameterString::IChannelParameterString(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
    :
    ChannelParameterBase<std::string>(h, s, c, p, m)
{
}

ChannelParameterString IChannelParameterString::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelParameterString>(h, s, c, p, m);
}

// Class for Channel Name
IChannelName::IChannelName(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
    :
    IChannelParameterString(h, s, c, p, m)
{
}

ChannelName IChannelName::create(int h, std::size_t s, std::size_t c, const std::string&  p, uint32_t m)
{
    return std::make_shared<IChannelName>(h, s, c, p, m);
}

std::string IChannelName::getVal() const
{
    char name_char[MAX_CH_NAME];

    uint16_t tempChan = channel;
    if (CAENHV_GetChName(handle, slot, 1, &tempChan, &name_char) != CAENHV_OK)
        throw std::runtime_error("CAENHV_GetChName failed: " + std::string(CAENHV_GetError(handle)));

    return std::string(name_char);
}

void IChannelName::setVal(std::string name) const
{
    uint16_t tempChan = channel;
    if (CAENHV_SetChName(handle, slot, 1, &tempChan, name.c_str()) != CAENHV_OK)
        throw std::runtime_error("CAENHV_SetChName failed: " + std::string(CAENHV_GetError(handle)));
}