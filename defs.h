
#ifndef Defs_H
#define Defs_H

#define Array_Count(arr) (int)(sizeof (arr) / sizeof ((arr)[0]))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) < (b) ? (b) : (a))
#define Clamp(v, a, b) (Max (Min (v, b), a))
#define Absolute(v) (v < 0 ? -v : v)
#define Lerp(a, b, t) ((b) - (a)) * (t) + (a)
#define Assert(cond) (void)((cond) ? 1 : (*((volatile int *)0) = 0))

#endif /* Defs_H */
