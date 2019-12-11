#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mount.h>  
#include <sys/stat.h>
#include <mtd/mtd-user.h>
#include <assert.h>

#include "mi_fb.h"


//oepn frame buffer
int MI_fb_open(PFBDEV pFbdev)
{
	pFbdev->fb = open(pFbdev->dev,O_RDWR);
	
	if(pFbdev->fb < 0)
	{	
		printf("open fail %s\n",pFbdev->dev);
		return 0;
	}

	//　init fb_var
	if(-1 == ioctl(pFbdev->fb,FBIOGET_VSCREENINFO,&(pFbdev->fb_var)))
	{
		printf("ioctl FBIOGET_VSCREENINFO\n");
		return 0;
	}

	//　init fb_fix
	if(-1 == ioctl(pFbdev->fb,FBIOGET_FSCREENINFO,&(pFbdev->fb_fix)))
	{
		printf("ioctl FBIOGET_FSCREENINFO\n");
		return 0;
	}

	//mmap fbPhyAddr to vAddr
	pFbdev->fb_mem_offset = (unsigned long)pFbdev->fb_fix.smem_start & (~PAGE_MASK);

	pFbdev->fb_mem = (unsigned long int)mmap(NULL,pFbdev->fb_fix.smem_len + pFbdev->fb_mem_offset,PROT_READ|PROT_WRITE,MAP_SHARED,pFbdev->fb,0);
	
	if(-1L == (long)pFbdev->fb_mem){
		printf("mmap error with mem:%lu,mem_offset:%lu\n",pFbdev->fb_mem_offset,pFbdev->fb_mem);
		return 0;
	}
	
	return 1;
}

//close frame buffer
int MI_fb_close(PFBDEV pFbdev)
{
	int r = close(pFbdev->fb);
	pFbdev->fb = -1;

	return r;
}

void MI_fb_pmem_start(PFBDEV pFbdev)
{
	printf("frame buffer start addr:0x%x\n",pFbdev->fb_fix.smem_start);
}

void MI_fb_p_type(PFBDEV pFbdev)
{
	printf("frame buffer type:%d\n",pFbdev->fb_fix.type);
}

void MI_fb_p_visible_res(PFBDEV pFbdev)
{
	printf("frame buffer visible ：\n\tx = %d\n\ty = %d\n",pFbdev->fb_var.xres,pFbdev->fb_var.yres);
}

void MI_fb_p_virt_res(PFBDEV pFbdev)
{
	printf("frame buffer virt：\n\tx = %d\n\ty = %d\n",pFbdev->fb_var.xres_virtual,pFbdev->fb_var.yres_virtual);
}

void MI_fb_p_offset(PFBDEV pFbdev)
{
	printf("frame buffer visible offset:\n\tx = %d\n\ty = %d\n",pFbdev->fb_var.xoffset,pFbdev->fb_var.yoffset);
}

void MI_fb_p_bpp(PFBDEV pFbdev)
{
	printf("frame buffer BPP：%d\n",pFbdev->fb_var.bits_per_pixel);
}

void MI_fb_p_rgbt(PFBDEV pFbdev)
{
	printf("R bit:\n");
	struct fb_bitfield bf = pFbdev->fb_var.red;
	printf("\tstart :%d\n",bf.offset);
	printf("\tlen:%d\n",bf.length);
	printf("\tMSB:%d\n",bf.msb_right);
	
	printf("G bit:\n");
	bf = pFbdev->fb_var.green;
	printf("\tstart:%d\n",bf.offset);
	printf("\tlen:%d\n",bf.length);
	printf("\tMSB:%d\n",bf.msb_right);

	printf("B bit:\n");
	bf = pFbdev->fb_var.blue;
	printf("\tstart:%d\n",bf.offset);
	printf("\tlen:%d\n",bf.length);
	printf("\tMSB:%d\n",bf.msb_right);

	printf("A bit:\n");
	bf = pFbdev->fb_var.transp;
	printf("\tstart:%d\n",bf.offset);
	printf("\tlen:%d\n",bf.length);
	printf("\tMSB:%d\n",bf.msb_right);
}

void MI_fb_p_hw(PFBDEV pFbdev)
{
	printf("frame buffer:\n\tweight = %d\n\thight = %d\n",pFbdev->fb_var.height,pFbdev->fb_var.width);
}

//clear FB
void MI_fb_clear_con(void *addr,int n,size_t len)
{
	memset(addr,n,len);
}

//mmap
void MI_fb_memcpy(void *addr,void *color,size_t len)
{
	memcpy(addr,color,len);
}

RGBT MI_fb_getRGBT(const char *temp){
	RGBT rgbt;	

	char *c_tmp;
	c_tmp = (char *)malloc(4 * sizeof(int));

	c_tmp[0] = '0';
	c_tmp[1] = 'x';
	int r,g,b,t;

	//RGBT
	c_tmp[2] = temp[0];
	c_tmp[3] = temp[1];
	
	r = strtol(c_tmp,NULL,16);
	
	c_tmp[2] = temp[2];
	c_tmp[3] = temp[3];
	
	g = strtol(c_tmp,NULL,16);

	c_tmp[2] = temp[4];
	c_tmp[3] = temp[5];
	
	b = strtol(c_tmp,NULL,16);
	
	c_tmp[2] = temp[6];
	c_tmp[3] = temp[7];
	
	t = strtol(c_tmp,NULL,16);

	rgbt.r = r;
	rgbt.g = g;
	rgbt.b = b;
	rgbt.t = t;

	return rgbt;
}

