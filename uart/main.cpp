#include <linux/version.h>
#include <linux/input.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <pthread.h>
#include <getopt.h>
#include <termios.h>
#include <sched.h>

#include "mi_common.h"
#include "mi_loader.h"

#include "mi_sys.h"
#include "mi_wlan.h"
#include "mi_flash.h"
#include "mi_ubi.h"
#include "mi_fb.h"

#define LOG_TAG "Uart_Test"

#define DEV_TTY_PATH     "/dev"
#define TTYS_DEV_NAME      "ttyS"

#define W_TIMEOUTMS	10		// millisecond
#define R_TIMEOUTMS	10		// millisecond
#define TTYS_DEV_TESTBUF	"uart_test_buffer"
#define COMMAND_VALUESIZE 255
#define MAX_PACKET_LEN 64
#define MAX_REP_LEN 6
#define OVER_READ_LEN 17


/********************************************************
******P_header****P_length****CMD****Datan****Verify*****
	  1Byte      1Byte     1Byte    nByte     1Byte
	  0x5B        len       cmd      ndata     crc
*********************************************************/

#define PHEADER 0x5A
#define PRHEADER 0x5B

#define P_AHEAD 0
#define P_ALEN  1
#define P_ACMD  2

#define CMD_232ENTER 	    0x0D
#define CMD_232AENTER		0x0A
#define CMD_SHANDS			0x80
#define CMD_WRITE_BARCODE 	0x82
#define CMD_WRITE_MAC		0x83
#define CMD_START_WRITE		0x8D

#define MAC_ADD "mac_add"
#define BAR_CODE "bar_code"

struct support_tty_dev{
	char* name;
	int flag;  // 0:disable   1:enable
};

struct support_tty_dev support_dev[] = {
	{"ttyS0",0},
	{"ttyS1",0},
	{"ttyS2",1},
	{"ttyS3",0},
	{"ttyS4",0},
};

pthread_mutex_t ev_mutex;
MI_FLASH_EnvInfo_t CurEnv;
static int ttyfd = -1;
static unsigned char MacAdd[MAX_REP_LEN+1] = {0};
static unsigned char BarCode[MAX_PACKET_LEN] = {0};
static int shands_flag = -1;
static int COUNT_TIME = 2*100;

static int is_tty_device(const struct dirent *dir) {
    return strncmp(TTYS_DEV_NAME, dir->d_name, 4) == 0;
}


static int tty_dev_set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if(tcgetattr( fd,&oldtio)  !=  0){
	 	printf("tty_dev_set_opt() fd =%d set option failed!\n",fd);
	  	perror("tty_dev_set_opt() error\n");
	  	return -1;
	}
	bzero(&newtio, sizeof( newtio ));
	oldtio.c_cflag  |=  CLOCAL | CREAD; //设置控制模式状态，本地连接，接收使能
	oldtio.c_cflag &= ~CSIZE; ;//字符长度，设置数据位之前一定要屏掉这个位
	oldtio.c_cflag &= ~CRTSCTS;//无硬件流控
	 
	 switch( nBits )
	 {
	 case 7:
	  	oldtio.c_cflag |= CS7;
	  	break;
	 case 8:
	  	oldtio.c_cflag |= CS8;
	  	break;
	 }
 
	 switch( nEvent )
	 {
	 case 'O':
	  	oldtio.c_cflag |= PARENB; //允许输出产生奇偶信息以及输入到奇偶校验
	  	oldtio.c_cflag |= PARODD;  //输入和输出是奇及校验
	  	oldtio.c_iflag |= (INPCK | ISTRIP); // INPACK:启用输入奇偶检测；ISTRIP：去掉第八位
	  	break;
	 case 'E':
	  	oldtio.c_iflag |= (INPCK | ISTRIP);
	  	oldtio.c_cflag |= PARENB;
	  	oldtio.c_cflag &= ~PARODD;
	  	break;
	 case 'N': 
	  	oldtio.c_cflag &= ~PARENB;
	  	break;
	 default:
	 	oldtio.c_iflag |= IGNPAR;//无奇偶检验位 
		break;
	 }
 
	 switch( nSpeed )
	 {
	 case 2400:
	  	cfsetispeed(&oldtio, B2400);
	  	cfsetospeed(&oldtio, B2400);
	  	break;
	 case 4800:
	  	cfsetispeed(&oldtio, B4800);
	  	cfsetospeed(&oldtio, B4800);
	  	break;
	 case 9600:
	  	cfsetispeed(&oldtio, B9600);
	  	cfsetospeed(&oldtio, B9600);
	  	break;
	 case 115200:
	  	cfsetispeed(&oldtio, B115200);
	  	cfsetospeed(&oldtio, B115200);
	  	break;
	 case 460800:
	  	cfsetispeed(&oldtio, B460800);
	  	cfsetospeed(&oldtio, B460800);
	  	break;
	 default:
	  	cfsetispeed(&oldtio, B9600);
	  	cfsetospeed(&oldtio, B9600);
	  	break;
	 }
 
	 if( nStop == 1 )
	  	oldtio.c_cflag &=  ~CSTOPB; //CSTOPB:设置两个停止位，而不是一个
	 else if ( nStop == 2 )
	 	oldtio.c_cflag |=  CSTOPB;

	 oldtio.c_oflag = 0; //输出模式 
	 oldtio.c_lflag = 0; //不激活终端模式 
	 //oldtio.c_cc[VTIME]  = 0; //VTIME:非cannoical模式读时的延时，以十分之一秒位单位
	 //oldtio.c_cc[VMIN] = 0; //VMIN:非canonical模式读到最小字符数
	 tcflush(fd,TCIFLUSH); // 改变在所有写入 fd 引用的对象的输出都被传输后生效，所有已接受但未读入的输入都在改变发生前丢弃。
	 if((tcsetattr(fd,TCSANOW,&oldtio))!=0) //TCSANOW:改变立即发生
	 {
		printf("tty_dev_set_opt() fd =%d set option failed!\n",fd);
	  	return -1;
	 }
	 printf("tty_dev_set_opt() fd =%d set option success!\n",fd);
	 
	 return 0;
}

