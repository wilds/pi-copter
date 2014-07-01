/*
 * Copyright (C) 2014 Danilo Selvaggi & Ivano Selvaggi
 * Copyright (C) 2013 Vincent Jaunet
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

#ifndef MOTOR_H
#define MOTOR_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MOTOR_MAX 2000       //2ms in us
#define MOTOR_MIN 1000       //1ms in us

#define FRONT_RIGHT 0
#define FRONT_LEFT  1
#define REAR_LEFT   2
#define REAR_RIGHT  3

#define COUNT_MOTOR   4

class Motor {
private:
    uint8_t m_servoId[COUNT_MOTOR];
    uint16_t m_servoVal[COUNT_MOTOR];
    
    FILE* fid_servo;

public:
    Motor();
    virtual ~Motor();

    void open_blaster();
    void close_blaster();
    bool is_open_blaster();
    bool check_blaster_exec();

    void update(int m0, int m1, int m2, int m3);
    void stop();
    
    void print();

protected:    
    void setPWM();
};

#endif
