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

#ifndef CONTROL_H
#define	CONTROL_H

#include "PID.h"
#include "Motor.h"
#include <cstring>

struct s_orientation
{
    double phi;
    double psi;
    double theta;
};

struct s_PIDConstants
{
    float kp;
    float ki;
    float kd;
};

struct s_PID
{
    s_orientation differential;
    s_orientation integral;
    s_PIDConstants constants;
    s_PIDConstants yawConstants;
    s_orientation error;
    s_orientation prevError;
    s_orientation output;
};

struct s_altitudePID
{
    double differential;
    double integral;
    double error;
    double prevError;
    double output;
    s_PIDConstants constants;
};

/*
struct s_rxWidths
{
    uint16_t roll;
    uint16_t pitch;
    uint16_t throttle;
    uint16_t yaw;
    uint16_t sw1;
    uint16_t sw2;
};
 */

struct s_rxcalibrated
{
    float pitchDem;
    float rollDem;
    float throttleDem;
    float yawRateDem;
    float pitchRateDem; // TODO remove
    float rollRateDem;  // TODO remove
    bool sw1;
    bool sw2;
};

class ControlClass
{
public:
    ControlClass();
    ControlClass(const ControlClass& orig);
    virtual ~ControlClass();

    void update();
    void startMotorTest();
    void startCalibrateESC(bool max);
    void stopCalibrateESC();
    void setRatePID(float KP, float KI, float KD);
    void getRatePID();
    void setAttitudePID(float KP, float KI, float KD);
    void getAttitudePID();
    s_altitudePID altitudePID;
    PIDClass ratePitchPID, rateRollPID, rateYawPID;
    PIDClass attitudePitchPID, attitudeRollPID;

    void updateRX(float throttle, float pitch, float roll, float yaw);
private:
    void incrementMotorTest_();
    void updatePWM_(float throttle, float pitch, float roll, float yaw);
    void evaluateAltitudeControl_();
    void altitudeControl_();
    void rateControl_(float pitchrate, float rollrate, float yawrate);
    void attitudeControl_(float targetPitch, float targetRoll, float targetYaw);
    void constrain_(double* value, float range);
#define differentialFilterSize 5
    s_orientation differentialBuf[differentialFilterSize];
    double targetAltitude_;
    bool altitudeHoldActive_;
    bool motorTesting_;
    int motorTestCounter_;
    
    bool calibratingESC_;
    
    bool rateMode;
    s_rxcalibrated rx;
};

#endif	/* CONTROL_H */

