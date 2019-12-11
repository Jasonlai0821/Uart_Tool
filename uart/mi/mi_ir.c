
#include "string.h"
#include "mi_common.h"
#include "mi_sys.h"
#include "internal/mi_sys_internal.h"
#include "mi_os.h"
#include <fcntl.h>
#include <linux/string.h>
#include <stdio.h>
#include <limits.h>
#include <linux/mman.h>
#include <unistd.h>
#include <sys/times.h>
#include <poll.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "mi_gpio.h"
#include "gpio.h"
#include "mi_ir.h"


#ifdef hxd019C_V1

#define I2CERR_NO_ERROR 0
#define  DELAY_TIME    1800//1050	//H: 25uS-45uS
#define  DELAY_ST  21 //18mS-25mS
#define  IR_BUF_LEN  232

#define REMOTE_DIN_PIN PAD_VID0  //(69)
#define REMOTE_CLK_PIN PAD_VID1  // (70)
#define REMOTE_BUSY_PIN PAD_PM_PDM_DIN //28
char ir_buf[IR_BUF_LEN];

//extern IRCallback MI_IR_Callback ;

/*******************************************
    SDA_PIN
*******************************************/
void Set_SDA_IO(void)
{
    /* Set this port to IO port of SDA */
    MI_GPIO_Init(REMOTE_DIN_PIN);
}

void SetSDAInput(void)
{
    /* It set SDA port as an input */
    MI_GPIO_SetInput(REMOTE_DIN_PIN);
}

void SetSDAOutput(void)
{
    /* It set SDA port as an output */
    //MI_GPIO_SetOutput(REMOTE_DIN_PIN, 0);
}

void SetSDAHigh(void)
{
    /* It output a High level to SDA here. */
    MI_GPIO_SetOutput(REMOTE_DIN_PIN, 1);
}

void SetSDALow(void)
{
    /* It output a Low level to SDA here. */
    MI_GPIO_SetOutput(REMOTE_DIN_PIN, 0);
}


UINT8 GetDINStatus(void)
{
    MI_GPIO_Level_e peLevel;
    MI_BOOL pbInvert;
    MI_GPIO_GetLevel(REMOTE_DIN_PIN, &peLevel, &pbInvert);
    return peLevel;
}

/*******************************************
    SCK_PIN  Í¬ÉÏ£¬²Î¿¼SDA
*******************************************/
void Set_SCL_IO(void)
{
    /* Set this port to IO port of SCL */
    MI_GPIO_Init(REMOTE_CLK_PIN);
}

void SetSCLOutput(void)
{
    /* It set SCL port as an output */
    MI_GPIO_SetOutput(REMOTE_CLK_PIN, 0);
}

void SetSCLHigh(void)
{
    /* It output a High level to SCL here. */
    MI_GPIO_SetOutput(REMOTE_CLK_PIN, 1);
}

void SetSCLLow(void)
{
    /* It output a Low level to SCL here. */
    MI_GPIO_SetOutput(REMOTE_CLK_PIN, 0);
}

/*******************************************
    BUSY_PIN  =============Í¬ÉÏ£¬²Î¿¼SDA
*******************************************/
void Set_BUSY_IO(void)
{
    /* Set this port to IO port of BUSY */
    MI_GPIO_Init(REMOTE_BUSY_PIN);
}

void SetBUSYInput(void)
{
    /* It set BUSY port as an input */
    MI_GPIO_SetInput(REMOTE_BUSY_PIN);
}

UINT8 GetBUSYStatus(void)
{
    MI_GPIO_Level_e peLevel;
    MI_BOOL pbInvert;
    MI_GPIO_GetLevel(REMOTE_BUSY_PIN, &peLevel, &pbInvert);
    return peLevel;
}


//=====================================================ÉÏµç³õÊ¼»¯»ò´ý»úÏà¹Ø£É£ÏÉèÖÃº¯Êý
void remote_poweron_init(void)
{
    Set_SDA_IO();
    SetSDAOutput();
    SetSDAHigh();

    Set_SCL_IO();
    SetSCLOutput();
    SetSCLHigh();

    Set_BUSY_IO();
    SetBUSYInput();
}

void DelayTime( UINT16 lTime )
{
    UINT16 i ;
    for(i=0;i<lTime;i++)
    {}
}
void RemoteDelayms(UINT16 data)
{
    MI_Delayms(data);
}
//===========

UINT8 GetACKSign(void)
{
    UINT8 ACKSign;
    UINT16 dataRead=0;

    SetSDAInput();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    SetSCLHigh();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    ACKSign = GetDINStatus();

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    SetSCLLow();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    return ACKSign;

}

void SendACKSign(void)
{
    SetSDAOutput();
    DelayTime(DELAY_TIME);
    SetSDALow(); ///wjs;ºó¼ÓµÍSDA_Low,Ã»ÓÐÑéÖ¤¹ý
    DelayTime(DELAY_TIME);

    SetSCLHigh(); //µÚ9´ÎµÄCLKµÄ¸ß
    DelayTime(DELAY_TIME); //;1mS

    SetSCLLow();

}

