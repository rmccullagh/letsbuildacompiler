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

static inline void skip_white(parser_t* self)
{
	while(self->look == '\n') {
		self->line++;
		next(self);
	}
}

void expected(parser_t* self, const char* s)
{
	switch(self->look) {
		case '\0':
			printf("error: expecting %s, but got an unexpected EOF on line %d\n", s, self->line);
		break;
		case '\t':
			printf("error: expecting %s, but got an unexpected \"\\t\" on line %d\n", s, self->line);
		break;
		default:
			printf("error: expecting %s, but got an unexpected \"%c\" on line %d\n", s, self->look, self->line);
		break;
	}
	parser_shutdown(self);
	exit(1);
}

int match(parser_t* self, char c)
{
	if(self->look == c) {
		next(self);
		return 1;
	} else {
		printf("Expected %c\n", c);
		parser_shutdown(self);
		exit(1);
	}
}

char* getName(parser_t* self)
{
	int buffer_size = D_NAME_SIZE;
	int i = 0;
	char* buffer = malloc(D_NAME_SIZE);
	if(buffer == NULL) {
		return NULL;
	}
	if(!isalpha((int)self->look)) {
		expected(self, "char");
	} 
	while(isalnum((int)self->look)) {
		if(i == buffer_size) {
			buffer_size *= 2;
			buffer = realloc(buffer, buffer_size);
			if(buffer == NULL) {
				return NULL;
			}
		}
		buffer[i++] = self->look;
		next(self);
	}
	buffer[i] = '\0';
	return buffer;
}

/*
 * This method continues reading a string until it doesn't see 
 * anymore numerials. It then converts that string into a long long integer
 */
long long int getNumber(parser_t* self)
{
	skip_white(self);
	int buffer_size = D_NAME_SIZE;
	long long int out = 0;
	int i = 0;
	char* buffer = malloc(D_NAME_SIZE);
	if(buffer == NULL) {
		return 0;
	}
	if(!isdigit((int)self->look)) {
		expected(self, "integer");
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
	skip_white(self);
	next(self);
}

void factor(parser_t* self)
{
	printf("LA R0, %lli\n", getNumber(self));
}
/*
 * the term leaves the result in register 0
 * R0
 */
void term(parser_t* self)
{
	printf("LA R0,%lli\n", getNumber(self));
}

void expression(parser_t* self)
{
	/* get a number in R0 */
	term(self);
	/* at this point there is already a number in R0 */
	expression:
		while(self->look) {
			skip_white(self);
			/*
			 * load contents of R0 into R1. Copy the contents of R1 to R0
			 * because term() uses R0 exclusively, and both add() and subtract()
			 * call term(), therefore if we don't copy the content, we'll lose
			 * the left hand operand in <term> +|- <term>
			 */
			switch(self->look) {
				case '+':
					printf("LR R1,R0\n");
					/* add is responsible for populating R0 */
					add(self);
					goto expression;
				break;
				case '-':
					printf("LR R1,R0\n");
					/* subtract is responsible for populating R0 */
					subtract(self);
					goto expression;
				break;
				case '\0':
					goto finish;
				break;
				default:
					expected(self, "+ or -");
				break;
			}
		}
		finish:
			printf("HALT\n");
}

void add(parser_t* self)
{
	match(self, '+');
	/* term puts a number in R0 */
	term(self);
	/* ADD R1 AND R0 AND STORE IT IN R0 */
	printf("AR R1,R0\n");
}

void subtract(parser_t* self)
{
	match(self, '-');
	/* term puts a number in R0 */
	term(self);
	/* SUB R1 AND R0 AND STORE IT IN R0 */
	printf("SR R1,R0\n");
}

/*
 * init the parser
 * return 1 on error, 0 on success. The caller must check for 1.
 */
int init_parser(parser_t* self, char* text)
{
	self->text = strdup(text);
	if(self->text == NULL) {
		return 1;
	}
	self->pos = 0;
	self->len = strlen(text);
	self->look = EOF;
	self->status = 0;
	self->line = 1;
	return 0;
}

void parser_shutdown(parser_t* self)
{
	#ifdef DEBUG
		fprintf(stderr, "*** debug info ***\n");
		fprintf(stderr, "total lines   : %d\n", self->line);
		fprintf(stderr, "source length : %zu\n", self->len);
		fprintf(stderr, "last position : %zu\n", self->pos);
		fprintf(stderr, "last status   : %d\n", self->status);
		fprintf(stderr, "*** end debug info ***\n");
	#endif
	free(self->text);
	free(self);
}

void next(parser_t* self)
{
	if(self->pos >= self->len) {
		self->status = EOF;
		self->look = '\0';
	} else {
		self->look = self->text[self->pos++];
	}
}



















