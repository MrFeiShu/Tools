#include "tcpproxy.h"

TcpProxy::TcpProxy()
{

}

TcpProxy::~TcpProxy()
{

}

bool TcpProxy::Init(SOCKET sockSrc)
{
	m_sockSrc = sockSrc;
	
	m_sockDest = ConnectDest();
	if (m_sockDest == INVALID_SOCKET)
	{
		return false;
	}
	else
	{
		Proxy();
		return true;
	}

}

SOCKET TcpProxy::ConnectDest()
{
	SOCKET sockDest = INVALID_SOCKET;
	char	pszServerName[MAX_PATH] = {0};
	char	pszServerPort[MAX_PATH] = {0};
	int		nRet = SOCKET_ERROR;

	ADDRINFO hints, *results;

	(void)StringCchCopyA(pszServerName, _countof(pszServerName), DEFAULT_DEST_IP);
	(void)StringCchCopyA(pszServerPort, _countof(pszServerPort), DEFAULT_DEST_PORT);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	nRet = getaddrinfo(pszServerName, pszServerPort, &hints, &results);
	if (nRet != 0)
	{
		printf("getaddrinfo failed with error %u.\n", WSAGetLastError());
		return INVALID_SOCKET;
	}
	if (results == NULL)
	{
		printf("Server (%s) name could not be resolved!\n", pszServerName);
		return INVALID_SOCKET;
	}

	sockDest = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (sockDest == INVALID_SOCKET)
	{
		printf("socket failed with error %u.\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	nRet = connect(sockDest, results->ai_addr, results->ai_addrlen);
	if (nRet == SOCKET_ERROR)
	{
		printf("connect failed with error %u.\n", WSAGetLastError());
		return INVALID_SOCKET;
	}


END:
	if (nRet == SOCKET_ERROR)
	{
		if (sockDest == INVALID_SOCKET)
		{
			closesocket(sockDest);
			sockDest = INVALID_SOCKET;
		}

		return INVALID_SOCKET;
	}
	if (NULL != results)
	{
		freeaddrinfo(results);
	}

	return sockDest;
}

void TcpProxy::Proxy()
{
	(void)_beginthreadex(NULL, NULL, PorxyThradProc, (void*)this, NULL, NULL);
}

unsigned __stdcall TcpProxy::PorxyThradProc(void* vParam)
{
	unsigned int uRet = 0;

	if (NULL == vParam)
	{
		printf("invalid param.\n");
		return 1;
	}

	struct fd_set	readfds;
	char			buffer[DEFAULT_BUFFER_LEN] = {0};
	TcpProxy*		pTcpProxy = (TcpProxy*)vParam;
	SOCKET			sockSrc = pTcpProxy->GetSrcSock();
	SOCKET			sockDest = pTcpProxy->GetDestSock();
	while(true == pTcpProxy->IsSocketValid())
	{
		//
		// A socket in the listen() state becomes ready to read when a
		// client connects to it. An accept() will complete without
		// blocking.
		//
		int retval = 0;

		FD_ZERO(&readfds);
		FD_SET(pTcpProxy->GetSrcSock(), &readfds);
		FD_SET(pTcpProxy->GetDestSock(), &readfds);

		retval = select(0, &readfds, NULL, NULL, NULL);

		if (retval < 0)
		{
			break;
		}
		else if (retval == 0)
		{
			continue;
		}
		else
		{
			if (FD_ISSET(sockSrc, &readfds))
			{
				(void)pTcpProxy->ProxySrc2Dest();
			}

			if (FD_ISSET(sockDest, &readfds))
			{
				(void)pTcpProxy->ProxyDest2Src();
			}
		}		
	}

	return uRet;
}

bool TcpProxy::ProxyData(SOCKET sockSrc, SOCKET sockDest, Direction direction)
{
	bool bRet = false;
	char pszBuf[DEFAULT_BUFFER_LEN] = {0};
	int	nBufSize = sizeof(pszBuf);
	int nRet = 0;

	if (INVALID_SOCKET == sockSrc
		|| INVALID_SOCKET == sockDest)
	{
		printf("invalid socket.\n");
		return false;
	}

	do 
	{
		nBufSize = sizeof(pszBuf);
		ZeroMemory(pszBuf, nBufSize);

		nRet = recv(sockSrc, pszBuf, nBufSize, 0);
		if (nRet == SOCKET_ERROR)
		{
			printf("recv failed with error %u.\n", WSAGetLastError());
			return false;
		}
		else if (nRet == 0)
		{
			printf("close socket.\n");
			if (m_sockSrc != INVALID_SOCKET)
			{
				closesocket(m_sockSrc);
				m_sockSrc = NULL;
			}
			if (m_sockDest != INVALID_SOCKET)
			{
				closesocket(m_sockDest);
				m_sockDest = NULL;
			}
			return true;
		}
		else
		{
			nBufSize = nRet;
			printf("recv [%u] data [%s]\n", nBufSize, pszBuf);
		}
	} while (DEFAULT_BUFFER_LEN == nBufSize);
	
	nRet = send(sockDest, pszBuf, nBufSize, 0);
	if (nRet == SOCKET_ERROR)
	{
		printf("send failed with error %u.\n", nRet);
		return false;
	}

	bRet = true;
	return bRet;
}

bool TcpProxy::ProxySrc2Dest()
{
	return ProxyData(m_sockSrc, m_sockDest, SRC2DEST);
}

bool TcpProxy::ProxyDest2Src()
{
	return ProxyData(m_sockDest, m_sockSrc, DEST2SRC);
}

bool TcpProxy::IsSocketValid()
{
	if (m_sockSrc != INVALID_SOCKET
		&& m_sockDest != INVALID_SOCKET)
	{
		return true;
	}
	else
	{
		return false;
	}
}