static int find_multi_tty_dev(int *fds)
{
	struct dirent **namelist;
	int i, j,ndev,tdev;
	char fname[64];
    int fd = -1, ret = -1;
    int count = 0;

	ndev = scandir(DEV_TTY_PATH, &namelist, is_tty_device, alphasort);

	tdev = sizeof(support_dev)/sizeof(struct support_tty_dev);
	printf("find_multi_tty_dev() tdev:%d,ndev:%d\n",tdev,ndev);
	
	if (ndev <= 0)
    	return 0;
	for(i =0; i < ndev; i++){
		for(j =0; j < tdev; j++){
			if(support_dev[j].flag == 1 && strstr(namelist[i]->d_name, support_dev[j].name)){
				sprintf(fname, "%s/%s", DEV_TTY_PATH, namelist[i]->d_name);
				fd = open(fname,  O_RDWR | O_NOCTTY | O_NDELAY);
				if (fd < 0){
					printf("find_multi_tty_dev() open fname:%s failed\n",fname);
            		continue;
				}
				else{
					int nset = tty_dev_set_opt(fd, 115200, 8, 'D', 1);
					if(nset < 0){
						printf("find_multi_tty_dev() set option fname:%s failed\n",fname);
						close(fd);
						continue;
					}else{
						printf("find_multi_tty_dev() open fname:%s success\n",fname);
						fds[count++] = fd;
						ttyfd = fd;
					}
					
				}
			}
		}
		free(namelist[i]);
	}

	if(count == 0){
		printf("Can't find ttyS device\n");
	}
	free(namelist);

	printf("find_multi_tty_dev() count =%d\n",count);
	return count;
}

static int close_tty_dev(int fd)
{
	printf("close_tty_dev() fd =%d\n",fd);
	close(fd);
}

static void clear_tty_dev_data(int fd)
{
	printf("clear_tty_dev_data() fd =%d\n",fd);
	tcflush(fd, TCIOFLUSH);
}

