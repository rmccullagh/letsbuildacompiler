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
#ifndef __CRADLE_H__
#define __CRADLE_H__

#include <stddef.h>

typedef struct {
	char* text; /* source code */
	size_t pos; /* current position in text */
	size_t len; /* length of text */
	char look;  /* lookahead character */
	int status; /* weather we are at EOF */
	int line;   /* the current line number, also total lines */
} parser_t;

void expected(parser_t*, const char*) __attribute__ ((noreturn));
int match(parser_t*, char);
char* getName(parser_t*);
long long int getNumber(parser_t*);
void init(parser_t*);
void factor(parser_t*);
void term(parser_t*);
void expression(parser_t*);
void add(parser_t*);
void subtract(parser_t*);
int init_parser(parser_t*, char* text);
void parser_shutdown(parser_t*);
void next(parser_t*);

#endif /* __CRADLE_H__ */