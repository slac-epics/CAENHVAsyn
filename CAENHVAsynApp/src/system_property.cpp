/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : system_property.cpp
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

#include "system_property.h"

SystemPropertyBase::SystemPropertyBase(int h, const std::string&  p, uint32_t m)
:
    handle(h),
    prop(p),
    mode(m)
{
    // Generate mode string
    modeStr = processMode(mode);

    std::stringstream temp;

    // Generate the EPICS parameter name
    temp.str("");
    temp << "C_" << processParamName(prop);
    epicsParamName = temp.str();

    // Generate the EPICS record name
    temp.str("");
    temp << "C:" << processParamName(prop);
    epicsRecordName = temp.str();

    // Generate the EPICS description
    temp.str("");
    temp << "'Chassis, " << prop << " (" << modeStr << ")'";
    epicsDesc = temp.str();

};

void SystemPropertyBase::printInfo(std::ostream& stream) const
{
    stream << "      Name = "   << prop \
           << ", Handle = "     << handle \
           << ", Mode = "       << modeStr \
           << ", epicsParamName = " << epicsParamName \
           << ", epicsRecordName = " << epicsRecordName \
           << std::endl;
}

// String class
SystemPropertyString ISystemPropertyString::create(int h, const std::string&  p, uint32_t m)
{
    return std::make_shared<ISystemPropertyString>(h, p, m);
}

ISystemPropertyString::ISystemPropertyString(int h, const std::string&  p, uint32_t m)
:
    SystemPropertyBase(h,p,m)
{
}

std::string ISystemPropertyString::getVal() const
{

    if (mode == SYSPROP_MODE_WRONLY)
        return "";

    char temp[4096];

    CAENHVRESULT r = CAENHV_GetSysProp(handle, prop.c_str(), temp);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_GetSysProp failed: " + std::string(CAENHV_GetError(handle)));

    return temp;
}

void ISystemPropertyString::setVal(const std::string& v) const
{

    if (mode == SYSPROP_MODE_RDONLY)
        return;

    char* temp = new char[v.size() + 1];
    strcpy(temp, v.c_str());

    CAENHVRESULT r = CAENHV_SetSysProp(handle, prop.c_str(), temp);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_SetSysProp failed: " + std::string(CAENHV_GetError(handle)));

    delete[] temp;
}

// Float class
SystemPropertyFloat ISystemPropertyFloat::create(int h, const std::string&  p, uint32_t m)
{
    return std::make_shared<ISystemPropertyFloat>(h, p, m);
}

ISystemPropertyFloat::ISystemPropertyFloat(int h, const std::string&  p, uint32_t m)
:
    SystemPropertyBase(h,p,m)
{
}

float ISystemPropertyFloat::getVal() const
{
    if (mode == SYSPROP_MODE_WRONLY)
        return 0.0;

    float temp;

    CAENHVRESULT r = CAENHV_GetSysProp(handle, prop.c_str(), &temp);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_GetSysProp failed: " + std::string(CAENHV_GetError(handle)));

    return temp;
}

void ISystemPropertyFloat::setVal(float v) const
{
    if (mode == SYSPROP_MODE_RDONLY)
        return;

    CAENHVRESULT r = CAENHV_SetSysProp(handle, prop.c_str(), &v);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_SetSysProp failed: " + std::string(CAENHV_GetError(handle)));
}

// Integer class template
template<typename T>
std::shared_ptr< ISystemPropertyIntegerTemplate<T> > ISystemPropertyIntegerTemplate<T>::create(int h, const std::string&  p, uint32_t m)
{
    return std::make_shared<ISystemPropertyIntegerTemplate>(h, p, m);
}

template<typename T>
int32_t ISystemPropertyIntegerTemplate<T>::getVal() const
{
    if (mode == SYSPROP_MODE_WRONLY)
        return 0;

    T temp;

    CAENHVRESULT r = CAENHV_GetSysProp(handle, prop.c_str(), &temp);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_GetSysProp failed: " + std::string(CAENHV_GetError(handle)));

    return static_cast<int32_t>(temp);
}

template<typename T>
void ISystemPropertyIntegerTemplate<T>::setVal(int32_t value) const
{
    if (mode == SYSPROP_MODE_RDONLY)
        return;

    T temp = static_cast<T>(value);
    CAENHVRESULT r = CAENHV_SetSysProp(handle, prop.c_str(), &temp);

    if ( r != CAENHV_OK && r != CAENHV_GETPROPNOTIMPL && r != CAENHV_NOTGETPROP )
        throw std::runtime_error("CAENHV_SetSysProp failed: " + std::string(CAENHV_GetError(handle)));
}

template class ISystemPropertyIntegerTemplate<uint32_t>;
template class ISystemPropertyIntegerTemplate<uint16_t>;
template class ISystemPropertyIntegerTemplate<int32_t>;
template class ISystemPropertyIntegerTemplate<int16_t>;
template class ISystemPropertyIntegerTemplate<uint8_t>;