static int write_tty_dev_data(int fd,unsigned char *pszBuf,unsigned int sendCnt, unsigned long timeOutMS)
{
	int ret = 0;
	struct timeval CurTime;
	fd_set wwrite;

	//printf("write_tty_dev_data() fd =%d\n",fd);
	FD_ZERO(&wwrite);
	FD_SET(fd, &wwrite);

	CurTime.tv_sec = (timeOutMS/ 1000);
	CurTime.tv_usec = (timeOutMS % 1000) * 1000;

	ret = select(fd + 1, NULL, &wwrite, NULL, &CurTime);
	if (ret == 0) {
		printf("tty dev fd:%d write data select timeout\n",fd);
		return -1;
	} else if (ret < 0) {
		printf("tty dev fd:%d write data select error:%d\n",fd,errno);
		return -1;
	}

	ret = write(fd, pszBuf, sendCnt);
	//printf("write_tty_dev_data() write buffer:%s\n",pszBuf);
	if (ret <= 0) {
		clear_tty_dev_data(fd);
		printf("tty dev fd:%d  write buffer error:%d \n",fd, errno);
		return -1;
	}
	return 0;
	
}

static int read_tty_dev_data(int fd,unsigned char *pszBuf,int readCnt,unsigned long timeOutMS)
{
	int ret = 0;
	int index = 0;
	struct timeval CurTime;
	fd_set wread;

	//printf("read_tty_dev_data() fd =%d,readCnt = %d\n",fd,readCnt);
	FD_ZERO(&wread);
	FD_SET(fd, &wread);

	CurTime.tv_sec = (timeOutMS / 1000);
	CurTime.tv_usec = (timeOutMS % 1000) * 1000;

	ret = select(fd + 1, &wread, NULL, NULL, &CurTime);
	if (ret == 0) {
		//printf("read_tty_dev_data() %d read data select timeout\n",fd);
		return -1;
	} else if (ret < 0) {
		printf("tty dev fd:%d read data select error: %d\n", fd,errno);
		return -1;
	}

	if(FD_ISSET(fd, &wread)){
		ret = read(fd, pszBuf, readCnt);
	}
	
	if (ret <= 0) {
		printf("tty dev fd:%d read buffer error:%d \n",fd, errno);
		return -1;
	}else{
		printf("read_tty_dev_data() read buffer:%s\n",pszBuf);
	}
	return ret;
}


int uart_test(void *argv)
{
	int tty_fds[5],max_fd;
	int tty_fds_count = 0;
	int i, ret = -1;
	int w_flag = -1,r_flag = -1;
	char buf[COMMAND_VALUESIZE];

	printf("====================function : %s start =================\n", __func__);
	memset(buf, 0 , COMMAND_VALUESIZE * sizeof(char));
	pthread_mutex_init(&ev_mutex,NULL);
	tty_fds_count = find_multi_tty_dev(tty_fds);

	printf("tty_fds_count = %d\n", tty_fds_count);
	if(tty_fds_count < 1){
		return -1;
	}

	for(i =0; i < tty_fds_count; i++){
		clear_tty_dev_data(tty_fds[i]);
	}
	
	int count  = 0;
	while(1){
		strncpy(buf,TTYS_DEV_TESTBUF,16);
		for(i =0; i < tty_fds_count; i++){
			pthread_mutex_lock(&ev_mutex);
			//write data
			
			if(write_tty_dev_data(tty_fds[i],(unsigned char*)buf,16,W_TIMEOUTMS) < 0){
				w_flag = 0;
			}
			pthread_mutex_unlock(&ev_mutex);
		}

		if(w_flag == 0){
			printf("uart_test() write data failed!!!\n");
			goto fail;
		}

		sleep(1);
	
		for(i =0; i < tty_fds_count; i++){
			memset(buf, 0 , COMMAND_VALUESIZE * sizeof(char));
			pthread_mutex_lock(&ev_mutex);
			//read data
			if(read_tty_dev_data(tty_fds[i],(unsigned char*)buf,16,R_TIMEOUTMS) < 0){
				r_flag = 0;
			}
			printf("uart_test() read data\n");

			if(strncmp(buf, TTYS_DEV_TESTBUF, 16) == 0){
				ret = 0;
				goto success;
			}else{
				count ++;
				r_flag = 0;
				if(count > 60){
					goto fail;
				}
			}
			pthread_mutex_unlock(&ev_mutex);
		}
	}

	if(r_flag == 0){
		printf("uart_test() read data failed!!!\n");
		goto fail;
	}

success:
	for(i =0; i < tty_fds_count; i++){
		clear_tty_dev_data(tty_fds[i]);
		close_tty_dev(tty_fds[i]);
		tty_fds[i] = -1;
	}

	printf("====================function : %s success end =================\n", __func__);
	return 0;
	
fail:
	for(i =0; i < tty_fds_count; i++){
		clear_tty_dev_data(tty_fds[i]);
		close_tty_dev(tty_fds[i]);
		tty_fds[i] = -1;
	}
	printf("====================function : %s fail end =================\n", __func__);
	return -1;
}

