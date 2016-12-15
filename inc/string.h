#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

size_t strlen(const char *str);
int strncmp(const char *str1, const char *str2, int n);
int strcmp(const char *str1, const char *str2);
void *memcpy(void *dst, const void *src, size_t size);
void *memset(void *dst, int fill, size_t size);

#endif /*__STRING_H__*/
