
#ifndef Log_H
#define Log_H

#ifndef NDEBUG

#include <stdio.h>

#define Info(format, ...) \
printf ("INF|%s:%d: " format "\n", __func__, __LINE__, ##__VA_ARGS__)

#define Error(format, ...) \
printf ("ERR|%s:%d: " format "\n", __func__, __LINE__, ##__VA_ARGS__)

#define Trace(format, ...) \
printf ("TRC|%s:%d: " format "\n", __func__, __LINE__, ##__VA_ARGS__)

#else

#define Info(...)
#define Error(...)
#define Trace(...)

#endif

#endif /* Log_H */