void MI_fb_draw_dot(PFBDEV pFbdev,POINT p,uint8_t r,uint8_t g,uint8_t b)
{
	uint32_t offset;
	uint8_t color[4];
	color[0] = b;
	color[1] = g;
	color[2] = r;
	color[3] = 0x0; //alpha 
	
	offset = p.y * pFbdev->fb_fix.line_length + 4 * p.x;
	MI_fb_memcpy((void*)pFbdev->fb_mem + pFbdev->fb_mem_offset + offset,color,4);
}

void MI_fb_draw_dot_with_trans(PFBDEV pFbdev,POINT p,uint8_t r,uint8_t g,uint8_t b,uint8_t t)
{
	uint32_t offset;
	uint8_t color[4];
	color[0] = b;
	color[1] = g;
	color[2] = r;
	color[3] = t; //alpha
	
	offset = p.y * pFbdev->fb_fix.line_length + 4 * p.x;
	MI_fb_memcpy((void*)pFbdev->fb_mem + pFbdev->fb_mem_offset + offset,color,4);
}

void MI_fb_draw_x_y_dot(PFBDEV pFbdev,int x,int y,uint8_t r,uint8_t g,uint8_t b)
{
	POINT p;
	p.x = x;
	p.y = y;

	MI_fb_draw_dot(pFbdev,p,r,g,b);
}

void MI_fb_draw_x_y_dot_with_trans(PFBDEV pFbdev,int x,int y,uint8_t r,uint8_t g,uint8_t b,uint8_t t)
{
	POINT p;
	p.x = x;
	p.y = y;

	MI_fb_draw_dot_with_trans(pFbdev,p,r,g,b,t);
}

void MI_fb_draw_x_y_color_dot(PFBDEV pFbdev,int x,int y,RGBT c)
{
	MI_fb_draw_x_y_dot(pFbdev,x,y,c.r,c.g,c.b);
}

void MI_fb_draw_x_y_color_dot_with_trans(PFBDEV pFbdev,int x,int y,RGBT c)
{
	MI_fb_draw_x_y_dot_with_trans(pFbdev,x,y,c.r,c.g,c.b,c.t);
}

void MI_fb_draw_x_y_color_dot_with_string(PFBDEV pFbdev,int x,int y,const char *temp)
{
	RGBT rgbt = MI_fb_getRGBT(temp);
	
	MI_fb_draw_x_y_dot_with_trans(pFbdev,x,y,rgbt.r,rgbt.g,rgbt.b,rgbt.t);
}

void MI_fb_draw_h_line(PFBDEV pFbdev,POINT minX,POINT maxX,uint8_t r,uint8_t g,uint8_t b)
{
	int m;
	
	int length = maxX.x - minX.x;
	for(m = 0;m < length;m++){
		POINT tp;
		tp.x = minX.x + m;
		tp.y = minX.y;

		MI_fb_draw_dot(pFbdev,tp,r,g,b);
	}
}

void MI_fb_draw_h_line_with_trans(PFBDEV pFbdev,POINT minX,POINT maxX,uint8_t r,uint8_t g,uint8_t b,uint8_t t){
	int m;
	
	int length = maxX.x - minX.x;
	for(m = 0;m < length;m++){
		POINT tp;
		tp.x = minX.x + m;
		tp.y = minX.y;

		MI_fb_draw_dot_with_trans(pFbdev,tp,r,g,b,t);
	}
}

void MI_fb_draw_v_line(PFBDEV pFbdev,POINT minY,POINT maxY,uint8_t r,uint8_t g,uint8_t b)
{
	int m;
	
	int length = maxY.y - minY.y;
	for(m = 0;m < length;m++){
		POINT tp;
		tp.x = minY.x;
		tp.y = minY.y + m;

		MI_fb_draw_dot(pFbdev,tp,r,g,b);
	}
}

void MI_fb_draw_h_line_with_string(PFBDEV pFbdev,POINT minX,POINT maxX,const char *color)
{
	RGBT rgbt = MI_fb_getRGBT(color);

	MI_fb_draw_h_line_with_trans(pFbdev,minX,maxX,rgbt.r,rgbt.g,rgbt.b,rgbt.t);
}

void MI_fb_draw_rec(PFBDEV pFbdev,POINT lu,POINT ld,POINT ru,POINT rd,uint8_t r,uint8_t g,uint8_t b)
{
	MI_fb_draw_h_line(pFbdev,lu,ru,r,g,b);
	MI_fb_draw_h_line(pFbdev,ld,rd,r,g,b);
	MI_fb_draw_v_line(pFbdev,lu,ld,r,g,b);
	MI_fb_draw_v_line(pFbdev,ru,rd,r,g,b);
}

void MI_fb_fill_rec(PFBDEV pFbdev,POINT lu,POINT ld,POINT ru,POINT rd,uint8_t r,uint8_t g,uint8_t b)
{
	int xlen = ru.x - lu.x;
	int ylen = ld.y - lu.y;

	int m,n;

	for(m = 0;m < ylen;m++){
		for(n = 0;n < xlen;n++){
			POINT p;
			p.x = lu.x + n;
			p.y = lu.y + m;
	
			MI_fb_draw_dot(pFbdev,p,r,g,b);	
		}	
	}
}

void MI_fb_draw_circle(PFBDEV pFbdev,int x,int y,int radius,const char *color)
{
	double i;

	int tmp;

	for(i = x - radius; i < x + radius;i+=0.01){
		tmp = sqrt(radius * radius - (i - x) * (i - x));

		MI_fb_draw_x_y_color_dot_with_string(pFbdev,i,(tmp + y),color);
		MI_fb_draw_x_y_color_dot_with_string(pFbdev,i,(-tmp + y),color);
	}
}

