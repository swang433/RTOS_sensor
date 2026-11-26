#include <stddef.h>

void *memcpy(void *dest, const void *source, size_t sz){
    unsigned char* d = dest; 
    const unsigned char* s = source; 
    while(sz--) *d++ = *s++; 
    return dest;
}

void *memset(void *dest, int val, size_t sz){
    unsigned char *d = dest;
    while(sz--) *d++ = (unsigned char)val; 
    return dest; 
}