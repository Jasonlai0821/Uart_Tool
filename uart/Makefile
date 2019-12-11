#XX = g++
PROJECT_DIR = $(shell pwd)
PROM    = uart_tool

export ARMCXX = /home/jason.lai/workspace/AIA/MTK5263/mtk_release/msv5263/tool/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

CXXFLAGS ?= -Wall -fPIC -O3 -ldl -lstdc++ -D_USE_LINUX  -Wextra -ffunction-sections -g -lpthread -lm

#INCLUDE_DIRS = -I./include/internal -I./include

#INCLUDE_PATH = $(INCLUDE_DIRS)

all:uart_tool

uart_tool:main.cpp
	$(ARMCXX) $(CXXFLAGS) main.cpp -L./libs -Wl,-rpath-link /home/jason.lai/workspace/AIA/MTK5263/mtk_release/msv5263/SDK/project/C4P/demo/lib -lmi -lm -lpthread -o uart_tool
	@echo "Build successfully"
# -lpthread -lrt -lcurl -ljson 

.PHONY : install clean  
clean:
	rm -rf uart_tool
	@echo "clean done" 
install:
	sudo install -D -m 755 uart_tool -t /usr/bin/
	#cp -f ota $(TARGET_DIR)/usr/bin/
	sudo cp -rf ./libs/*.* /usr/lib/
