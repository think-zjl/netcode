COMPILE_PATH := C:\MinGW
CROSS_COMPILE := $(COMPILE_PATH)\bin/


main : udpClient.c
	$(CROSS_COMPILE)gcc $^ -o $@ -lws2_32
lib:
	$(CROSS_COMPILE)gcc -c udpClient.c
	$(CROSS_COMPILE)ar crv libudpClient.a udpClient.o
	$(CROSS_COMPILE)gcc -shared -fPIC -o libsum.dll udpClient.o -lhid -lsetupapi
	
clean :
	del /Q /S .\main.exe