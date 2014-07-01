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

#include "Logger.h"
#include "Timer.h"
#include "Control.h"

LoggerClass::LoggerClass() {
    logging = false;
    sampleno = 0;
    log.str().reserve(100e6); //Allocate 100mb
}

LoggerClass::LoggerClass(const LoggerClass& orig) {
}

LoggerClass::~LoggerClass() {
    logFile.close();
}

void LoggerClass::open(const char* filename) {
    logFile.open(filename, std::fstream::out);
    logFile.rdbuf()->pubsetbuf(0, 0); //No buffer
    logging = true;
}

void LoggerClass::doWeNeedToFlush() {
    if (timeSinceLastFlush > 1000) {
        logFile << log.str();
        log.str(std::string()); //Clear log
        sync(); //Force dirty page write
        timeSinceLastFlush = 0;
    } else {
        timeSinceLastFlush++;
    }
}

void LoggerClass::update() {
    if (logging) {
        sampleno++;
        /* TODO
        log << sampleno << ", "
                << Timer.dt * 1000 << ", "
                << AHRS.calibratedData.x << ", "
                << AHRS.calibratedData.y << ", "
                << AHRS.calibratedData.z << ", "
                << AHRS.calibratedData.p << ", "
                << AHRS.calibratedData.q << ", "
                << AHRS.calibratedData.r << ", "
                << AHRS.calibratedData.temp << ", "
                << AHRS.calibratedData.magx << ", "
                << AHRS.calibratedData.magy << ", "
                << AHRS.calibratedData.magz << ", "
                << AHRS.calibratedData.pressure << ", "
                << AHRS.calibratedData.altitude << ", "
                << AHRS.orientation.pitch << ", "
                << AHRS.orientation.roll << ", "
                << AHRS.orientation.yaw << ", "
                << Control.ratePitchPID.output << ", "
                << Control.rateRollPID.output << ", "
                << Control.rateYawPID.output << ", "
                << Control.attitudePitchPID.output << ", "
                << Control.attitudeRollPID.output << ", "
                << AHRS.quaternion.w << ", "
                << AHRS.quaternion.x << ", "
                << AHRS.quaternion.y << ", "
                << AHRS.quaternion.z
                //Add additional logs below
                << std::endl;
        */
        /*if (PICInterface.rx.sw1 == false) {
            doWeNeedToFlush();
        }*/
    }
}