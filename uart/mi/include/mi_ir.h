
#include "mi_gpio.h"
#include "gpio.h"
//hxd019C_ V1
#define hxd019C_V1

#ifdef hxd019C_V1

#ifndef UINT8
typedef unsigned char UINT8;
#endif

#ifndef UINT16
typedef unsigned short UINT16;
#endif

typedef int (*IRCallback)(char *);

void remote_poweron_init(void);
UINT16 I2COpen(void);
UINT16 I2CClose(void);
UINT16 I2CStart(void);
UINT16 I2CStop(void);
UINT16 I2CWriteData( char bData );
void writeI2C(char* data, UINT8 count);
void Learn_start(void);
UINT16 I2CReadData(UINT8* pbData);
UINT16 readI2C(char* readtempbuf);
void* mi_ir_learn_thread(void * arg);
int mi_ir_init(void);
int mi_ir_write_test(void);

#endif


