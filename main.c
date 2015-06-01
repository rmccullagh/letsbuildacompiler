/*
 *  Copyright (c) 2015 Ryan McCullagh <me@ryanmccullagh.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "cradle.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if(argc != 2) {
		return 1;
	}
	parser_t* parser = malloc(sizeof(parser_t));
	if(!parser) {
		return 1;
	}
	int status = init_parser(parser, argv[1]);
	if(status == 1) {
		free(parser);
		return 1;
	}
	init(parser);
	while(parser->status != EOF) {
		expression(parser);
	}
	parser_shutdown(parser);
	return 0;
}