void send_Responecmd(char * buf,int len)
{
	pthread_mutex_lock(&ev_mutex);
	write_tty_dev_data(ttyfd,(unsigned char*)buf,len,W_TIMEOUTMS);
	pthread_mutex_unlock(&ev_mutex);
}

void send_responecmd_OK()
{
	char buf[4] = {0};
	buf[0] = PRHEADER;
	buf[1] = 0x04;
	buf[2] = 0x0A;
	buf[3] = 0x69;
	send_Responecmd(buf, 4);
}

void send_responecmd_ERR()
{
	char buf[4] = {0};
	buf[0] = PRHEADER;
	buf[1] = 0x04;
	buf[2] = 0x0C;
	buf[3] = 0x6B;
	send_Responecmd(buf, 4);
}

void send_responecmd_Retrans()
{
	char buf[4] = {0};
	buf[0] = PRHEADER;
	buf[1] = 0x04;
	buf[2] = 0x0E;
	buf[3] = 0x6D;
	send_Responecmd(buf, 4);
}

void send_responecmd_SHOK()
{
	char buf[5] = {0};
	buf[0] = PRHEADER;
	buf[1] = 0x05;
	buf[2] = 0x81;
	buf[3] = 0x01;
	buf[4] = 0xE2;
	send_Responecmd(buf, 5);
}

void send_responecmd_SHFailed()
{
	char buf[5] = {0};
	buf[0] = PRHEADER;
	buf[1] = 0x05;
	buf[2] = 0x81;
	buf[3] = 0x00;
	buf[4] = 0xE1;
	send_Responecmd(buf, 5);
}

void Int2Char(int p_nNum, char *pCh)
{
	if(p_nNum <= 9){
		*pCh = p_nNum + '0';
	}else{
		*pCh = (p_nNum-10) + 'A';
	}
}

void Str2BcdStr(char *p_Str, int p_nLen, char *p_StrBcd)
{
	int i =0, j = 0;
	
	for(i = 0; i < p_nLen; ++i)
	{		
		Int2Char((p_Str[i] >> 4)&0x0F, &p_StrBcd[j++]);	
		Int2Char(p_Str[i] &0x0F, &p_StrBcd[j++]);
	}
	return ;
}

