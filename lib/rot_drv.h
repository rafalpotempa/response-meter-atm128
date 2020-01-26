#ifndef ROT_DRV_H
#define ROT_DRV_H

//
#define ROT_IN PINE
#define ROT_CTRL DDRE
#define ROT_OUT PORTE

#define SW_A 0
#define SW_B 1
#define SW_PB 2

#define ROT_RIGHT 1
#define ROT_LEFT 2
#define ROT_PB 3

unsigned char rotEnc();

#endif
