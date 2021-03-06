运行脚本`/usr/lib/ssl/misc/CA.sh -newca`，根据提示填写信息完成后会生成一个`demoCA`的目录，里面存放了`CA`的证书和私钥。

生成客户端和服务器证书申请：
```
$ openssl req -newkey rsa:1204 -out sslclientreq.pem -keyout sslclientkey.pem
$ openssl req -newkey rsa:1204 -out sslserverreq.pem -keyout sslserverkey.pem
```

签发客户端和服务器证书：
```
$ openssl ca -in sslclientreq.pem -out sslclientcert.pem
$ openssl ca -in sslserverreq.pem -out sslservercert.pem
```

```
$ g++ -o ssl_server ssl_server.cpp common.cpp -lssl -lcrypto
$ g++ -o ssl_client ssl_client.cpp common.cpp -lssl -lcrypto
```

```
$ ./ssl_server -m 1 -p 6666 -c ca/demoCA/cacert.pem -k ca/demoCA/private/cakey.pem
$ ./ssl_client -m 1 -p 6666 -s 127.0.0.1
```