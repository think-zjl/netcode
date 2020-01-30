COMPILE_PATH := C:\MinGW
CROSS_COMPILE := $(COMPILE_PATH)\bin/


tcpServer : tcpServer.c
	$(CROSS_COMPILE)gcc $^ -o $@ -lws2_32
lib:
	$(CROSS_COMPILE)gcc -c tcpServer.c
	$(CROSS_COMPILE)ar crv libsum.a tcpServer.o
	$(CROSS_COMPILE)gcc -shared -fPIC -o libsum.dll tcpServer.o -lhid -lsetupapi
	
clean :
	del /Q /S .\main.exe