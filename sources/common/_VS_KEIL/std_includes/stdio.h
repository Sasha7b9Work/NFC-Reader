#pragma once

int sprintf(char *dest, char *src, ...);
int snprintf(char *, int, const char *, ...);
int sscanf(void *, void *, ...);
int vsprintf(char *__restrict, const char *__restrict, struct va_list);
