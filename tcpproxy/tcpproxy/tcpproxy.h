#ifndef _TCP_PROXY_H_
#define _TCP_PROXY_H_

#include "common.h"

class TcpProxy
{
public:
	TcpProxy();
	~TcpProxy();

	bool Init(SOCKET sockSrc);

	SOCKET ConnectDest();

	SOCKET GetSrcSock(){return m_sockSrc;}
	SOCKET GetDestSock(){return m_sockDest;}

	void Proxy();
	static unsigned __stdcall PorxyThradProc(void*);

	bool ProxySrc2Dest();
	bool ProxyDest2Src();

	bool ProxyData(SOCKET sockSrc, SOCKET sockDests, Direction direction);

	bool IsSocketValid();

protected:

private:
	SOCKET	m_sockSrc;
	SOCKET	m_sockDest;
};

#endif
