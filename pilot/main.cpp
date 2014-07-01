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

#include "main.h"

// TODO remove this global shit
RTIMU *imu;
RTIMU_DATA imuData;

ControlClass Control;
TimerClass Timer;
Motor ESC;

LoggerClass LogMan;

SocketRemoteController remote;
CLI_class CLI;

CommandParser parser;

void task1(SocketRemoteController remote) {
    CLI.open();
}

void my_handler(int s) {
    ESC.update(MOTOR_MIN, MOTOR_MIN, MOTOR_MIN, MOTOR_MIN);
    printf("\nEmergency stop!!\n");
    exit(1);
}

int main(int argc, char** argv) {
    std::cout << "\033[1;31m" << std::endl << "--------------------" << std::endl << " πCopter Pilot v0.1 " << std::endl << "--------------------" << "\033[0m" << std::endl << std::endl;

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    ESC.open_blaster();

    RTIMUSettings *settings = new RTIMUSettings("pi-copter");
    imu = RTIMU::createIMU(settings);
    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        printf("No IMU found\n");
        exit(1);
    }
    imu->IMUInit();

    Timer.start();

    // CLI on other thread
    std::thread t1(task1, remote);

    // remote on main thread
    remote.open();
    remote.start();

    t1.join();
}




