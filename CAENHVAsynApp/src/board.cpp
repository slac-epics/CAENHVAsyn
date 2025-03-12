/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : board.cpp
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-08-30
 * ----------------------------------------------------------------------------
 * Description:
 * CAEN HV Power supplies Board Class
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

#include "board.h"

IBoard::IBoard(int h, std::size_t s, std::string m, std::string d, std::size_t n, std::string sn, std::string fw, bool readOnly)
:
    handle(h),
    slot(s),
    model(m),
    description(d),
    numChannels(n),
    serialNumber(sn),
    firmwareRelease(fw),
    readOnly(readOnly)
{
    GetBoardParams();
    GetBoardChannels();
}

IBoard::~IBoard()
{
}

Board IBoard::create(int h, std::size_t s, std::string m, std::string d, std::size_t n, std::string sn, std::string fw, bool readOnly)
{
    return std::make_shared<IBoard>(h, s, m, d, n, sn, fw, readOnly);
}

void IBoard::printInfo(std::ostream& stream) const
{
    printBoardInfo(stream);

    stream << "    Board parameters:" << std::endl;
    stream << "    ..........................." << std::endl;

    stream << "      Number of Numeric parameters: " << boardParameterNumerics.size() << std::endl;
    for (std::vector<BoardParameterNumeric>::const_iterator it = boardParameterNumerics.begin(); it != boardParameterNumerics.end(); ++it)
        (*it)->printInfo(stream);

    stream << "      Number of OnOff parameters: " << boardParameterOnOffs.size() << std::endl;
    for (std::vector<BoardParameterOnOff>::const_iterator it = boardParameterOnOffs.begin(); it != boardParameterOnOffs.end(); ++it)
        (*it)->printInfo(stream);

    stream << "      Number of ChStatus parameters: " << boardParameterChStatuses.size() << std::endl;
    for (std::vector<BoardParameterChStatus>::const_iterator it = boardParameterChStatuses.begin(); it != boardParameterChStatuses.end(); ++it)
        (*it)->printInfo(stream);

    stream << "      Number of BdStatus parameters: " << boardParameterBdStatuses.size() << std::endl;
    for (std::vector<BoardParameterBdStatus>::const_iterator it = boardParameterBdStatuses.begin(); it != boardParameterBdStatuses.end(); ++it)
        (*it)->printInfo(stream);

    stream << "    Channel parameters:" << std::endl;
    stream << "    ..........................." << std::endl;
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        (*it)->printInfo(stream);
}

void IBoard::printBoardInfo(std::ostream& stream) const
{
    stream << "    Slot: " << slot \
           << ", Model: " << model \
           << ", Description: " <<  description \
           << ", Number of channels: " << numChannels \
           << ", Serial Number: " << serialNumber \
           << ", Firmware release: " << firmwareRelease \
           << std::endl;
}

void IBoard::GetBoardParams()
{
    // Get Board Parameter Info
    std::string functionName("GetBoardParams");

    char *ParNameList = (char *)NULL;
    CAENHVRESULT r = CAENHV_GetBdParamInfo(handle, slot, &ParNameList);

    std::stringstream retMessage;
    retMessage << "CAENHV_GetBdParamInfo (slot = " << slot << ") : " << CAENHV_GetError(handle) << " (num. " << r << ")";

    printMessage(functionName, retMessage.str().c_str());

    if ( r != CAENHV_OK )
        return;

    char (*p)[MAX_PARAM_NAME];
    p = (char (*)[MAX_PARAM_NAME])ParNameList;

    boardParameterNumerics.reserve(MAX_PARAM_NAME);
    boardParameterOnOffs.reserve(MAX_PARAM_NAME);

    for (std::size_t i(0); p[i][0]; ++i)
    {
        uint32_t type, mode;

        if ( CAENHV_GetBdParamProp(handle, slot, p[i], "Type", &type) != CAENHV_OK )
            throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

        if (CAENHV_GetBdParamProp(handle, slot, p[i], "Mode", &mode) != CAENHV_OK )
            throw std::runtime_error("CAENHV_GetBdParamProp failed: " + std::string(CAENHV_GetError(handle)));

        if (readOnly) {
            if (mode == PARAM_MODE_RDWR) {
                mode = PARAM_MODE_RDONLY;
            } else if (mode == PARAM_MODE_WRONLY) {
                break;
            }
        }

        if (type == PARAM_TYPE_NUMERIC)
            boardParameterNumerics.push_back( IBoardParameterNumeric::create(handle, slot, p[i], mode));
        else if (type == PARAM_TYPE_ONOFF)
            boardParameterOnOffs.push_back( IBoardParameterOnOff::create(handle, slot, p[i], mode));
        else if (type == PARAM_TYPE_CHSTATUS)
            boardParameterChStatuses.push_back( IBoardParameterChStatus::create(handle, slot, p[i], mode));
        else if (type == PARAM_TYPE_BDSTATUS)
            boardParameterBdStatuses.push_back( IBoardParameterBdStatus::create(handle, slot, p[i], mode));
        else
            //throw std::runtime_error("Parameter type not  supported!");
            std::cerr << "Error found when creating a Board Parameter object for pamater '" << p[i] << "'. Unsupported type = " << type << std::endl;
    }

    // Deallocate memory (Use RAII in the future for this)
    free(ParNameList);
}

void IBoard::GetBoardChannels()
{
    for (std::size_t i(0); i < numChannels; ++i)
        channels.push_back( IChannel::create(handle, slot, i, readOnly) );
}
