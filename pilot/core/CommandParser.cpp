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

#include "CommandParser.h"
#include "main.h"

const char* com_testmotor(int, int, int, int);
const char* com_rcinput(float, float, float, float);

//constructor

CommandParser::CommandParser() {
    initialiseMap_();
}

//destructor

CommandParser::~CommandParser() {
}

std::string CommandParser::parse(std::string stringbuf_[]) {
    std::string resp("");
    switch (lineMap_[stringbuf_[0]]) {
        case en_stringNotDefined:
            std::cout << stringbuf_[0] << " isn't a valid command" << std::endl;
            break;

        case en_hello:
            resp = "OK";
            break;

        case en_openlog:
            std::cout << "Opening log at " << stringbuf_[1] << std::endl;
            LogMan.open(stringbuf_[1].c_str());
            break;

        case en_dumporientation:
            //TODO resp
            if (stringbuf_[1] == "rad")
                std::cout << "Pitch " << imuData.fusionPose.x() << ", roll " << imuData.fusionPose.y() << ", yaw " << imuData.fusionPose.z() << std::endl;
            else
                std::cout << "Pitch " << (imuData.fusionPose.x()*(180.0 / M_PI)) << ", roll " << (imuData.fusionPose.y()*(180.0 / M_PI)) << ", yaw " << (imuData.fusionPose.z()*(180.0 / M_PI)) << std::endl;
            break;

        case en_dumpsensors:
            std::cout << "AccX " << imuData.accel.x() << ", AccY " << imuData.accel.y() << ", AccZ " << imuData.accel.z() << ", GyrX " << imuData.gyro.x() << ", GyrY " << imuData.gyro.y() << ", GyrZ " << imuData.gyro.z() << ", temp " << imuData.temperature << ", MagX " << imuData.compass.x() << ", MagY " << imuData.compass.y() << ", MagZ " << imuData.compass.z() << ", pressure " << imuData.pressure << ", altitude " << 0 << std::endl;
            break;

        case en_setRatePID:
            Control.setRatePID(atof(stringbuf_[1].c_str()), atof(stringbuf_[2].c_str()), atof(stringbuf_[3].c_str()));
            break;

        case en_getRatePID:
            Control.getRatePID();
            break;

        case en_setAttitudePID:
            Control.setAttitudePID(atof(stringbuf_[1].c_str()), atof(stringbuf_[2].c_str()), atof(stringbuf_[3].c_str()));
            break;

        case en_getAttitudePID:
            Control.getAttitudePID();
            break;

        case en_setYawPID:
            break;

        case en_getYawPID:
            break;

        case en_getdt:
            std::cout << Timer.dt << std::endl;
            break;

        case en_startMotorTest:
            resp = com_testmotor(atoi(stringbuf_[1].c_str()), atoi(stringbuf_[2].c_str()), atoi(stringbuf_[3].c_str()), atoi(stringbuf_[4].c_str()));
            break;

        case en_rcinput:
            resp = com_rcinput(atof(stringbuf_[1].c_str()), atof(stringbuf_[2].c_str()), atof(stringbuf_[3].c_str()), atof(stringbuf_[4].c_str()));
            break;

        case en_exit:
            resp = "OK";
            exit(1);
            break;

        case en_dumpmotor:
            ESC.print();
            break;
            
        case en_calibrate:
        {
            bool max = strcmp(stringbuf_[1].c_str(), "max") == 0;
            bool min = strcmp(stringbuf_[1].c_str(), "min") == 0;
            bool stop = strcmp(stringbuf_[1].c_str(), "stop") == 0;
            resp = "OK";
            if (stop)
                Control.stopCalibrateESC();
            else if (max)
                Control.startCalibrateESC(true);
            else if (min)
                Control.startCalibrateESC(false);
            else
                resp = "KO";
            break;
        }
        default:
            std::cout << stringbuf_[0] << " isn't a valid command" << std::endl;
            break;
    }

    return resp;
}

const char* com_testmotor(int m0, int m1, int m2, int m3) {
    //ESC.open_blaster();
    //if (m0 < 0 || m1 < 0 || m2 < 0 || m3 < 0) {
    Control.startMotorTest();
    //} else {
    //    ESC.update(m0, m1, m2, m3);
    //}
    //ESC.close_blaster();
    return "OK";
}

const char* com_rcinput(float throttle, float pitch, float roll, float yaw) {
    Control.updateRX(throttle, pitch, roll, yaw);
    //ESC.open_blaster();
    //ESC.update(throttle, pitch, roll, yaw);
    //ESC.close_blaster();
    return "OK";
}

void CommandParser::initialiseMap_() {
    lineMap_[""] = en_stringNotDefined;
    lineMap_["hello"] = en_hello;
    lineMap_["openlog"] = en_openlog;
    lineMap_["do"] = en_dumporientation;
    lineMap_["ds"] = en_dumpsensors;
    lineMap_["srpid"] = en_setRatePID;
    lineMap_["grpid"] = en_getRatePID;
    lineMap_["sypid"] = en_setYawPID;
    lineMap_["gypid"] = en_getYawPID;
    lineMap_["sapid"] = en_setAttitudePID;
    lineMap_["gapid"] = en_getAttitudePID;
    lineMap_["gdt"] = en_getdt;
    lineMap_["startmotortest"] = en_startMotorTest;
    lineMap_["tm"] = en_startMotorTest;
    lineMap_["rcinput"] = en_rcinput;
    lineMap_["exit"] = en_exit;
    lineMap_["calibrate"] = en_calibrate;
    lineMap_["dm"] = en_dumpmotor;
}
