# 1.证书的生成
```
└── somedir
├── .rnd
├── workdir
│   ├── cacert.pem
│   ├── ca.csr
│   ├── cakey.pem
│   ├── ca.srl
│   ├── clientcert.pem
│   ├── client.csr
│   ├── key.pem
│   ├── servercert.pem
│   └── server.csr
```
# 1.1 RSA证书
1. 生成随机数文件
```
cd somedir
openssl rand -writerand .rnd
cd workdir
```
2. 根证书
- 生成根证书私钥(pem文件)

```openssl genrsa -out cakey.pem 2048```

- 生成根证书签发申请文件(csr文件)

```openssl req -new -key cakey.pem -out ca.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myCA"```

- 自签发根证书(cer文件)

```openssl x509 -req -days 365 -sha1 -extensions v3_ca -signkey cakey.pem -in ca.csr -out  cacert.pem```

> cacert. pem (公有密钥)  privkey.pem(私有密钥)

3. 服务端证书
- 生成服务端私钥


```openssl genrsa -out srvkey.pem 2048```

- 生成证书请求文件


```openssl req -new -key srvkey.pem -out server.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myServer"```

- 使用根证书签发服务端证书


```openssl x509 -req -days 365 -sha1 -extensions v3_req -CA ./cacert.pem -CAkey ./cakey.pem -CAserial ca.srl -CAcreateserial -in server.csr -out servercert.pem```

- 使用CA证书验证server端证书


```openssl verify -CAfile ./cacert.pem  servercert.pem```

4. 客户端证书
- 生成客户端私钥


```openssl genrsa  -out clikey.pem 2048```

- 生成证书请求文件


```openssl req -new -key clikey.pem -out client.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myClient"```

- 使用根证书签发客户端证书


```openssl x509 -req -days 365 -sha1 -extensions v3_req -CA  ./cacert.pem -CAkey ./cakey.pem  -CAserial ./ca.srl -in client.csr -out  clientcert.pem```

- 使用CA证书验证客户端证书


```openssl verify -CAfile ./cacert.pem  clientcert.pem```

# 2.证书的作用
证书主要用于在安全通信开始之前的认证

这个程序中client和server均需要通过证书认证彼此，这些过程主要通过OpenSSL库的API完成

client和server如果需要对对方进行认证，需要有ca.crt这个文件，因为client.crt和server.crt都是通过ca.crt签发的，ca.crt用来确认crt文件的合法性

认证完成后，client和server之间会协商一个对阵加密算法，对阵加密相关信息通过非对称加密传输，之后的数据通信是对称加密的，SSL connection using AES256-GCM-SHA384

# 3.SSL安全通信流程
OpenSSL初始化过程，设置证书文件和私钥文件，设置认证方式，生成SSL上下文ctx

通过Socket API获得用于网络通信的fd，通过ctx生成ssl，绑定ssl与fd

通过SSL_read()和SSL_Write()读写ssl进行安全通信
