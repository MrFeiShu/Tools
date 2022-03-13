#include "common.h"

//
// Function: FreeSocket
//
// Description:
//    If the socket closed without error shut it down and then close the 
//    socket handle.
//
void FreeSocket(SOCKET s, int rc)
{
	int     retval;

	// client connection was closed
	if (rc == NO_ERROR)
	{
		retval = shutdown(s, SD_SEND);
		if (retval == SOCKET_ERROR)
		{
			fprintf(stderr, "FreeSocket: shutdown failed: %d\n", WSAGetLastError());
		}
	}

	// Release the handle
	retval = closesocket(s);
	if (retval == SOCKET_ERROR)
	{
		fprintf(stderr, "FreeSocket: closesocket failed: %d\n", WSAGetLastError());
	}
}