COMPILE_PATH := C:\MinGW
CROSS_COMPILE := $(COMPILE_PATH)\bin/


main : main.c
	$(CROSS_COMPILE)gcc $^ -o $@ 
lib:
	$(CROSS_COMPILE)gcc -c main.c
	$(CROSS_COMPILE)ar crv libsum.a main.o
	$(CROSS_COMPILE)gcc -shared -fPIC -o libsum.dll main.o -lhid -lsetupapi
	
clean :
	del /Q /S .\main.exe