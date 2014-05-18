#include "base64.h"

typedef struct _io io;

struct _io {
	base64_readfn read;
	base64_writefn write;
	void *rdata;
	void *wdata;
	unsigned char *enc;
	unsigned char dec[256];
};

static int encode(io *x);
static int decode(io *x);
static void gendec(io *x);
static void io_init(io *x, base64_readfn read, void *readstream, base64_writefn write, void *writestream);

void io_init(io *x, base64_readfn read, void *readstream, base64_writefn write, void *writestream)
{
	if(read != NULL)
		x->read = read;
	else
		x->read = (base64_readfn)fread;
	if(write != NULL)
		x->write = write;
	else
		x->write = (base64_writefn)fwrite;
	x->rdata = readstream;
	x->wdata = writestream;
	x->enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
}

int base64_encode(base64_readfn read, void *readstream, base64_writefn write, void *writestream)
{
	io x = { 0 };

	io_init(&x, read, readstream, write, writestream);

	return encode(&x);
}

int base64_decode(base64_readfn read, void *readstream, base64_writefn write, void *writestream)
{
	io x;

	io_init(&x, read, readstream, write, writestream);
	gendec(&x);

	return decode(&x);
}

size_t base64_fread(void *ptr, size_t size, size_t nitems, void *stream)
{
	return fread(ptr, size, nitems, stream);
}

size_t base64_fwrite(void *ptr, size_t size, size_t nitems, void *stream)
{
	return fwrite(ptr, size, nitems, stream);
}

int xgetc(io *x)
{
	unsigned char c;
	size_t len;

	len = x->read(&c, 1, 1, x->rdata);
	if(len == 0)
		return EOF;

	return c;
}

int xputc(unsigned char c, io *x)
{
	size_t len;

	len = x->write(&c, 1, 1, x->wdata);
	if(len == 0)
		return EOF;

	return c;
}

static int encode(io *x)
{
	int c;
	int state;
	unsigned char e;
	int n;

	n = 0;
	state = 0;

	while((c = xgetc(x)) != EOF) {
		switch(state) {
		case 0:
			e = (c >> 2) & 077;
			n += xputc(x->enc[e], x);
			e = (c << 4) & 077;
			break;
		case 1:
			e |= (c >> 4) & 077;
			n += xputc(x->enc[e], x);
			e = (c << 2) & 077;
			break;
		case 2:
			e |= (c >> 6) & 077;
			n += xputc(x->enc[e], x);
			e = c & 077;
			n += xputc(x->enc[e], x);
			break;
		}
		state = (state + 1) % 3;
	}

	switch(state) {
	case 1:
		n += xputc(x->enc[e], x);
		n += xputc('=', x);
		n += xputc('=', x);
		break;
	case 2:
		n += xputc(x->enc[e], x);
		n += xputc('=', x);
		break;
	}

	return n;
}

void gendec(io *x)
{
	unsigned char *enc;
	int i;

	enc = x->enc;
	for(i = 0; i < 64; i++) 
		x->dec[enc[i]] = i + 1;
}

int decode(io *x)
{
	int c;
	int state;
	unsigned char e;
	int n;

	n = 0;
	state = 0;

	while((c = xgetc(x)) != EOF) {

		c = x->dec[c];
		if(c == 0)
			continue;
		c--;

		switch(state) {
		case 0:
			e = (c << 2);
			break;
		case 1:
			e |= (c >> 4);
			n += xputc(e, x);
			e = (c << 4);
			break;
		case 2:
			e |= (c >> 2);
			n += xputc(e, x);
			e = (c << 6);
			break;
		case 3:
			e |= c;
			n += xputc(e, x);
			break;
		}
		state = (state + 1) % 4;
	}

	return n;
}
