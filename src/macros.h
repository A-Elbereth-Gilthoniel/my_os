#ifndef MACROS
#define MACROS
typedef unsigned char uint8_t;
typedef char int8_t;

#if (defined(__SIZEOF_SHORT__) && (__SIZEOF_SHORT__ == 2))
typedef unsigned short uint16_t;
typedef short int16_t;
#else
typedef unsigned int uint16_t;
typedef int int16_t;
#endif

#if defined(__SIZEOF_INT__) && (__SIZEOF_INT__ == 4)
typedef unsigned int uint32_t;
typedef int int32_t;
#else
typedef unsigned long uint32_t;
typedef long int32_t;
#endif

typedef uint32_t size_t;

#define WIDTH 80
#define HEIGHT 25
#define VGA_MEMORY 0xB8000
#endif
