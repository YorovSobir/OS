#include <string.h>

int strcmp(const char *str1, const char *str2){

	while (*str1 && *str2){
		if (*str1 > *str2){
			return 1;
		}

		if (*str1 < *str2){
			return -1;
		}

		++str1;
		++str2;
	}

	if (!(*str1) && !(*str2)){
		return 0;
	}
	if (*str1){
		return 1;
	}
	return -1;

}

int strncmp(const char *str1, const char *str2, int n){
	int i = 0;
	while ((i < n) && *str1 && *str2){
		if (*str1 > *str2){
			return 1;
		}

		if (*str1 < *str2){
			return -1;
		}

		++str1;
		++str2;
		++i;
	}

	if (i == n){
		return 0;
	}

	if (*str1){
		return 1;
	}

	return -1;

}

size_t strlen(const char *str)
{
	const char *begin = str;

	while (*str++);
	return str - begin - 1;
}

void *memcpy(void *dst, const void *src, size_t size)
{
	char *to = dst;
	const char *from = src;

	while (size--)
		*to++ = *from++;
	return dst;
}

void *memset(void *dst, int fill, size_t size)
{
	char *to = dst;

	while (size--)
		*to++ = fill;
	return dst;
}
