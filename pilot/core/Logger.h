/*
 * Copyright (C) 2014 Danilo Selvaggi & Ivano Selvaggi
 * Copyright (C) 2012 Matt
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

#ifndef LOGGER_H
#define	LOGGER_H

#include <iostream>
#include <fstream>
#include <sstream>



class LoggerClass
{
public:
    LoggerClass();
    LoggerClass(const LoggerClass& orig);
    virtual ~LoggerClass();
    
    void open(const char* filename);
    void update();
    void doWeNeedToFlush();
    int sampleno;
    bool logging;
    std::stringstream log;
    
private:
    int timeSinceLastFlush;
    std::fstream logFile;

};

#endif	/* LOGGER_H */

