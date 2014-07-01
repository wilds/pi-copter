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

#include "Control.h"
#include <iomanip>
#include "main.h"
//const uint16_t MOTOR_MAX = 20000;
//const uint16_t MIN_THROTTLE = 9000;

#define MAX_THROTTLE 1700
#define MIN_THROTTLE 1100

#define PITCH_RANGE 6.0F
#define ROLL_RANGE 6.0F
#define PITCH_RATE_RANGE 50//18.0F
#define ROLL_RATE_RANGE 50//18.0F
#define YAW_RATE_RANGE 10.0F

ControlClass::ControlClass() {

    float rateIntegralLimit = 5;
    float rateProportionalLimit = 100;
    int rateDerivativeHistoryLen = 1;
    float rateRollPitchP = 0.5;
    float rateRollPitchI = 1.5;
    float rateRollPitchD = 0.02;
    float rateYawP = 8.0;
    float rateYawI = 4.0;
    float rateYawD = 0.0;

    float attitudeIntegralLimit = 1;
    float attitudeProportionalLimit = 100;
    int attitudeDerivativeHistoryLen = 16;
    float attitudeRollPitchP = 4.0;
    float attitudeRollPitchI = 1.0;
    float attitudeRollPitchD = 0.02;

    FILE *m_fd;
    if ((m_fd = fopen("pidfile.ini", "r"))) {

        char buf[200];
        char key[200];
        char val[200];
        char val2[200];
        char val3[200];

        while (fgets(buf, 200, m_fd)) {

            val2[0] = 0;
            val3[0] = 0;

            if ((buf[0] == '#') || (buf[0] == ' ') || (buf[0] == '\n'))
                // just a comment
                continue;

            int l = sscanf(buf, "%[^=]=%s %s %s", key, val, val2, val3);
            if (l != 2 && l != 4) {
                std::cout << "Bad line in PID file: " << buf << std::endl;
                fclose(m_fd);
                continue;
            }

            if (strcmp(key, "rateIntegralLimit") == 0) {
                rateIntegralLimit = atof(val);
            } else if (strcmp(key, "rateProportionalLimit") == 0) {
                rateProportionalLimit = atof(val);
            } else if (strcmp(key, "rateDerivativeHistoryLen") == 0) {
                rateDerivativeHistoryLen = atoi(val);
            } else if (strcmp(key, "rateRollPitchPID") == 0) {
                if (l == 4) {
                    rateRollPitchP = atof(val);
                    rateRollPitchI = atof(val2);
                    rateRollPitchD = atof(val3);
                } else {
                    std::cout << "Bad line in PID file: " << buf << std::endl;
                }
            } else if (strcmp(key, "rateYawPID") == 0) {
                if (l == 4) {
                    rateYawP = atof(val);
                    rateYawI = atof(val2);
                    rateYawD = atof(val3);
                } else {
                    std::cout << "Bad line in PID file: " << buf << std::endl;
                }
            } else if (strcmp(key, "attitudeIntegralLimit") == 0) {
                attitudeIntegralLimit = atof(val);
            } else if (strcmp(key, "attitudeProportionalLimit") == 0) {
                attitudeProportionalLimit = atof(val);
            } else if (strcmp(key, "attitudeDerivativeHistoryLen") == 0) {
                attitudeDerivativeHistoryLen = atoi(val);
            } else if (strcmp(key, "attitudeRollPitchPID") == 0) {
                if (l == 4) {
                    attitudeRollPitchP = atof(val);
                    attitudeRollPitchI = atof(val2);
                    attitudeRollPitchD = atof(val3);
                } else {
                    std::cout << "Bad line in PID file: " << buf << std::endl;
                }
            }
        }
    } else {
        std::cout << "PID file not found. Using defaults!" << std::endl;
    }

    ratePitchPID.initialise(rateRollPitchP, rateRollPitchI, rateRollPitchD, rateIntegralLimit, rateProportionalLimit, rateDerivativeHistoryLen);
    rateRollPID.initialise(rateRollPitchP, rateRollPitchI, rateRollPitchD, rateIntegralLimit, rateProportionalLimit, rateDerivativeHistoryLen);
    rateYawPID.initialise(rateYawP, rateYawI, rateYawD, rateIntegralLimit, rateProportionalLimit, rateDerivativeHistoryLen);

    attitudePitchPID.initialise(attitudeRollPitchP, attitudeRollPitchI, attitudeRollPitchD, attitudeIntegralLimit, attitudeProportionalLimit, attitudeDerivativeHistoryLen/*, &imuData.gyro.x()*/);
    attitudeRollPID.initialise(attitudeRollPitchP, attitudeRollPitchI, attitudeRollPitchD, attitudeIntegralLimit, attitudeProportionalLimit, attitudeDerivativeHistoryLen/*, &imuData.gyro.x()*/);

    altitudeHoldActive_ = false;
    motorTesting_ = false;
    rateMode = true;

    calibratingESC_ = false;

    memset((char *) &rx, 0, sizeof (rx));
}

