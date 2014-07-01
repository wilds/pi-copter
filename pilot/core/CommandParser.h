/*
 * Copyright (C) 2014 Danilo Selvaggi & Ivano Selvaggi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "Control.h"
#include "Motor.h"
#include "Logger.h"
#include "Timer.h"

enum lineString {
    en_stringNotDefined,
    en_hello,
    en_openlog,
    en_dumporientation,
    en_dumpsensors,
    en_setRatePID,
    en_getRatePID,
    en_setAttitudePID,
    en_getAttitudePID,
    en_setYawPID,
    en_getYawPID,
    en_getdt,
    en_startMotorTest,
    en_calibrate,
    en_rcinput,
    en_dumpmotor,
    en_exit
};

class CommandParser {
public:
    CommandParser();
    ~CommandParser();
    std::string parse(std::string stringbuf_[]);

private:
    std::map<std::string, lineString> lineMap_;
    void initialiseMap_();
};

#endif
