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
## 1.1 RSA证书
### 1.1.1 生成随机数文件
```
cd somedir
openssl rand -writerand .rnd
cd workdir
```
### 1.1.2 根证书
- 生成根证书私钥(pem文件)

```openssl genrsa -out cakey.pem 2048```

- 生成根证书签发申请文件(csr文件)

```openssl req -new -key cakey.pem -out ca.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myCA"```

- 自签发根证书(cer文件)

```openssl x509 -req -days 365 -sha1 -extensions v3_ca -signkey cakey.pem -in ca.csr -out  cacert.pem```

> cacert. pem (公有密钥)  privkey.pem(私有密钥)

### 1.1.3 服务端证书
- 生成服务端私钥


```openssl genrsa -out srvkey.pem 2048```

- 生成证书请求文件


```openssl req -new -key srvkey.pem -out server.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myServer"```

- 使用根证书签发服务端证书


```openssl x509 -req -days 365 -sha1 -extensions v3_req -CA ./cacert.pem -CAkey ./cakey.pem -CAserial ca.srl -CAcreateserial -in server.csr -out servercert.pem```

- 使用CA证书验证server端证书


```openssl verify -CAfile ./cacert.pem  servercert.pem```

### 1.1.4 客户端证书
- 生成客户端私钥


```openssl genrsa  -out clikey.pem 2048```

- 生成证书请求文件


```openssl req -new -key clikey.pem -out client.csr -subj "/C=CN/ST=myprovince/L=mycity/O=myorganization/OU=mygroup/CN=myClient"```

- 使用根证书签发客户端证书


```openssl x509 -req -days 365 -sha1 -extensions v3_req -CA  ./cacert.pem -CAkey ./cakey.pem  -CAserial ./ca.srl -in client.csr -out  clientcert.pem```

- 使用CA证书验证客户端证书


```openssl verify -CAfile ./cacert.pem  clientcert.pem```