ControlClass::ControlClass(const ControlClass & orig) {
}

ControlClass::~ControlClass() {
}

void ControlClass::update() {
    if (!motorTesting_ && !calibratingESC_) {
        if (rateMode) {//in rate mode
            rateControl_(rx.pitchRateDem, rx.rollRateDem, rx.yawRateDem);
        } else { //in attitude mode
            attitudeControl_(rx.pitchDem, rx.rollDem, rx.yawRateDem);
        }
    } else if (motorTesting_) {
        incrementMotorTest_();
    } else if (calibratingESC_) {

    }
    //        evaluateAltitudeControl_(); //Checks to see if altitude control if required, and performs if necessary
}

void ControlClass::startCalibrateESC(bool max) {
    std::cout << "Starting calibrate " << std::endl;
    calibratingESC_ = true;
    if (max) {
        ESC.update(MAX_THROTTLE, MAX_THROTTLE, MAX_THROTTLE, MAX_THROTTLE);
    } else {
        ESC.update(MIN_THROTTLE, MIN_THROTTLE, MIN_THROTTLE, MIN_THROTTLE);
    }
}

void ControlClass::stopCalibrateESC() {
    std::cout << "Stop calibrate" << std::endl;
    calibratingESC_ = false;
}

void ControlClass::startMotorTest() {
    std::cout << "Starting motor test" << std::endl;
    motorTestCounter_ = 0;
    motorTesting_ = true;
}

void ControlClass::incrementMotorTest_() {
    switch (motorTestCounter_ / 100) {
        case 0:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            break;
        case 1:
            ESC.update(MIN_THROTTLE, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            break;
        case 2:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            break;
        case 3:
            ESC.update(MOTOR_MIN, MIN_THROTTLE, MOTOR_MIN, MOTOR_MIN);
            break;
        case 4:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            break;
        case 5:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MIN_THROTTLE, MOTOR_MIN);
            break;
        case 6:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            break;
        case 7:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MIN_THROTTLE);
            break;
        case 8:
            ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
            motorTesting_ = false;
            std::cout << "Motor test complete" << std::endl;
            break;
    }
    motorTestCounter_++;
}

inline void ControlClass::updatePWM_(float throttle, float pitch, float roll, float yaw) {
    if (throttle == 0) {
        ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
    } else {

        int throttleVal = (int) ((throttle * (MAX_THROTTLE - MIN_THROTTLE)) + MIN_THROTTLE); // throttle is in %, here scaled with min max value
        int m2 = throttleVal - pitch + roll - yaw + altitudePID.output;
        int m1 = throttleVal - pitch - roll + yaw + altitudePID.output;
        int m4 = throttleVal + pitch - roll - yaw + altitudePID.output;
        int m3 = throttleVal + pitch + roll + yaw + altitudePID.output;

        ESC.update(m1, m2, m3, m4);

        /*
        std::cout << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A" << "\x1b[A";
        std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(6);
        std::cout << "\r" << "Pitch " << imuData.fusionPose.x() << std::setw(15) << " Roll " << imuData.fusionPose.y() << std::setw(15) << " Yaw " << imuData.fusionPose.z() << std::endl;
        std::cout << "\r" << "GyroX " << imuData.gyro.x() << std::setw(15) << " GyroY " << imuData.gyro.y() << std::setw(15) << " GyroZ " << imuData.gyro.z() << std::endl;
        std::cout << "\r" << "AccX  " << imuData.accel.x() << std::setw(15) << " AccY  " << imuData.accel.y() << std::setw(15) << " AccZ  " << imuData.accel.z() << std::endl;
        std::cout << "\r" << "Throttle " << throttleVal << std::setw(15) << "Set Pitch " << pitch << std::setw(15) << " Roll " << roll << std::setw(15) << " Yaw " << yaw << std::endl;
        std::cout << "\r\r";
        ESC.print();
        */
    }
}

