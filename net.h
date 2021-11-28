#pragma once

#ifdef linux
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <unistd.h>
#endif
#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <ws2tcpip.h>
#endif
#ifdef __MACH__
// mac porting
#endif