UINT16 I2COpen(void)
{
    Set_SDA_IO();
    Set_SCL_IO();

    //SetSDAOutput();
    SetSCLOutput();

    SetSCLHigh();
    SetSDAHigh();

    return I2CERR_NO_ERROR;
}

UINT16 I2CClose(void)
{
    SetSDAOutput();
    SetSCLOutput();

    SetSCLHigh();
    SetSDAHigh();

    return I2CERR_NO_ERROR;
}

UINT16 I2CStart(void)
{
    int i;
    UINT8 Data_Bit,ACKSign,TmpDat;
    Data_Bit = 8;

    SetSDAOutput();
    SetSCLOutput();
    SetSCLHigh();
    SetSDAHigh();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    SetSDALow();
    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    SetSCLLow();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);

    return 1;

}


UINT16 I2CStop(void)
{
    SetSDAOutput();
    SetSCLOutput();
    SetSCLLow();
    SetSDALow();
    DelayTime(DELAY_TIME);

    SetSCLHigh();
    DelayTime(DELAY_TIME);

    SetSDAHigh();
    DelayTime(DELAY_TIME);

    return I2CERR_NO_ERROR;
}

UINT16 I2CWriteData( char bData )
{
    UINT8 Data_Bit,ACKSign,TmpDat;
    int  i;	//kal_int8 i;

    SetSDAOutput();
    SetSCLLow();
    DelayTime(DELAY_TIME);

    for(i=7;i>=0;i--)
    {
        DelayTime(DELAY_TIME);
        Data_Bit=(bData>>i)& 0x01;

        if(Data_Bit)
        SetSDAHigh();
        else
        SetSDALow();

        DelayTime(DELAY_TIME);
        SetSCLHigh();
        DelayTime(DELAY_TIME);
        SetSCLLow();
    }
        ACKSign=GetACKSign();

        return ACKSign;
}

///urc_send
void writeI2C(char* data, UINT8 count)
{
    UINT8 i;
    UINT8 j = 0;
    char iBuffer;

    I2COpen();
    DelayTime(DELAY_TIME);

    SetSCLLow();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    SetSCLHigh();

    RemoteDelayms(DELAY_ST); //20ms

    I2CStart();
    DelayTime(DELAY_TIME);


    for(i = 0; i < count; i++) //count=7,³õÖµ
    {
        iBuffer = data[i];
        I2CWriteData(iBuffer);
        DelayTime(DELAY_TIME);
    }


    DelayTime(DELAY_TIME);

    I2CStop();
    DelayTime(DELAY_TIME);

    I2CClose();
    DelayTime(DELAY_TIME);

}

void Learn_start(void)
{
    I2COpen();
    DelayTime(DELAY_TIME);

    SetSCLLow();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    SetSCLHigh();

    RemoteDelayms(DELAY_ST); //20ms

    I2CStart();
    DelayTime(DELAY_TIME);

    I2CWriteData(0x30);
    DelayTime(DELAY_TIME);

    I2CWriteData(0x20);
    DelayTime(DELAY_TIME);

    I2CWriteData(0x50);
    DelayTime(DELAY_TIME);

    DelayTime(DELAY_TIME);

    I2CStop();
    DelayTime(DELAY_TIME);

    I2CClose();
    DelayTime(DELAY_TIME);

}

UINT16 I2CReadData(UINT8* pbData)
{
    UINT8 Data_Bit,ACKSign;
    UINT8 readdata = 0;
    UINT8 i=8;

    SetSDAInput();
    while (i--)
    {
        readdata<<=1;

        SetSCLHigh();
        DelayTime(DELAY_TIME);

        readdata |= GetDINStatus()?0x01:0x00;;  //¶Á²»µ½Ê±¿ÉÒÔÕâÑùÊÔ£ºreaddata |= GetDINStatus()?0x01:0x00;

        SetSCLLow();
        DelayTime(DELAY_TIME);
        DelayTime(DELAY_TIME); //wjs;¼Ódelay
    }
    SetSCLLow();
    DelayTime(DELAY_TIME);

    *pbData = readdata;

    SendACKSign();

    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME); //wjs;1GÒÔÉÏµÄÏµÍ³Òª¼Ó,test

    return I2CERR_NO_ERROR;
}

