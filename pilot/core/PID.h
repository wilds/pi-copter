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

#ifndef PID_H
#define	PID_H

#include <cstddef>

class PIDClass
{
public:
    PIDClass();
    PIDClass(const PIDClass& orig);
    virtual ~PIDClass();
    void initialise(float KP, float KI, float KD, float ILIM, float LIM, int DFILLEN, double* ALT_DERIV_SOURCE = NULL);
    void calculate(double position, float setpoint, float dt);
    void setPID(float KP, float KI, float KD);
    void getPID();
    float output;
private:
    void constrain_(float* value, float range);
    float proportional, integral, derivative;
    float dHist[32];
    float kp;
    float ki;
    float kd;
    float ilim;
    float lim;
    int dFilLen;
    int dFilK;
    float prevError;
    double* altDerivativeSource;
};

#endif	/* PID_H */

