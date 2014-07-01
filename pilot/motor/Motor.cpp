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

#include "Motor.h"

Motor::Motor() {
    m_servoId[FRONT_RIGHT] = 0;
    m_servoId[FRONT_LEFT] = 1;
    m_servoId[REAR_LEFT] = 2;
    m_servoId[REAR_RIGHT] = 3;

    fid_servo = NULL;
}

Motor::~Motor() {
}

bool Motor::check_blaster_exec() {
    // TODO
    return true;
}

bool Motor::is_open_blaster() {
    if (fid_servo == NULL) {
        //printf("servoblaster not open !!!");
        return false;
    }

    return true;
}

void Motor::open_blaster() {

    if (is_open_blaster())
        return;

    fid_servo = fopen("/dev/servoblaster", "w");
    if (fid_servo == NULL) {
        printf("Opening /dev/servoblaster failed \n");
        exit(2);
    }

    //initialisation of ESC
    for (int i = 0; i < 4; i++) {
        m_servoVal[i] = MOTOR_MIN;
    };

    setPWM();
    sleep(1);
}

void Motor::close_blaster() {
    if (fid_servo == NULL) {
        printf("/dev/servoblaster not opened \n");
        return;
    }
    fclose(fid_servo);
    fid_servo = NULL;
}

void Motor::update(int m0, int m1, int m2, int m3) {
    m_servoVal[FRONT_RIGHT] = m0 > MOTOR_MAX ? MOTOR_MAX : (m0 < MOTOR_MIN ? MOTOR_MIN : m0);
    m_servoVal[FRONT_LEFT] = m1 > MOTOR_MAX ? MOTOR_MAX : (m1 < MOTOR_MIN ? MOTOR_MIN : m1);
    m_servoVal[REAR_LEFT] = m2 > MOTOR_MAX ? MOTOR_MAX : (m2 < MOTOR_MIN ? MOTOR_MIN : m2);
    m_servoVal[REAR_RIGHT] = m3 > MOTOR_MAX ? MOTOR_MAX : (m3 < MOTOR_MIN ? MOTOR_MIN : m3);
    //std::cout << m0 << " " << m1 << " " << m2 << " " << m3 << std::endl;
    setPWM();
}

void Motor::stop() {
    for (int i = 0; i < 4; i++) {
        m_servoVal[i] = MOTOR_MIN;
    };
    setPWM();
}

void Motor::setPWM() {
    if (is_open_blaster()) {
        for (int i = 0; i < 4; i++) {
            fprintf(fid_servo, "%d=%dus\n", m_servoId[i], m_servoVal[i]);
            fflush(fid_servo);
        }
    } else {
        printf("Servoblaster not opened \n");
    }
}

void Motor::print() {
   std::cout <<  m_servoVal[FRONT_LEFT] << "\t" <<  m_servoVal[FRONT_RIGHT] << std::endl << m_servoVal[REAR_LEFT] << "\t" <<  m_servoVal[REAR_RIGHT] << std::endl;
}
