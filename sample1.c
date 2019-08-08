/* dumped in big endian order.
   use `mrbc -e` option for better performance on little endian CPU. */
#include <stdint.h>
extern const uint8_t sample1[];
const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
sample1[] = {
0x52,0x49,0x54,0x45,0x30,0x30,0x30,0x34,0x9d,0x71,0x00,0x00,0x00,0x92,0x4d,0x41,
0x54,0x5a,0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x74,0x30,0x30,
0x30,0x30,0x00,0x00,0x00,0x6c,0x00,0x01,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x11,
0x00,0x40,0x06,0x17,0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,0x00,0x80,0x00,0xa0,
0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,0x00,0x80,0x40,0xa0,0x00,0x80,0x00,0x06,
0x01,0x3f,0xff,0x83,0x00,0x80,0x00,0xa0,0x00,0x80,0x00,0x06,0x01,0x40,0x00,0x03,
0x00,0x80,0x40,0xa0,0x00,0x80,0x00,0x07,0x00,0xbf,0xf9,0x18,0x00,0x80,0x00,0x05,
0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x03,0x72,0x62,
0x35,0x00,0x00,0x05,0x73,0x6c,0x65,0x65,0x70,0x00,0x45,0x4e,0x44,0x00,0x00,0x00,
0x00,0x08,
};
