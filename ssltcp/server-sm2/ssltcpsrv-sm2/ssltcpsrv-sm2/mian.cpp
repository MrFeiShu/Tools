#include <WinSock2.h>
#include <Windows.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

int create_socket(int port)
{
	int s;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		perror("Unable to create socket");
		exit(EXIT_FAILURE);
	}

	if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("Unable to bind");
		exit(EXIT_FAILURE);
	}

	if (listen(s, 1) < 0) {
		perror("Unable to listen");
		exit(EXIT_FAILURE);
	}

	return s;
}

SSL_CTX *create_context()
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	method = TLS_server_method();

	ctx = SSL_CTX_new(method);
	if (!ctx) {
		perror("Unable to create SSL context");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	return ctx;
}

void configure_context(SSL_CTX *ctx)
{
	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(ctx, "E:/github/Tools/ssltcp/server-sm2/ssltcpsrv-sm2/Debug/servercert.crt", SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	if (SSL_CTX_use_PrivateKey_file(ctx, "E:/github/Tools/ssltcp/server-sm2/ssltcpsrv-sm2/Debug/srvkey.pem", SSL_FILETYPE_PEM) <= 0 ) {
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	int sock;
	SSL_CTX *ctx;
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}

	ctx = create_context();

	configure_context(ctx);

	sock = create_socket(20000);

	/* Handle connections */
	while(1) {
		struct sockaddr_in addr;
		int len = sizeof(addr);
		SSL *ssl;
		const char reply[] = "test\n";
		X509*    client_cert;

		int client = accept(sock, (struct sockaddr*)&addr, &len);
		if (client < 0) {
			perror("Unable to accept");
			exit(EXIT_FAILURE);
		}

		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, client);
		client_cert = SSL_get_peer_certificate (ssl);

		if (SSL_accept(ssl) <= 0) {
			ERR_print_errors_fp(stderr);
		} else {
			SSL_write(ssl, reply, strlen(reply));
		}

		SSL_shutdown(ssl);
		SSL_free(ssl);
		closesocket(client);
	}

	closesocket(sock);
	SSL_CTX_free(ctx);
}