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

#ifndef MAIN_H
#define	MAIN_H


#include "Logger.h"
#include "CommandLineInterface.h"
#include "SocketRemoteController.h"
#include "Timer.h"
#include "RTIMULib.h"
#include "Motor.h"

#include <thread>
#include <iostream>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// TODO remove this global shit
extern RTIMU *imu;
extern RTIMU_DATA imuData;

extern Motor ESC;
extern SocketRemoteController remote;
extern TimerClass Timer;
extern LoggerClass LogMan;
extern ControlClass Control;
extern CommandParser parser;


#endif	/* MAIN_H */


