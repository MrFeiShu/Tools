#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef _IA64_
#pragma warning(disable:4127)
#endif 

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <process.h>
#include <strsafe.h>

typedef enum{
	SRC2DEST = 0,
	DEST2SRC
}Direction;

using namespace std;

#define DEFAULT_PORT        "5001"          // Default port for server
#define DEFAULT_BUFFER_LEN  100            // Default send/recv buffer length

#define DEFAULT_DEST_IP		"127.0.0.1"
#define DEFAULT_DEST_PORT	"40000"

//
// Function Prototypes
//
void FreeSocket(SOCKET s, int rc);

#endif
