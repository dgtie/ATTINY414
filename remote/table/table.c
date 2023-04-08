#include "../table.h"

//    SH ^  on      SH 0 in
//     < ok >        1 2 3
//    v+ v  c+       4 5 6
//    v- mu c-       7 8 9

//    ^   <   ok  >   v+  v   c+  v-  mu  c-  on
// SH 0   1   2   3   4   5   6   7   8   9   in
//    nul adr protocol

#define RC6 (void(*)(char*,char,char))(0x3A2>>1)
#define SONY20 (void(*)(char*,char,char))(0x3E8>>1)
#define NEC (void(*)(char*,char,char))(0x4C2>>1)

const unsigned char extend[8] = { FF, 226, FF, FF, FF, FF, FF, FF };
const struct Device devices[3] = {
  { { 88, 90, 92, 91, 16, 89, 63, 17, 13, 84, 12,	// 0
//                            source      menu
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, FF },	// PHILIPS DVD PLAYER
      FF, 4, RC6 },
  { { 57, 59, 61, 60, FF, 58, FF, FF, FF, FF, 21,	// 2
      FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF },	// SONY BLU-RAY
      FF, 26, SONY20 },
  { { FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF,	// 3 - blank entry
      FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF },
      FF, FF, (void(*)(char*,char,char))0xFFFF }
};
