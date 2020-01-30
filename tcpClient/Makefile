COMPILE_PATH := C:\MinGW
CROSS_COMPILE := $(COMPILE_PATH)\bin/

INCLUDE_PATH = $(COMPILE_PATH)\include\

tcpClient : tcpClient.c
	$(CROSS_COMPILE)gcc $^ -o $@ -lws2_32
lib:
	$(CROSS_COMPILE)gcc -c main.c
	$(CROSS_COMPILE)ar crv libsum.a main.o
	$(CROSS_COMPILE)gcc -shared -fPIC -o libsum.dll main.o -lhid -lsetupapi
	
clean :
	del /Q /S .\main.exe