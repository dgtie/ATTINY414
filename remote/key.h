//        P P G
//        B B N
//        0 1 D
//        _____
//  PA3  |9 8 7|   01000000 10000000 11000000    0x40 0x80 0xC0
//  PA2  |6 5 4|   00010000 00100000 00110000    0x10 0x20 0x30
//  PA1  |3 2 1|   00000100 00001000 00001100    0x04 0x08 0x0C
//  PA6  |P 0 S|   00000001 00000010 00000011    0x01 0x02 0x03
//        -----
//  IR    U   U

#define KEY_0 0x02		// index = 0
#define KEY_1 0x0C		// index = 1
#define KEY_2 0x08		// index = 2
#define KEY_3 0x04		// index = 3
#define KEY_4 0x30		// index = 4
#define KEY_5 0x20		// index = 5
#define KEY_6 0x10		// index = 6
#define KEY_7 (char)0xC0	// index = 7
#define KEY_8 (char)0x80	// index = 8
#define KEY_9 0x40		// index = 9
#define KEY_POWER 0x01		// index = 10
#define KEY_SHIFT 0x03		// index = 11
#define KEY_UP    0x00		// index = 12 (including invalid keys)

void key_scan(void), key_init(void);
char key_index(char), get_key(void);
