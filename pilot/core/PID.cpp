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

#include "PID.h"
#include <iostream>

PIDClass::PIDClass() : dFilK(0), prevError(0) {
}

PIDClass::PIDClass(const PIDClass& orig) {
}

PIDClass::~PIDClass() {
}

void PIDClass::initialise(float kp, float ki, float kd, float ilim, float lim, int dFilLen, double* altDerivativeSource) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
    
    this->ilim = ilim;
    this->lim = lim;
    this->dFilLen = dFilLen;
    this->altDerivativeSource = altDerivativeSource;
}

void PIDClass::setPID(float kp, float ki, float kd) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

void PIDClass::getPID() {
    std::cout << kp << ", " << ki << ", " << kd << std::endl;
}

void PIDClass::calculate(double position, float setpoint, float dt) {

    double error = setpoint - position;
    integral += error * dt;

    if (altDerivativeSource == NULL) {
        if (kd != 0) {
            //Derivative low pass filter
            //Store current derivative value into history table
            dHist[dFilK] = (error - prevError) / dt;
            dFilK++;
            if (dFilK == dFilLen) {
                dFilK = 0;
            }
            //Average history table
            derivative = 0;
            for (int k = 0; k < dFilLen; k++) {
                derivative += dHist[k];
            }
            derivative /= dFilLen;
        }
    } else {
        derivative = -*altDerivativeSource;
    }

    //Anti-windup
    constrain_(&integral, ilim);

    // TEST
//    if (error == 0)
//        integral = 0;
    
    output = error * kp + integral * ki + derivative * kd;

    //Anti-saturation
    constrain_(&output, lim);

    prevError = error;
}

//void PIDClass::calculate(double* position, float* setpoint, float* dt) {
//    prevError = error;
//    error = *setpoint - *position;
//    proportional = error;
//    integral += error * *dt;
//    derivative = (error - prevError) / *dt;
//
//    //Anti-windup
//    constrain_(&integral, ilim);
//
//    output = proportional * kp + integral * ki + derivative*kd;
//
//    //Anti-saturation
//    constrain_(&output, lim);
//}

inline void PIDClass::constrain_(float* value, float range) {
    if (*value > range) *value = range;
    else if (*value < -range) *value = -range;
}
