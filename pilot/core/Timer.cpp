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

#include "Timer.h"
#include "main.h"

#define PERIOD 2500000

pthread_mutex_t TimerMutex_;

TimerClass::TimerClass()
{
    /* Intialize sigaction struct */
    signalAction.sa_handler = &sig_handler_;



    /* Connect a signal handler routine to the SIGALRM signal */
    sigaction(SIGALRM, &signalAction, NULL);

    /* Allocate a timer */
    timer_create(CLOCK_REALTIME, NULL, &timerId);
    
    started = false;
}

TimerClass::TimerClass(const TimerClass& orig)
{
}

TimerClass::~TimerClass()
{
}

void TimerClass::start()
{
    timeValue_.tv_sec = 0;
    timeValue_.tv_nsec = PERIOD;
    timeToSet_.it_value = timeValue_;
    timer_settime(timerId, 0, &timeToSet_, NULL);
    started = true;
}

void TimerClass::stop()
{
    timeValue_.tv_sec = 0;
    timeValue_.tv_nsec = 0;
    timeToSet_.it_value = timeValue_;
    timer_settime(timerId, 0, &timeToSet_, NULL);
    started = false;
}

inline void TimerClass::calcdt_()
{
    oldtime_ = time_;
    clock_gettime(CLOCK_MONOTONIC, &time_);
    Timer.dt = ((static_cast<int64_t>(time_.tv_sec) * 1000000000 + static_cast<int64_t>(time_.tv_nsec)) - (static_cast<int64_t>(oldtime_.tv_sec) * 1000000000 + static_cast<int64_t>(oldtime_.tv_nsec))) / 1000000000.0;
}

inline void TimerClass::compensate_()
{
    //Timer aims to get as close to 400Hz as possible, mostly limited by the I2C bandwidth
    clock_gettime(CLOCK_MONOTONIC, &iterationtime_);
    long inttime = PERIOD - ((iterationtime_.tv_sec * 1000000000 + iterationtime_.tv_nsec) - (time_.tv_sec * 1000000000 + time_.tv_nsec));
    if (inttime < 0)
	Timer.timeValue_.tv_nsec = 1;
    else
	Timer.timeValue_.tv_nsec = inttime;
    Timer.timeToSet_.it_value = Timer.timeValue_;
    timer_settime(timerId, 0, &timeToSet_, NULL);
}

template <class T, void(T::*member_function)()>
void* thunk(void* p)
{
    (static_cast<T*> (p)->*member_function)();
    return 0;
}

void TimerClass::sig_handler_(int signum)
{
    pthread_mutex_lock(&TimerMutex_);
    //PICInterface.getRX(); //remote controller is update in other thread
    Timer.calcdt_();

    //usleep(imu->IMUGetPollInterval() * 1000);
    if (imu->IMURead()) {
        imuData = imu->getIMUData();
    }
    
    Control.update();
    LogMan.update();
    Timer.compensate_();
    pthread_mutex_unlock(&TimerMutex_);
}
