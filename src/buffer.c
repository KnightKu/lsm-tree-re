/*
 * =============================================================================
 *
 *       Filename:  buffer.c
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  12/13/2012 10:21:48 PM
 *       Revision:  r1
 *       Compiler:  gcc (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5
 *
 *         Author:  Wang Wei (sofartogo), wangwei881116@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "buffer.h"

/* 
 * Big Endian
 */
uint32_t u32_from_big(unsigned char *buf) 
{
	uint32_t val = 0;

	val |= buf[0] << 24;
	val |= buf[1] << 16;
	val |= buf[2] << 8;
	val |= buf[3];
	return val;
}

uint64_t u64_from_big(unsigned char *buf)
{
	uint64_t val = 0;

	val |= (uint64_t) buf[0] << 56;
	val |= (uint64_t) buf[1] << 48;
	val |= (uint64_t) buf[2] << 40;
	val |= (uint64_t) buf[3] << 32;
	val |= (uint64_t) buf[4] << 24;
	val |= (uint64_t) buf[5] << 16;
	val |= (uint64_t) buf[6] << 8;
	val |= (uint64_t) buf[7];
	return val;
}

unsigned _next_power(unsigned x)
{
	--x;
	x |= x >> 0x01;
	x |= x >> 0x02;
	x |= x >> 0x04;
	x |= x >> 0x08;
	x |= x >> 0x10;
	return ++x;
}

void _buffer_extendby(struct buffer *b, int len)
{
	char *buffer;

	len += b->NUL;
	if(len <= b->buflen)
		return;

	if(!b->buflen)
		b->buflen = 32;

	b->buflen = _next_power(len);
	buffer = realloc(b->buf, b->buflen);
	b->buf = buffer;
}

struct buffer *buffer_new(size_t reserve)
{
	struct buffer *b = malloc(sizeof(struct buffer));
	b->buf = NULL;
	b->NUL = 0;
	b->buflen = 0;

	if(reserve) 
		_buffer_extendby(b, reserve + 1);

	return b;
}

void buffer_clear(struct buffer *b)
{
	b->NUL = 0;
	if (b->buf) 
		b->buf[b->NUL] = '\0';
}

void buffer_putint(struct buffer *b, int val)
{
	_buffer_extendby(b, sizeof(int));
	b->buf[b->NUL++] = (val >> 24) & 0xff;
	b->buf[b->NUL++] = (val >> 16) & 0xff;
	b->buf[b->NUL++] = (val >> 8) & 0xff;
	b->buf[b->NUL++] = val & 0xff;
}


void buffer_putlong(struct buffer *b, uint64_t val)
{
	_buffer_extendby(b, sizeof(uint64_t));
	b->buf[b->NUL++] = (val >> 56) & 0xff;
	b->buf[b->NUL++] = (val >> 48) & 0xff;
	b->buf[b->NUL++] = (val >> 40) & 0xff;
	b->buf[b->NUL++] = (val >> 32) & 0xff;
	b->buf[b->NUL++] = (val >> 24) & 0xff;
	b->buf[b->NUL++] = (val >> 16) & 0xff;
	b->buf[b->NUL++] = (val >> 8) & 0xff;
	b->buf[b->NUL++] = val & 0xff;
}

void buffer_putnstr(struct buffer *b, const char *str, size_t n)
{
	_buffer_extendby(b, n+1);
	memcpy(&b->buf[b->NUL], str, n);
	b->NUL += n;
	b->buf[b->NUL] = '\0';
}

char *buffer_detach(struct buffer *b)
{
	char *buffer = b->buf;
	b->NUL = 0;
	return buffer;
}

void buffer_free(struct buffer *b)
{
	if (b->buf)
		free(b->buf);
	free(b);
}
