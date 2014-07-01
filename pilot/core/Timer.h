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

#ifndef TIMER_H
#define	TIMER_H

#include <signal.h>
#include <iostream>

#include "Logger.h"

#include "Control.h"

class TimerClass
{
public:
    TimerClass();
    TimerClass(const TimerClass& orig);
    virtual ~TimerClass();
    
    void start();
    void stop();
    
    float dt;
    timer_t timerId; //id  for the posix timer
    struct sigaction signalAction; //signal action handler struct
    bool started;
private:
    static void sig_handler_(int signum);
    struct itimerspec timeToSet_; //time to be set
    struct timespec timeValue_; //timer expiration value
    struct timespec timeInterval_; //timer period
    timespec oldtime_;
    timespec time_;
    timespec iterationtime_;
    
    void calcdt_();
    void compensate_();

};

#endif	/* TIMER_H */

