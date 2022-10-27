#ifndef _DEBUG_H_INCLUDED
#define _DEBUG_H_INCLUDED

#ifdef DEBUG

void dbg_print(const char *str);

#else

#define dbg_print(str)

#endif

#endif // _DEBUG_H_INCLUDED