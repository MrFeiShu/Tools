#include <WinSock2.h>
#include <Windows.h>

#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

extern "C"
{
#include <openssl/applink.c>
}

/* define HOME to be dir for key and cert files... */
#define HOME "./"
/* Make these what you want for cert & key files */
#define CERTF	"E:/github/Tools/ssltcp/certs/servercert.crt"
#define KEYF	"E:/github/Tools/ssltcp/certs/srvkey.pem"
#define CACERT	"E:/github/Tools/ssltcp/certs/cacert.crt"


#define CHK_NULL(x) if ((x)==NULL) return 1;
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); return 2; }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); return 3; }

int main ()
{
  int err;
  int sockListen;
  int sockClient;
  struct sockaddr_in sa_serv;
  struct sockaddr_in sa_cli;
  int client_len;
  SSL_CTX* ctx;
  SSL*     ssl;
  X509*    client_cert;
  char*    str;
  char     buf [4096];
  SSL_METHOD *meth;
  WORD wVersionRequested;
  WSADATA wsaData;

  wVersionRequested = MAKEWORD(2, 2);
  err = WSAStartup(wVersionRequested, &wsaData);
  if (err != 0) {
	  /* Tell the user that we could not find a usable */
	  /* Winsock DLL.                                  */
	  printf("WSAStartup failed with error: %d\n", err);
	  return 1;
  }

  /* SSL preliminaries. We keep the certificate and key with the context. */

  SSL_load_error_strings();
  SSLeay_add_ssl_algorithms();
  meth = (SSL_METHOD*)SSLv23_server_method();
  ctx = SSL_CTX_new (meth);
  if (!ctx) {
    ERR_print_errors_fp(stderr);
    exit(2);
  }

  SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL); /* whether verify the certificate */
  err = SSL_CTX_load_verify_locations(ctx,CACERT,NULL);
  CHK_ERR(err, "SSL_CTX_load_verify_locations");

  if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(3);
  }
  if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(4);
  }

  if (!SSL_CTX_check_private_key(ctx)) {
    fprintf(stderr,"Private key does not match the certificate public key\n");
    exit(5);
  }

  /* ----------------------------------------------- */
  /* Prepare TCP socket for receiving connections */
  
  sockListen = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (INVALID_SOCKET == sockListen)
  {
	  printf("socket failed with error %u.\n", WSAGetLastError());
  }
  CHK_ERR(sockListen, "socket");

  memset (&sa_serv, '\0', sizeof(sa_serv));
  sa_serv.sin_family      = AF_INET;
  sa_serv.sin_addr.s_addr = INADDR_ANY;
  sa_serv.sin_port        = htons (20000);          /* Server Port number */

  err = bind(sockListen, (struct sockaddr*) &sa_serv, sizeof (sa_serv));
  if (SOCKET_ERROR == err)
  {
	  printf("bind failed with error %u.\n", WSAGetLastError());
  }
  CHK_ERR(err, "bind");

  /* Receive a TCP connection. */

  err = listen (sockListen, 5);
  if (SOCKET_ERROR == err)
  {
	  printf("listen failed with error %u.\n", WSAGetLastError());
  }
  CHK_ERR(err, "listen");

  client_len = sizeof(sa_cli);
  sockClient = accept (sockListen, (struct sockaddr*) &sa_cli, &client_len);
  if (INVALID_SOCKET == sockClient)
  {
	  printf("accept failed with error %u.\n", WSAGetLastError());
  }
  CHK_ERR(sockClient, "accept");
  closesocket (sockListen);

  printf ("Connection from %lx, port %x\n",
          sa_cli.sin_addr.s_addr, sa_cli.sin_port);

  /* ----------------------------------------------- */
  /* TCP connection is ready. Do server side SSL. */

  ssl = SSL_new (ctx);
  CHK_NULL(ssl);

  SSL_set_fd (ssl, sockClient);
  err = SSL_accept (ssl);
  CHK_SSL(err);

  /* Get the cipher - opt */

  printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

  /* Get client's certificate (note: beware of dynamic allocation) - opt */

  client_cert = SSL_get_peer_certificate (ssl);
  if (client_cert != NULL) {
    printf ("Client certificate:\n");

    str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
    CHK_NULL(str);
    printf ("\t subject: %s\n", str);
    OPENSSL_free (str);

    str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
    CHK_NULL(str);
    printf ("\t issuer: %s\n", str);
    OPENSSL_free (str);

    /* We could do all sorts of certificate verification stuff here before
       deallocating the certificate. */

    X509_free (client_cert);
  } else
    printf ("Client does not have certificate.\n");

  /* DATA EXCHANGE - Receive message and send reply. */

  err = SSL_read (ssl, buf, sizeof(buf) - 1);
  CHK_SSL(err);

  buf[err] = '\0';
  printf ("Got %d chars:'%s'\n", err, buf);

  err = SSL_write (ssl, "I hear you.", strlen("I hear you."));
  CHK_SSL(err);

  /* Clean up. */

  closesocket (sockClient);
  SSL_free (ssl);
  SSL_CTX_free (ctx);

  WSACleanup();

  return 0;
}
/* EOF - serv.cpp */
