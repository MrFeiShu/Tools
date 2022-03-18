# 1.证书的生成
1. 生成server.csr和client.csr

openssl genrsa -des3 -out server.key 1024

openssl req -new -key server.key -out server.csr

openssl genrsa -des3 -out client.key 1024

openssl req -new -key client.key -out client.csr

2. 生成ca.crt
openssl genrsa -des3 -out ca.key 1024

openssl req -new -x509 -key ca.key -out ca.crt

3. 通过ca.crt颁发server.crt和client.crt

openssl ca -in server.csr -out server.crt -cert ca.crt -keyfile ca.key

openssl ca -in client.csr -out client.crt -cert ca.crt -keyfile ca.key

注：

这两条执行的时候可能因为没有指定openssl.cnf 会报错

不过没关系，我们用默认的 /etc/pki/tls/openssl.cnf 就可以

不过用默认的时候需要先执行下面两行：

touch /etc/pki/CA/index.txt

echo 00 > /etc/pki/CA/serial

# 2.证书的作用
证书主要用于在安全通信开始之前的认证

这个程序中client和server均需要通过证书认证彼此，这些过程主要通过OpenSSL库的API完成

client和server如果需要对对方进行认证，需要有ca.crt这个文件，因为client.crt和server.crt都是通过ca.crt签发的，ca.crt用来确认crt文件的合法性

认证完成后，client和server之间会协商一个对阵加密算法，对阵加密相关信息通过非对称加密传输，之后的数据通信是对称加密的，SSL connection using AES256-GCM-SHA384

# 3.SSL安全通信流程
OpenSSL初始化过程，设置证书文件和私钥文件，设置认证方式，生成SSL上下文ctx

通过Socket API获得用于网络通信的fd，通过ctx生成ssl，绑定ssl与fd

通过SSL_read()和SSL_Write()读写ssl进行安全通信
