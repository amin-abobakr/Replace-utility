#ifndef __UTILS__
#define __UTILS__
#define RET_OK 0
#define RET_KO -1
//OPT Flags
#define b  1
#define f  2
#define l  4
#define i  8
#define StrnCmp(dest,Src,size) strncmp((const char*)dest,(const char*)Src,size)
#define StrCpy(dest,Src) strcpy((char*)dest,(const char*)Src)
#define StrStr(dest,Src) stristr((const uint8_t*)dest,(const uint8_t*)Src)

//prototypes//
uint8_t* stristr( const uint8_t* str1, const uint8_t* str2 );
#endif // __UTILS__