void * thread_process_scmd_fun(void *arg)
{
	unsigned char* receiveBuf = (unsigned char*)arg;
	//printf("thread_process_scmd_fun()\n");
	char revBuf[MAX_PACKET_LEN] = {0};
	unsigned char responeBuf[MAX_REP_LEN] = {0};
	
	responeBuf[0] = 0x5B;
	responeBuf[1] = 0x06;
	responeBuf[2] = 0x8E;
	
	strcpy(revBuf,(char*)receiveBuf);
	
	if(receiveBuf[P_AHEAD] == PHEADER && receiveBuf[P_ACMD] == CMD_START_WRITE){
		char env_name[MAX_PACKET_LEN] = {0};
		char env_value[MAX_PACKET_LEN*2] = {0};
		char cmd_buf[COMMAND_VALUESIZE] = {0};
		int len =0,invalid = 0;
		
		switch(receiveBuf[P_ACMD+4]){
		case CMD_WRITE_BARCODE:
			len = BarCode[0];
			for(int i = 0; i < len; i++){
				invalid = invalid ^ BarCode[1+i];
			}
			printf("thread_process_scmd_fun() CMD_WRITE_BARCODE len = %d,recLen = %d\n",len,receiveBuf[P_ACMD+2]);
			printf("thread_process_scmd_fun() CMD_WRITE_BARCODE invalid = 0x%02x,r_invalid = 0x%02x\n",invalid,receiveBuf[P_ACMD+3]);
			if(len == receiveBuf[P_ACMD+2] && invalid == receiveBuf[P_ACMD+3]){
				strncpy(env_name,BAR_CODE,8);
				Str2BcdStr((char*)(BarCode+1),len,env_value);
			}
			responeBuf[4] =CMD_WRITE_BARCODE;
			break;
		case CMD_WRITE_MAC:
			len = MacAdd[0];
			for(int i = 0; i < len; i++){
				invalid = invalid ^ MacAdd[1+i];
			}

			printf("thread_process_scmd_fun() CMD_WRITE_MAC len = %d,recLen = %d\n",len,receiveBuf[P_ACMD+2]);
			printf("thread_process_scmd_fun() CMD_WRITE_BARCODE invalid = 0x%02x,r_invalid = 0x%02x\n",invalid,receiveBuf[P_ACMD+3]);
			if(len == receiveBuf[P_ACMD+2] && invalid == receiveBuf[P_ACMD+3]){
				strncpy(env_name,MAC_ADD,7);
				Str2BcdStr((char*)(MacAdd+1),len,env_value);
			}
			
			responeBuf[4] =CMD_WRITE_MAC;
			break;
		default:
			break;
		}
		
		if(env_name[0] != 0){
			printf("thread_process_scmd_fun() env_name:%s,env_value:%s\n",env_name,env_value);
			if(ms_setenv(env_name,env_value) != 0){//failed
				responeBuf[3] = 0x00;//data1
			}else{
				responeBuf[3] = 0x01;//data1
			}

			responeBuf[5] = (responeBuf[0] + responeBuf[1] +responeBuf[2] + responeBuf[3] + responeBuf[4]) & 0xFF;//crc

			send_Responecmd((char*)responeBuf,MAX_REP_LEN);

			sprintf(cmd_buf,"setprop rw.build.system.%s %s",env_name,env_value);
			system(cmd_buf);
		}else{
			responeBuf[3] = 0x00;//data1
			responeBuf[5] = (responeBuf[0] + responeBuf[1] +responeBuf[2] + responeBuf[3] + responeBuf[4]) & 0xFF;//crc
			send_Responecmd((char*)responeBuf,MAX_REP_LEN);
		}
	}
}

void startProcessCmd_Thread(unsigned char* buf)
{
	pthread_t tid;
	printf("startRead_Thread()\n");
    if(pthread_create(&tid, NULL, thread_process_scmd_fun, buf) == -1)
    {
        printf("fail to pthread_create");
        return;
    }

    void * result;
    if(pthread_join(tid, &result) == -1){
        printf("fail to pthread_join");
        return;
    }
}

int uart_parse_scmd(int fd,unsigned char* pszBuf)
{
	//printf("uart_parse_scmd pszBuf:0x%02x,0x%02x,0x%02x\n",pszBuf[P_AHEAD],pszBuf[P_ALEN],pszBuf[P_ACMD]);

	if(pszBuf[P_AHEAD] == 0x03 && pszBuf[P_ACMD] == 0x0A){
		send_responecmd_OK();
	}
	
	if(pszBuf[P_AHEAD] == PHEADER){
		int is_need_process = 0;
		switch(pszBuf[P_ACMD]){
			case CMD_232ENTER:
			case CMD_232AENTER:
				send_responecmd_OK();
				break;
			case CMD_SHANDS:
				send_responecmd_OK();
				usleep(30 * 1000);
				shands_flag = 0;
				send_responecmd_SHOK();
				break;
			case CMD_WRITE_BARCODE:
				BarCode[0] = pszBuf[P_ALEN] -6;
				memcpy(BarCode+1,pszBuf + (P_ACMD+3),pszBuf[P_ALEN] -6);
				send_responecmd_OK();
				break;
			case CMD_WRITE_MAC:
				MacAdd[0] = pszBuf[P_ALEN] -6;
				memcpy(MacAdd+1,pszBuf + (P_ACMD+3),pszBuf[P_ALEN] -6);
				send_responecmd_OK();
				break;
			case CMD_START_WRITE:
				is_need_process =1;
				send_responecmd_OK();
				break;
			default:
				send_responecmd_ERR();
				clear_tty_dev_data(fd);
				break;
		}
		if(is_need_process == 1 && shands_flag == 0){
			startProcessCmd_Thread(pszBuf);
		}
	}else{
		clear_tty_dev_data(fd);
		return -1;
	}
	return 0;
}