UINT16 readI2C(char* readtempbuf)     //UINT8
{
    UINT8 bValue;
    UINT8 i=0;
    UINT8 checksum;

    I2COpen();
    DelayTime(DELAY_TIME);

    SetSCLLow();
    DelayTime(DELAY_TIME);
    DelayTime(DELAY_TIME);
    SetSCLHigh();
    RemoteDelayms(DELAY_ST); //20ms

    I2CStart();
    DelayTime(DELAY_TIME);

    //----------------------------------------
    //write
    I2CWriteData(0x30);
    DelayTime(DELAY_TIME);
    //address point
    I2CWriteData(0x62);
    DelayTime(DELAY_TIME);

    //---------------------------------------
    //read
    I2CStart();
    DelayTime(DELAY_TIME);

    I2CWriteData(0x31);
    DelayTime(DELAY_TIME);

    I2CReadData(&bValue);    //wjs;read:FCS(1B)
    DelayTime(DELAY_TIME);    //wjs;1GÒÔÉÏµÄÏµÍ³Òª¼Ó

    i = 0;
    readtempbuf[i] = bValue;
    //printf( "remote_I2C_data[%d] = %x\n ",i,readtempbuf[i]);
    checksum = 0xc3;

    for(i = 1; i < (IR_BUF_LEN-2); i++)         //wjs;read:learndata(109B)+120=230
    {
        I2CReadData(&bValue);
        DelayTime(DELAY_TIME);
        readtempbuf[i] = bValue;
        //printf( "remote_I2C_data[%d] = %x\n",i,readtempbuf[i]);
        checksum += bValue;
    }

    I2CReadData(&bValue);
    DelayTime(DELAY_TIME);

    I2CStop();
    DelayTime(DELAY_TIME);
    I2CClose();
    DelayTime(DELAY_TIME);

    return 1;
}

void* mi_ir_learn_thread(void * arg)
{
    unsigned int currentTime ;
    remote_poweron_init();
    Learn_start();
    RemoteDelayms(50);

    while(1)
    {
        if(GetBUSYStatus()==1)
        {   //currentTime=MI_Get_SystemTime_ms();
            readI2C(ir_buf) ;
            //printf("ir read end  ,cost time0 =%d\n  ",MI_Get_SystemTime_ms()-currentTime);
            //if(MI_IR_Callback)
            //MI_IR_Callback(ir_buf);
            //printf("Learn_start again ,cost time1 =%d\n  ",MI_Get_SystemTime_ms()-currentTime);
            Learn_start();
            RemoteDelayms(30);
            //printf("Learn_start again ,cost time2 =%d\n  ",MI_Get_SystemTime_ms()-currentTime);
            currentTime=MI_Get_SystemTime_ms();
        }

        if(((MI_Get_SystemTime_ms()-currentTime)>15*1000)&&(GetBUSYStatus()==0))  //10 s timeout
        {
            Learn_start();
            RemoteDelayms(30);
            currentTime=MI_Get_SystemTime_ms();
        }
    }

}

int mi_ir_init(void)
{
    pthread_t IR_ReceiveThread ;
    pthread_attr_t attr;

    // Set pthread attribute and ready to start threads
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(pthread_create(&IR_ReceiveThread, NULL, mi_ir_learn_thread, NULL))
    {
        printf("fail to pthread_create IR_ReceiveThread \n");
        return -1;
    }
    pthread_attr_destroy(&attr);
    if(pthread_join(IR_ReceiveThread, NULL) != 0) {
        printf("fail tho recollect IR_ReceiveThread \n");
        return -1;
    }
}

int mi_ir_write_test(void)
{
    int i;
    unsigned char ck = 0x00;
    unsigned char buf_232[232];
    unsigned char ir_buf[]={0x0,0x52,0x47,0x0,0x34,0x17,0x1,0x2d,0x2,0x21,0x0,0x23,0x0,0x66,0x0,0x1d,0x0,0xc8,0x9,0x23,0x0,0x8a,0x0,0x32,0x2,0x0,0x0,0x1f,0x0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x1,0x11,0x21,0x11,0x12,0x22,0x12,0x22,0x21,0x12,0x22,0x11,0x12,0x21,0x11,0x22,0x23,0x45,0xf0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
            0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
            0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x5,0x76,0x1};
    unsigned int len = 232;
    printf("mi_ir_write_test\n  ");

    remote_poweron_init();
    RemoteDelayms(50);

    if (ir_buf[1] == 0xFF)
    { //ir_buf[]=buf230[],???230???
        ir_buf[1]=0xFe;
        ir_buf[2]=0xFE;
        ir_buf[226]=0xFe;

        for (i=38; i < 216; i++)
        {
            if (ir_buf[i] == 00 && ir_buf[i+1] == 00 && ir_buf[i+2] == 00 && ir_buf[i+3] == 00 && ir_buf[i+4] == 00 && ir_buf[i+5] == 00 && ir_buf[i+6] == 00 && ir_buf[i+7] == 00 && ir_buf[i+8] == 00)
            {
                ir_buf[i]=0xFE;
                i=216;
            };

        }
    }

    buf_232[0] = 0x30;
    ck += buf_232[0];
    buf_232[1] = 0x03;
    ck += buf_232[1];
    i = 1;
    for (; i < 230; i++){
        buf_232[i + 1] = ir_buf[i];
        ck += ir_buf[i];
    }
    buf_232[231] = ck;

    writeI2C(buf_232, len);
    return 0;

}

#endif


