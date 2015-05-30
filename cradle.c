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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "cradle.h"

#define D_NAME_SIZE 16

void expected(const char* s)
{
	printf("Exepcted %s\n", s);
	exit(1);
}

int match(parser_t* self, char c)
{
	if(self->look == c) {
		next(self);
		return 1;
	} else {
		printf("expected %c\n", c);
		exit(1);
	}
}

char getName(parser_t* self)
{
	if(!isalpha((int)self->look)) {
		expected("char");
	} 
	return self->look;
}

/*
 * This method continues reading a string until it doesn't see 
 * anymore numerials. It then converts that string into a long long integer
 */
long long int getNumber(parser_t* self)
{
	int buffer_size = D_NAME_SIZE;
	long long int out = 0;
	int i = 0;
	char* buffer = malloc(D_NAME_SIZE);
	if(buffer == NULL) {
		return 0;
	}
	if(!isdigit((int)self->look)) {
		expected("integer");
	} else {
		while(isdigit((int)self->look)) {
			if(i == buffer_size) {
				buffer_size *= 2;
				buffer = realloc(buffer, buffer_size);
				if(buffer == NULL) {
					return 0;
				}
			}
			buffer[i++] = self->look;
			next(self);
		}
		buffer[i] = '\0';
		out = strtoll(buffer, NULL, 10);
	}
	free(buffer);
	return out;
}

void init(parser_t* self)
{
	next(self);
}

void expression(parser_t* self)
{
	printf("LA R0, %lli\n", getNumber(self));
}

void init_parser(parser_t* self, char* text)
{
	self->text = text;
	self->pos = 0;
	self->len = strlen(text);
	self->look = EOF;
	self->status = 0;
}

void next(parser_t* self)
{
	if(self->pos >= self->len) {
		self->status = EOF;
		self->look = '\0';
		return;
	}
	
	self->look = self->text[self->pos++];
}



















