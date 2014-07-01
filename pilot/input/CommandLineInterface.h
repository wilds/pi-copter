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

#ifndef COMMANDLINEINTERFACE_H
#define	COMMANDLINEINTERFACE_H

#include <iostream>
#include <map>

#include "CommandParser.h"


class CLI_class
{
public:
    CLI_class();
    CLI_class(const CLI_class& orig);
    virtual ~CLI_class();
    void open();
private:
    std::string stringbuf_[6];
};

#endif	/* COMMANDLINEINTERFACE_H */

