#ifndef BOARD_H
#define BOARD_H

/**
 *-----------------------------------------------------------------------------
 * Title      : CAEN HV Asyn module
 * ----------------------------------------------------------------------------
 * File       : board.h
 * Author     : Jesus Vasquez, jvasquez@slac.stanford.edu
 * Created    : 2019-08-20
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
#include "board_parameter.h"
#include "channel.h"

class IBoard;

typedef std::shared_ptr<IBoard> Board;

class IBoard
{
public:
    IBoard(int h, std::size_t s, std::string m, std::string d, std::size_t n, std::string sn, std::string fw, bool readOnly);
    ~IBoard();

    // Factory method
    static Board create(int h, std::size_t s, std::string m, std::string d, std::size_t n, std::string sn, std::string fw, bool readOnly);

    void printInfo(std::ostream& stream) const;
    void printBoardInfo(std::ostream& stream) const;

    std::vector<BoardParameterNumeric>  getBoardParameterNumerics()   { return boardParameterNumerics;   };
    std::vector<BoardParameterOnOff>    getBoardParameterOnOffs()     { return boardParameterOnOffs;     };
    std::vector<BoardParameterChStatus> getBoardParameterChStatuses() { return boardParameterChStatuses; };
    std::vector<BoardParameterBdStatus> getBoardParameterBdStatuses() { return boardParameterBdStatuses; };
    std::vector<Channel>                getChannels()                 { return channels;                 };

private:

    void GetBoardParams();
    void GetBoardChannels();

    int                         handle;
    std::size_t                 slot;
    std::string                 model;
    std::string                 description;
    std::size_t                 numChannels;
    std::string                 serialNumber;
    std::string                 firmwareRelease;
    bool                        readOnly;

    std::vector<BoardParameterNumeric>  boardParameterNumerics;
    std::vector<BoardParameterOnOff>    boardParameterOnOffs;
    std::vector<BoardParameterChStatus> boardParameterChStatuses;
    std::vector<BoardParameterBdStatus> boardParameterBdStatuses;

    std::vector<Channel> channels;
};

#endif
