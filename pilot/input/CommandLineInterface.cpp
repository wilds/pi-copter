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

#include "CommandLineInterface.h"
#include "main.h"

CLI_class::CLI_class() {

}

CLI_class::CLI_class(const CLI_class& orig) {
    //std::string stringbuf_[6] = {0};
}

CLI_class::~CLI_class() {
}

void CLI_class::open() {
    while(1) {
	std::string line;
	do {
	    std::cout << "\033[32m" << "πCopter> " << "\033[0m";
	    do {
		std::cin.clear();
		line.clear();
		std::getline(std::cin, line);
	    } while(std::cin.fail());
	} while(line.length() == 0);

	std::stringstream stream(line);
	int i = 0;
	while(std::getline(stream, stringbuf_[i], ' ')) {
	    ++i;
	}

        std::string resp = parser.parse(stringbuf_);
        if (!resp.empty())
            std::cout << resp << std::endl;
    }
}
