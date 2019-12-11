################################################################################
#
# Copyright (c) 2006-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
# Makefile used for include MI files
#
#################################################################################
SHELL=/bin/bash

# **********************************************
# Import configure file
# **********************************************

PRJ_DIR              =.
MI_DIR               = $(PRJ_DIR)

CROSS_COMPILE=/home/jason.lai/workspace/AIA/MTK5263/mtk_release/msv5263/tool/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc

CFLAGS   = -Wall -Wextra -ffunction-sections -O2 -fPIC -g -lpthread -lm

MI_SRCDIR=$(MI_DIR)
MI_OBJPATH=$(MI_DIR)

CC_OPTS= $(INCLUDE)

INCDIR      += \
            += $(MI_DIR)/include   \
            += $(MI_DIR)/include/internal \
            += $(MI_DIR) \
			
INCLUDE    += $(foreach dir, $(INCDIR), -I$(dir))

MI_SRC_OUT     =
MI_SRC_ALL     = $(foreach dir, $(MI_SRCDIR), $(wildcard $(dir)/*.c))
MI_SRC_C       = $(filter-out $(MI_SRC_OUT), $(MI_SRC_ALL))
MI_SRC         = $(MI_SRC_C)

MI_OBJ_C_0     = $(notdir $(patsubst %.c, %.o, $(MI_SRC_C)))
MI_OBJ_C       = $(foreach file, $(MI_OBJ_C_0), $(MI_OBJPATH)/$(file))
MI_OBJ         = $(MI_OBJ_C)

LIB_NAME=libmi

$(MI_OBJ_C): %.o : %.c
	@echo  [mi] $@
	@$(CC) $(CC_OPTS) $(LIB_CFLAGS) -c  $< -o $@ ;
	
all:shared_library

shared_library:
	@$(CC) $(CC_OPTS)  -shared -fpic -o ${LIB_NAME}.so $(MI_OBJPATH)/*.c  $(pthread_OPTS);
	
clean:
	rm -rf $(MI_OBJPATH)/*.o
	rm -rf $(PRJ_DIR)/*.so