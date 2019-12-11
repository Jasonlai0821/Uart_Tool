/*
 * SerialCommand.cpp
 *
 *  Created on: 2017-12-5
 *      Author: Administrator
 */
#define LOG_TAG "SerialCommand"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <termios.h>
#include <fcntl.h>
#include <pthread.h>
#include <syslog.h>
#include <signal.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <utils/StrongPointer.h>
#include "SerialCommand.h"
#include "MyUtils.h"
namespace soundbar {

#define INVALID_HANDLE_VALUE	-1
#define VOLUME_NUM 7
#define DEFAULT_COMM_PORT "/dev/ttyS0"
#define MAX_PACKET_LEN (12)

static int m_serial_handle;
//static sem_t m_singal;
static pthread_mutex_t m_mutex;
static pthread_cond_t m_cond;

static ser_cmd_e Now_cmd_e;
static ser_data_s S_Read;
static unsigned char FLAG_READ_THREAD = 0; /*0: running ,1: need to exit, 2: exited*/

//only for test
static int testFun(ser_data_s* data) {
	data->ser_cmd = static_cast<ser_cmd_e>(rand() % 17);
	data->ser_data = rand() % 25 + 97;
	data->ser_mutedata = static_cast<para_volume_e>(rand() % 2);
	LOGD(
			"data->ser_cmd = %d, data->ser_data = %c, data->ser_mutedata = %d", data->ser_cmd, data->ser_data, data->ser_mutedata);
	return 0;
}

SerialCommand* SerialCommand::mInstance = NULL;

SerialCommand::SerialCommand() :
		mListener(NULL) {
	LOGD("SerialCommand Constructor");
	mInstance = this;
}

SerialCommand::~SerialCommand() {
	LOGD("SerialCommand Destructor");
	mInstance = NULL;
	mListener = NULL;
}

sp<SerialCommand> SerialCommand::getInstance() {
	if (mInstance != NULL) {
		return mInstance;
	}
	return NULL;
}

void SerialCommand::setListener(const sp<SkyDisplayListener>& listener) {
	mListener = listener;
}
int SerialCommand::SerOpen(int PortNo) {
	int ret;
	struct termios options;

	m_serial_handle = open(DEFAULT_COMM_PORT, O_RDWR|O_NOCTTY|O_NDELAY);
	if (m_serial_handle == INVALID_HANDLE_VALUE) {
		LOGE("Open file err:%s errno:%d", DEFAULT_COMM_PORT, errno);
		return -1;
	}

//	tcgetattr(m_serial_handle, &options);

	bzero(&options,sizeof(struct termios));
	options.c_lflag = 0;
	options.c_iflag &= ~(INLCR | ICRNL | IGNCR);
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_oflag &= ~(ONLCR | OCRNL);
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 0;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //rockchip add

	/*data bits*/
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag |= (CLOCAL | CREAD); //rockchip modify
	/*parity*/
	options.c_cflag &= ~PARENB; /* è¾“å‡ºä¸è¿›è¡Œå¥‡å¶æ ¡éªŒ */
	options.c_iflag &= ~INPCK; /* è¾“å…¥ä¸è¿›è¡Œå¥‡å¶æ ¡éªŒ */
	/*stop bits*/
	options.c_cflag &= ~CSTOPB;

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	ret = tcsetattr(m_serial_handle, TCSANOW, &options);
	if (ret == -1) {
		LOGE("SetArrt tcsetattr err: %d", errno);
		return -1;
	}
	return 0;
}

void SerialCommand::SerClose(int PortNo) {
	if (m_serial_handle != INVALID_HANDLE_VALUE) {
		close(m_serial_handle);
	}
	m_serial_handle = INVALID_HANDLE_VALUE;
}

void SerialCommand::SerClear(int PortNo) {
	LOGV("SerClear: clear the data of uart....");
	tcflush(m_serial_handle, TCIOFLUSH);
}

int SerialCommand::SerWrite(int PortNo, unsigned char *pszBuf,
		unsigned int SendCnt, unsigned long TimeOutMS) {
	int ret = 0;
	struct timeval CurTime;
	fd_set wwrite;

	FD_ZERO(&wwrite);
	FD_SET(m_serial_handle, &wwrite);

	CurTime.tv_sec = (TimeOutMS / 1000);
	CurTime.tv_usec = (TimeOutMS % 1000) * 1000;

	ret = select(m_serial_handle + 1, NULL, &wwrite, NULL, &CurTime);
	if (ret == 0) {
		LOGE("SerWrite select timeout");
		return -1;
	} else if (ret < 0) {
		LOGE("SerWrite select error: %d", errno);
		return -1;
	}

	ret = write(m_serial_handle, pszBuf, SendCnt);
	if (ret <= 0) {
		SerClear(NOW_COM);
		LOGE("SerWrite write buffer error:%d ", errno);
		return -1;
	}
	return 0;
}

int SerialCommand::SerRead(int PortNo, unsigned char *pszBuf, int ReadCnt,
		unsigned long TimeOutMS) {
	int ret = 0;
	int index = 0;
	struct timeval CurTime;
	fd_set wread;

	FD_ZERO(&wread);
	FD_SET(m_serial_handle, &wread);

	CurTime.tv_sec = (TimeOutMS / 1000);
	CurTime.tv_usec = (TimeOutMS % 1000) * 1000;

	ret = select(m_serial_handle + 1, &wread, NULL, NULL, &CurTime);
	if (ret == 0) {
		return -1;
	} else if (ret < 0) {
		LOGE("SerRead select error: %d", errno);
		return -1;
	}

	ret = read(m_serial_handle, pszBuf, ReadCnt);
	if (ret <= 0) {
		LOGE("SerRead read buffer error:%d ", errno);
		return -1;
	}
	return ret;
}

int SerialCommand::SerWrite_rk3229_t(int PortNo, ser_data_s *w_pBuf,
		unsigned long TimeOut) {
	unsigned int data_crc16 = 0;
	unsigned char pszBuf[4] = { 0 }; //[4]
	unsigned char highByte, lowByte;
		char buffer[MAX_PACKET_LEN + 1];
		int ret , i;

	LOGV(
			"SerWrite_rk3229: PortNo=%d ser_cmd=%x ser_data=%d", PortNo, w_pBuf->ser_cmd, w_pBuf->ser_data);
//	pszBuf = malloc(MAXlength_serwritedata);

	pszBuf[A_HDR1] = D_HEAD;
	pszBuf[A_TAddr] = ADD_speaker;

	switch (w_pBuf->ser_cmd) {
	case SERCMD_getvolume:
		pszBuf[A_COMD] = CMD_GETVOL;
		break;
	case SERCMD_setvolume:
		pszBuf[A_TAddr] = CMD_SETVOL;
		pszBuf[A_COMD] = w_pBuf->ser_data;
		break;
	case SERCMD_setmusmode:
		pszBuf[A_COMD] = CMD_SETMUSIC;
		break;
	case SERCMD_booting:
		pszBuf[A_COMD] = CMD_BOOTING;
		break;
	case SERCMD_connecting:
		pszBuf[A_COMD] = CMD_CONING;
		break;
	case SERCMD_connectfail:
		pszBuf[A_COMD] = CMD_CONFAIL;
		break;
	case SERCMD_connectsecc:
		pszBuf[A_COMD] = CMD_CONSECC;
		break;
	case SERCMD_afterwakeup:
		pszBuf[A_COMD] = CMD_AFTWAKE;
		break;
	case SERCMD_searching:
		pszBuf[A_COMD] = CMD_SERCHING;
		break;
	case SERCMD_playing:
		pszBuf[A_COMD] = CMD_PLAYING;
		break;
	case SERCMD_alarm:
		pszBuf[A_COMD] = CMD_ALARM;
		break;
	case SERCMD_updating:
		pszBuf[A_COMD] = CMD_UPDATE;
		break;
	case SERCMD_updatefail:
		pszBuf[A_COMD] = CMD_UPFAIL;
		break;
	case SERCMD_setnewsmode:
		pszBuf[A_COMD] = CMD_SETNEWS;
		break;
	case SERCMD_setmovmode:
		pszBuf[A_COMD] = CMD_SETMOV;
		break;

	default:
		LOGE("SerWrite data is invalid");
		break;
	}

	pszBuf[A_CHECK] = pszBuf[A_TAddr] ^ pszBuf[A_COMD];
//	LOGV("SerWrite: pszBuf=%02x%02x%02x%02x",*pszBuf, *(pszBuf+1), *(pszBuf+2), *(pszBuf+3) );
	LOGV(
			"SerWrite_rk3229: pszBuf=%02x%02x%02x%02x", pszBuf[A_HDR1], pszBuf[A_TAddr], pszBuf[A_COMD], pszBuf[A_CHECK]);
#if 0
	memset(buffer, 0x0, MAX_PACKET_LEN + 1);
    for (i = 0; i < MAXlength_serwritedata; i++)
    {
        highByte = pszBuf[i] >> 4;
        lowByte = pszBuf[i] & 0x0f ;

        highByte += 0x30;
        if (highByte > 0x39)
        	buffer[i * 3] = highByte + 0x07;
        else
            buffer[i * 3] = highByte;

        lowByte += 0x30;
        if (lowByte > 0x39)
            buffer[i * 3 + 1] = lowByte + 0x07;
        else
            buffer[i * 3 + 1] = lowByte;

		buffer[i * 3 + 2] = 0x20;
    }
    LOGV("SerWrite_rk3229: buffer: %s", buffer);
#endif
	ret = SerWrite(PortNo, &pszBuf[0], MAXlength_serwritedata, TimeOut);
//    ret = SerWrite(PortNo,(unsigned char*)buffer,MAX_PACKET_LEN,TimeOut);
	if (ret < 0) {
		LOGE("SerWrite err");
		return -1;
	} else {
		return 0;
	}
}

int SerialCommand::SerParse_rk3229_t(int PortNo, ser_data_s *pBuf,
		unsigned char* pszBuf) {
	LOGV(
			"SerParse_rk3229: pszBuf=%02x%02x%02x%02x", pszBuf[A_HDR1], pszBuf[A_TAddr], pszBuf[A_COMD], pszBuf[A_CHECK]);
	if (pszBuf[A_CHECK] == (pszBuf[A_TAddr] ^ pszBuf[A_COMD])) {
		if (D_HEAD == pszBuf[A_HDR1]) {
			if (ADD_rk3229 == pszBuf[A_TAddr]) {
				switch (pszBuf[A_COMD]) {
				case CMD_SETMUSIC:
					pBuf->ser_cmd = SERCMD_setmusmode;
					pBuf->ser_data = 0;
					break;
				case CMD_BOOTING:
					pBuf->ser_cmd = SERCMD_booting;
					pBuf->ser_data = 0;
					break;
				case CMD_CONING:
					pBuf->ser_cmd = SERCMD_connecting;
					pBuf->ser_data = 0;
					break;
				case CMD_CONFAIL:
					pBuf->ser_cmd = SERCMD_connectfail;
					pBuf->ser_data = 0;
					break;
				case CMD_CONSECC:
					pBuf->ser_cmd = SERCMD_connectsecc;
					pBuf->ser_data = 0;
					break;
				case CMD_AFTWAKE:
					pBuf->ser_cmd = SERCMD_afterwakeup;
					pBuf->ser_data = 0;
					break;
				case CMD_SERCHING:
					pBuf->ser_cmd = SERCMD_searching;
					pBuf->ser_data = 0;
					break;
				case CMD_PLAYING:
					pBuf->ser_cmd = SERCMD_playing;
					pBuf->ser_data = 0;
					break;
				case CMD_ALARM:
					pBuf->ser_cmd = SERCMD_alarm;
					pBuf->ser_data = 0;
					break;
				case CMD_UPDATE:
					pBuf->ser_cmd = SERCMD_updating;
					pBuf->ser_data = 0;
					break;
				case CMD_UPFAIL:
					pBuf->ser_cmd = SERCMD_updatefail;
					pBuf->ser_data = 0;
					break;
				case CMD_SETNEWS:
					pBuf->ser_cmd = SERCMD_setnewsmode;
					pBuf->ser_data = 0;
					break;
				case CMD_SETMOV:
					pBuf->ser_cmd = SERCMD_setmovmode;
					pBuf->ser_data = 0;
					break;
				case CMD_SETNETCOF:
					pBuf->ser_cmd = SERCMD_setnetworkconf;
					pBuf->ser_data = 0;
					break;
				case CMD_SETDEF:
					pBuf->ser_cmd = SERCMD_setdefault;
					pBuf->ser_data = 0;
					break;
				default:
					SerClear(PortNo);
					LOGE("SerRead_rk3229 A_COMD is invalid");
					break;

				}
			} else {
				switch (pszBuf[A_TAddr]) {
				case CMD_SETVOL:
					pBuf->ser_cmd = SERCMD_setvolume;
					break;
				case CMD_GETVOL:
					pBuf->ser_cmd = SERCMD_getvolume;
					break;
				default:
					break;
				}
				pBuf->ser_data = pszBuf[A_COMD] & 0x7f;
				if (0 == (pszBuf[A_COMD] & 0x80)) {
					pBuf->ser_mutedata = MUTE_off;
				} else {
					pBuf->ser_mutedata = MUTE_on;
				}
			}
		} else {
			LOGE("SerRead_rk3229 A_HDR1 is invalid");
			SerClear(PortNo);
			return -1;
		}
	} else {
		LOGE("SerRead_rk3229 check is err");
		SerClear(PortNo);
		return -1;
	}
	return 0;
}

//#define MAX_PACKET_LEN (32)
static void* thread_read_sercom_fun(void *arg)
{
	int ret = 0;
	int n_read_bytes = 0;
	int n_store_bytes = 0;
	ser_data_s ser_data;
	unsigned char pszBuf[MAXlength_serwritedata];	//[4]
	unsigned char pReponse[MAX_PACKET_LEN + 1];
	LOGV("thread_read_sercom_fun IN");
	memset(pReponse, 0x0, MAX_PACKET_LEN+1);
	while (1)// var
	{
		int i = 0;
		int n_trim_byte = 0;
		if (1 == FLAG_READ_THREAD){
			LOGD("thread_read_sercom_fun exit thread");
			break;
		}

		memset(&ser_data, 0x0, sizeof(ser_data_s));
		n_read_bytes = SerialCommand::SerRead(NOW_COM, pReponse + n_store_bytes, MAX_PACKET_LEN - n_store_bytes, R_TIMEOUTMS);
		if (n_read_bytes <= 0){
			if (n_store_bytes <= 0){
				continue;
			}
		} else {
			n_store_bytes += n_read_bytes;
		}


		/* check if one packet ready */
		memset(pszBuf, 0x0, sizeof(pszBuf));
		for (i = 0; i < n_store_bytes; i++){
			unsigned char value = 0;
			if (pReponse[i] == 0x20){
				n_trim_byte++;
				continue;
			}

			if (n_trim_byte >= MAXlength_serwritedata){
				LOGV("PACKET PARSE OK, n_trim_byte: %d, n_store_bytes: %d", n_trim_byte,n_store_bytes);
				break;
			}
			if (pReponse[i] >= 'a' && pReponse[i] <= 'f'){
				value = (pReponse[i] - 'a' + 10);
			}else if (pReponse[i] >= 'A' && pReponse[i] <= 'F'){
				value = (pReponse[i] - 'A' + 10);
			}else {
				value = (pReponse[i] - '0');
			}

			pszBuf[n_trim_byte] = (pszBuf[n_trim_byte] << 4) | value;
		}

		if (n_trim_byte < MAXlength_serwritedata){
			if (n_store_bytes >= MAX_PACKET_LEN){
				n_store_bytes = 0;
				SerialCommand::SerClear(NOW_COM);
				LOGE("thread_read_sercom_fun packet invalid");
			}
			LOGV("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x",
							pReponse[0],pReponse[1],pReponse[2],pReponse[3],pReponse[4],pReponse[5],
							pReponse[6],pReponse[7],pReponse[8],pReponse[9],pReponse[10],pReponse[11]);
			continue;
		}
		n_store_bytes = 0;
		memset(pReponse, 0x0, MAX_PACKET_LEN+1);

		ret = SerialCommand::SerParse_rk3229_t(NOW_COM, &ser_data, pszBuf);
		if (ret == -1) {
			LOGE("SerParse_rk3229 get invalid packet");
			continue;
		}

		if (Now_cmd_e == ser_data.ser_cmd)
		{
			if (Now_cmd_e != SERCMD_unknown){
				S_Read.ser_cmd = ser_data.ser_cmd;
				S_Read.ser_data = ser_data.ser_data;
				S_Read.ser_mutedata = ser_data.ser_mutedata;

				pthread_cond_signal(&m_cond);
			}
		}
		else
		{
			LOGV("thread_read_sercom_fun: call 'registerCallback'");
			// call the registerCallback
			//only for debug
			//ser_data_s ser_data;
			//testFun(&ser_data);
			sp<SkyDisplayListener> listener = SerialCommand::getInstance()->getListener();
			if(listener != NULL){
				listener->notify(ser_data.ser_cmd,ser_data.ser_data,ser_data.ser_mutedata,NULL);
			}
		}
		usleep(1000);
	}

	FLAG_READ_THREAD = 2;
	return (void*) 0;
}

int SerialCommand::iniSer() {
	LOGD("Enter iniSer()");
	int ret = 0;
	pthread_t tid;
	pthread_mutexattr_t ma;

	ret = SerOpen(NOW_COM);
	if (ret < 0) {
		LOGE("iniSer->Seropen Failed");
		return -1;
	}
	SerClear(NOW_COM);
	LOGE("iniSer 1111111111111");
	pthread_mutexattr_init(&ma);
	pthread_mutexattr_settype(&ma, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &ma);
	pthread_mutexattr_destroy(&ma);
	LOGE("iniSer 22222222");

	pthread_cond_init(&m_cond, NULL);
	LOGE("iniSer 3333333");
	Now_cmd_e = SERCMD_unknown;
	memset(&S_Read, 0x0, sizeof(S_Read));

	FLAG_READ_THREAD = 0; /*thread running flag*/
	if (pthread_create(&tid, NULL, thread_read_sercom_fun, NULL) != 0) {
		LOGE("iniSer pthread_create failed");
		return -1;
	}
	LOGD("Leave iniSer()");
	return 0;
}

int SerialCommand::deInitSer() {
	LOGD("Enter deInitSer()");
	FLAG_READ_THREAD = 1;
	while (1) {
		if (FLAG_READ_THREAD == 2) {
			LOGV("deInitSer serial read thread exited");
			break;
		}
		usleep(100000);
	}
	SerClose(NOW_COM);

	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
	LOGD("Leave deInitSer()");
	return 0;
}

int SerialCommand::onBooting() {
	LOGD("Enter onBooting()");
	ser_data_s s_write;
	int ret;

	s_write.ser_cmd = SERCMD_booting;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onBooting:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onBooting:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onBooting: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onBooting()");
}

int SerialCommand::onNetworkConnecting() {
	LOGD("Enter onNetworkConnecting()");
	ser_data_s s_write;
	int ret;

	s_write.ser_cmd = SERCMD_connecting;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {

		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onNetworkConnecting:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onNetworkConnecting:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onNetworkConnecting: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onNetworkConnecting()");
}

int SerialCommand::onNetworkConnectionSuccess() {
	LOGD("Enter onNetworkConnectionSuccess()");
	ser_data_s s_write;
	int ret;

	s_write.ser_cmd = SERCMD_connectsecc;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onNetworkConnectionSuccess:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onNetworkConnectionSuccess:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV(" onNetworkConnectionSuccess: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onNetworkConnectionSuccess()");
}

int SerialCommand::onNetworkConnectionFailed() {
	LOGD("Enter onNetworkConnectionFailed()");
	ser_data_s s_write;
	int ret;

	s_write.ser_cmd = SERCMD_connectfail;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onNetworkConnectionFailed:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onNetworkConnectionFailed:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onNetworkConnectionFailed: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onNetworkConnectionFailed()");
}

int SerialCommand::onWakeup() {
	LOGD("Enter onWakeup()");
	ser_data_s s_write;
	int ret;

	s_write.ser_cmd = SERCMD_afterwakeup;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onWakeup:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onWakeup:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onWakeup: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onWakeup()");
}

int SerialCommand::onSerchingOrLoading() {
	LOGD("Enter onSerchingOrLoading()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_searching;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onSerchingOrLoading:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onSerchingOrLoading:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onSerchingOrLoading: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onSerchingOrLoading()");
}

int SerialCommand::onPlaying() {
	LOGD("Enter onPlaying()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_playing;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onPlaying:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onPlaying:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onPlaying: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onPlaying()");
}

int SerialCommand::onAlarm() {
	LOGD("Enter onAlarm()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_alarm;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onAlarm:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			Now_cmd_e = SERCMD_unknown;
			LOGV("onAlarm:  no response ...");
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}

	} else {
		LOGV("onAlarm: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onAlarm()");
}

int SerialCommand::onSysUpgrading() {
	LOGD("Enter onSysUpgrading()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_updating;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onSysUpgrading:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			LOGV("onSysUpgrading:  no response ...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onSysUpgrading: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onSysUpgrading()");
}

int SerialCommand::onSysUpgradeFailed() {
	LOGD("Enter onSysUpgradeFailed()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_updatefail;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onSysUpgradeFailed:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			LOGV("onSysUpgradeFailed:  no response ...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}

	} else {
		LOGV("onSysUpgradeFailed: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onSysUpgradeFailed()");
}

int SerialCommand::onSetVolume(int i4Volume) {
	LOGD("Enter onSetVolume()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_setvolume;
	s_write.ser_data = i4Volume;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onSetVolume:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			LOGV("onSetVolume:  no response ...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}

	} else {
		LOGV("onSetVolume: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onSetVolume()");
}

int SerialCommand::onGetVolume() {
	LOGD("Enter onGetVolume()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	s_write.ser_cmd = SERCMD_getvolume;
	s_write.ser_data = 0;

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onGetVolume:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			LOGV("onGetVolume:  no response ...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onGetVolume: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onGetVolume()");
}

int SerialCommand::onSetSoundMode(ser_soundmode_e i4SoundMode) {
	LOGD("Enter onSetSoundMode()");
	ser_data_s s_write;
	struct timespec ts;
	unsigned long nnsec;
	int ret;

	switch (i4SoundMode) {
	case SER_musmode:
		s_write.ser_cmd = SERCMD_setmusmode;
		s_write.ser_data = 0;
		break;
	case SER_newsmode:
		s_write.ser_cmd = SERCMD_setnewsmode;
		s_write.ser_data = 0;
		break;
	case SER_movmode:
		s_write.ser_cmd = SERCMD_setmovmode;
		s_write.ser_data = 0;
		break;
	default:
		break;
	}

	pthread_mutex_lock(&m_mutex);
	Now_cmd_e = s_write.ser_cmd;

	ret = SerWrite_rk3229_t(NOW_COM, &s_write, W_TIMEOUTMS);
	if (ret >= 0) {
		struct timeval delta;
		struct timespec abstime;
		gettimeofday(&delta, NULL);
		abstime.tv_sec = delta.tv_sec + B_TIMEOUTMS / 1000;
		abstime.tv_nsec = (delta.tv_usec + (B_TIMEOUTMS % 1000) * 1000) * 1000;
		if (abstime.tv_nsec > 1000000000) {
			abstime.tv_sec += 1;
			abstime.tv_nsec -= 1000000000;
		}

		if (pthread_cond_timedwait(&m_cond, &m_mutex, &abstime) == 0) {
			LOGV(" onSetSoundMode:  SECC...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return S_Read.ser_data;
		} else {
			LOGV("onSetSoundMode:  no response ...");
			Now_cmd_e = SERCMD_unknown;
			pthread_mutex_unlock(&m_mutex);
			return -2; //
		}
	} else {
		LOGV("onSetSoundMode: SerWrite_rk3229 err...");
		Now_cmd_e = SERCMD_unknown;
		pthread_mutex_unlock(&m_mutex);
		return -1;
	}
	LOGD("Leave onSetSoundMode()");
}

} // namespace soundbar

