GPP= g++
CFLAGS= 

main:
	$(GPP) $(CFLAGS) .\Source\main.cpp

clean:
	powershell rm *.exe