void ControlClass::rateControl_(float pitchRate, float rollRate, float yawRate) {

    ratePitchPID.calculate(imuData.gyro.x()*(180.0 / M_PI), pitchRate, Timer.dt);
    rateRollPID.calculate(imuData.gyro.y()*(180.0 / M_PI), rollRate, Timer.dt);
    rateYawPID.calculate(imuData.gyro.z()*(180.0 / M_PI), yawRate, Timer.dt);

    /*
    ratePitchPID.calculate(imuData.fusionPose.y()*(180.0 / M_PI), pitchRate, Timer.dt);
    rateRollPID.calculate(imuData.fusionPose.x()*(180.0 / M_PI), rollRate, Timer.dt);
    rateYawPID.calculate(imuData.fusionPose.z()*(180.0 / M_PI), yawRate, Timer.dt);
     */
    updatePWM_(rx.throttleDem, ratePitchPID.output, rateRollPID.output, rateYawPID.output);
}

void ControlClass::attitudeControl_(float targetPitch, float targetRoll, float targetYawRate) {
    attitudePitchPID.calculate(imuData.fusionPose.x()*(180.0 / M_PI), targetPitch, Timer.dt);
    attitudeRollPID.calculate(imuData.fusionPose.y()*(180.0 / M_PI), targetRoll, Timer.dt);
    rateControl_(attitudePitchPID.output, attitudeRollPID.output, targetYawRate);
}

void ControlClass::setRatePID(float KP, float KI, float KD) {
    ratePitchPID.setPID(KP, KI, KD);
    rateRollPID.setPID(KP, KI, KD);
}

void ControlClass::getRatePID() {
    ratePitchPID.getPID();
    rateRollPID.getPID();
}

void ControlClass::setAttitudePID(float KP, float KI, float KD) {
    attitudePitchPID.setPID(KP, KI, KD);
    attitudeRollPID.setPID(KP, KI, KD);
}

void ControlClass::getAttitudePID() {
    attitudePitchPID.getPID();
    attitudeRollPID.getPID();
}

void ControlClass::evaluateAltitudeControl_() {
    // TODO
    /*
    if(PICInterface.rxWidths.sw2 > 15000 && altitudeHoldActive_ == false) {
        targetAltitude_ = AHRS.calibratedData.altitude;
        altitudeHoldActive_ = true;
        altitudeControl_();
        std::cout << "Altitude hold active at " << targetAltitude_ << "m" << std::endl;
    } else if(PICInterface.rxWidths.sw2 > 15000 && altitudeHoldActive_ == true) {
        altitudeControl_();
    } else if(PICInterface.rxWidths.sw2 < 15000)*/
    {
        altitudeHoldActive_ = false;
        altitudePID.output = 0;
    }
}

void ControlClass::altitudeControl_() {
    altitudePID.prevError = altitudePID.error;
    //altitudePID.error = targetAltitude_ - AHRS.calibratedData.altitude;
    //altitudePID.differential = (altitudePID.error - altitudePID.prevError) / Timer.dt;
    altitudePID.output = altitudePID.constants.kp * altitudePID.error + altitudePID.differential * altitudePID.error;
    constrain_(&altitudePID.output, 500);
}

inline void ControlClass::constrain_(double* value, float range) {
    if (*value > range) *value = range;
    else if (*value < -range) *value = -range;
}

void ControlClass::updateRX(float throttle, float pitch, float roll, float yaw) {
    rx.pitchDem = PITCH_RATE_RANGE * pitch / 50;
    rx.rollDem = ROLL_RATE_RANGE * roll / 50;
    rx.yawRateDem = YAW_RATE_RANGE * yaw / 50;
    rx.throttleDem = throttle / 100;
    /*
    rx.pitchDem = pitch;
    rx.rollDem = roll;
    rx.yawRateDem = yaw;
    rx.throttleDem = throttle;*/
    //rx.sw1 = (rxWidths.sw1 > 15000);
    //rx.sw2 = (rxWidths.sw2 > 15000);
    rx.pitchRateDem = PITCH_RATE_RANGE * pitch / 50; // TODO remove
    rx.rollRateDem = ROLL_RATE_RANGE * roll / 50; // TODO remove


    rx.yawRateDem = -rx.yawRateDem;
}