#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>

typedef size_t (*base64_writefn)(const void *ptr, size_t size, size_t nitems, void *stream);
typedef size_t (*base64_readfn)(void *ptr, size_t size, size_t nitems, void *stream);

int base64_encode(base64_readfn read, void *readstream, base64_writefn write, void *writestream);
int base64_decode(base64_readfn read, void *readstream, base64_writefn write, void *writestream);

#endif