void * thread_read_sercom_fun(void *arg)
{
	int tty_fds[5],max_fd;
	int tty_fds_count = 0;
	int i, ret = -1;
	int r_count =0;
	unsigned char pReponsebuf[MAX_PACKET_LEN] = {0};
	unsigned char pReadbuf[MAX_PACKET_LEN+1] ={0};
	int n_read_bytes = 0;
	int n_store_bytes = 0;

	printf("====================function : %s start =================\n", __func__);
	memset(pReadbuf, 0x0 , (MAX_PACKET_LEN+1) * sizeof(unsigned char));
	pthread_mutex_init(&ev_mutex,NULL);
	tty_fds_count = find_multi_tty_dev(tty_fds);

	printf("tty_fds_count = %d\n", tty_fds_count);
	if(tty_fds_count < 1){
		return NULL;
	}

	for(i =0; i < tty_fds_count; i++){
		clear_tty_dev_data(tty_fds[i]);
	}

	while(1){
		int j = 0;
		int n_trim_byte = 0;

		usleep(30);
		r_count++;
		if(r_count >= COUNT_TIME){
			goto fail;
		}
		
		for(i =0; i < tty_fds_count; i++){
			memset(pReadbuf, 0x0 , (MAX_PACKET_LEN+1) * sizeof(unsigned char));
			pthread_mutex_lock(&ev_mutex);
			//read data
			n_read_bytes = read_tty_dev_data(tty_fds[i],(unsigned char*)(pReadbuf),MAX_PACKET_LEN,R_TIMEOUTMS);
			
			if(n_read_bytes <= 0){
				if (n_store_bytes <= 0){
					pthread_mutex_unlock(&ev_mutex);
					continue;
				}
			}
			COUNT_TIME = 20*100*3*10;

			printf("thread_read_sercom_fun() n_store_bytes = %d,n_read_bytes= %d\n", n_store_bytes,n_read_bytes);
			for(j =0; j < n_read_bytes; j++){
				unsigned char value = 0;
				//printf("thread_read_sercom_fun pReadbuf[%d] = 0x%02x\n",j,pReadbuf[j]);
				
				pReponsebuf[n_store_bytes+j] = pReadbuf[j];
				
			}

			memset(pReadbuf, 0x0 , (MAX_PACKET_LEN+1) * sizeof(unsigned char));
			pthread_mutex_unlock(&ev_mutex);

			n_store_bytes += n_read_bytes;
			
			if(n_read_bytes == OVER_READ_LEN){
				printf("over read len,continue reading!\n");
				continue;
			}
			//parse the buf
			uart_parse_scmd(tty_fds[i],pReponsebuf);
			memset(pReponsebuf, 0x0, sizeof(pReponsebuf));
			n_store_bytes = 0;
		}
	}

fail:
	for(i =0; i < tty_fds_count; i++){
		clear_tty_dev_data(tty_fds[i]);
		close_tty_dev(tty_fds[i]);
		tty_fds[i] = -1;
	}
	printf("thread_read_sercom_fun exit!\n");
}

void startRead_Thread()
{
	pthread_t tid;
	printf("startRead_Thread()\n");
    if(pthread_create(&tid, NULL, thread_read_sercom_fun, NULL) == -1)
    {
        printf("fail to pthread_create");
        return;
    }

    void * result;
    if(pthread_join(tid, &result) == -1){
        printf("fail to pthread_join");
        return;
    }
}

int 
main(int argc, char *argv[])
{
	char env_val[COMMAND_VALUESIZE] = {0};
	char cmd_buf[COMMAND_VALUESIZE*2] = {0};
	prepare_ota();

	if (ms_getenv(MAC_ADD, env_val) != 0) {
     	//faild
     	strcpy(env_val,"default");
    }else{
		printf("uart MAC_ADD:%s\n",env_val);
	}

	sprintf(cmd_buf,"setprop rw.build.system.%s %s",MAC_ADD,env_val);
	system(cmd_buf);
	
	memset(cmd_buf, 0, COMMAND_VALUESIZE *sizeof(char));
	memset(env_val, 0, COMMAND_VALUESIZE *sizeof(char));
	
	if (ms_getenv(BAR_CODE, env_val) != 0) {
     	//faild  
     	strcpy(env_val,"default");
    }else{
		printf("uart BAR_CODE:%s\n",env_val);
	}

	sprintf(cmd_buf,"setprop rw.build.system.%s %s",BAR_CODE,env_val);
	system(cmd_buf);
	
	startRead_Thread();
	printf("uart tool exit()\n");
	exit(0);
    return 0;
}