#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>

typedef size_t (*base64_iofunc)(void *, size_t, size_t, void *);

int base64_encode(base64_iofunc read, void *readstream, base64_iofunc write, void *writestream);
int base64_decode(base64_iofunc read, void *readstream, base64_iofunc write, void *writestream);

size_t base64_fread(void *ptr, size_t size, size_t nitems, void *stream);
size_t base64_fwrite(void *ptr, size_t size, size_t nitems, void *stream);

#endif
