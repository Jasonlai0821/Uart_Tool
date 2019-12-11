/*
 * SerialCommand.h
 *
 *  Created on: 2017-11-29
 *      Author: Administrator
 */

#ifndef SERIALCOMMAND_H_
#define SERIALCOMMAND_H_
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/Mutex.h>
#include "MyUtils.h"
using namespace android;
namespace soundbar {

#define COM0       0
#define COM1       1
#define COM2       2
#define COM3       3

//
#define NOW_COM 	COM0

#define W_TIMEOUTMS	400		// millisecond
#define R_TIMEOUTMS	400		// millisecond
#define B_TIMEOUTMS	1400    // (millisecond)the maxinum of response time after 'write'
#define MAXlength_serwritedata	4
#define MINlength_serwritedata	4

#define         A_HDR1          0
#define         A_TAddr         1
#define         A_COMD          2
#define         A_CHECK         3

#define			D_HEAD			0x7A

#define         ADD_rk3229     	0x01
#define         ADD_speaker    	0x02

#define		CMD_GETVOL		0xA1
#define		CMD_SETVOL		0xA2
#define		CMD_SETMUSIC	0xA3
#define		CMD_BOOTING		0xA4
#define		CMD_CONING		0xA5
#define		CMD_CONFAIL		0xA6
#define		CMD_CONSECC		0xA7
#define		CMD_AFTWAKE		0xA8
#define		CMD_SERCHING	0xA9
#define		CMD_PLAYING		0xAA
#define		CMD_ALARM		0xAB
#define		CMD_UPDATE		0xAC
#define		CMD_UPFAIL		0xAD
#define		CMD_SETNETCOF	0xAE
#define		CMD_SETNEWS		0xAF
#define		CMD_SETDEF		0xB0
#define		CMD_SETMOV		0xB1
typedef enum {
	MUTE_off, MUTE_on
} para_volume_e;

typedef enum {
	SERCMD_unknown = 0,
	SERCMD_getvolume,
	SERCMD_setvolume,
	SERCMD_setmusmode,
	SERCMD_booting,
	SERCMD_connecting,
	SERCMD_connectfail,
	SERCMD_connectsecc,
	SERCMD_afterwakeup,
	SERCMD_searching,
	SERCMD_playing,
	SERCMD_alarm,
	SERCMD_updating,
	SERCMD_updatefail,
	SERCMD_setnewsmode,
	SERCMD_setmovmode,
	SERCMD_setnetworkconf,
	SERCMD_setdefault,
} ser_cmd_e;
typedef enum {
	SER_musmode = 0, SER_newsmode, SER_movmode
} ser_soundmode_e;

typedef struct {
	ser_cmd_e ser_cmd;
	unsigned char ser_data;
	para_volume_e ser_mutedata;
} ser_data_s;

class SerialCommand: virtual public RefBase {
public:
	SerialCommand();
	virtual ~SerialCommand();
	int iniSer();
	int deInitSer();
	int onBooting();
	int onNetworkConnecting();
	int onNetworkConnectionSuccess();
	int onNetworkConnectionFailed();
	int onWakeup();
	int onSerchingOrLoading();
	int onPlaying();
	int onAlarm();
	int onSysUpgrading();
	int onSysUpgradeFailed();
	int onSetVolume(int i4Volume);
	int onGetVolume();
	int onSetSoundMode(ser_soundmode_e i4SoundMode);
	void setListener(const sp<SkyDisplayListener>& listener);
	sp<SkyDisplayListener> getListener() {
		return mListener;
	}

	//static function as tools
	static int InitSerPort(int PortNo);
	static int SerOpen(int PortNo);
	static int OpenCom(int Comm, int baud, unsigned char databits,
			unsigned char stopbits, unsigned char parity);
	static int SetArrt(int Comm, int baud, unsigned char databits,
			unsigned char stopbits, unsigned char parity);
	static void CloseSerPort(int PortNo);
	static void CloseComm(int Comm);
	static int SerWrite_rk3229_t(int PortNo, ser_data_s *w_pBuf,
			unsigned long TimeOut);
	static int SerRead_rk3229(int PortNo, ser_data_s *pBuf,
			unsigned long TimeOut);
	static void SerClear(int PortNo);
	static int ReadPort(int Comm, unsigned char *buffer, int nMaxLength,
			int TimeOutMs);
	static int SerWrite(int PortNo, unsigned char *pszBuf, unsigned int SendCnt,
			unsigned long TimeOut);
	static int WritePort(int Comm, unsigned char *buffer, int dwBytesToWrite,
			int TimeOutMs);
	static int ReadPortSerWrite(int PortNo, unsigned char *pszBuf,
			unsigned int SendCnt, unsigned long TimeOut);
	static int SerParse_rk3229_t(int PortNo, ser_data_s *pBuf,
			unsigned char* pszBuf);
	static void SerClose(int PortNo);
	static int SerRead(int PortNo, unsigned char *pszBuf, int ReadCnt,
			unsigned long TimeOutMS);
	static sp<SerialCommand> getInstance();
private:
	SerialCommand(const SerialCommand&);
	SerialCommand& operator=(const SerialCommand&);
	static SerialCommand* mInstance;
	sp<SkyDisplayListener> mListener;
};

} // namespace RK3229

#endif /* SERIALCOMMAND_H_ */
