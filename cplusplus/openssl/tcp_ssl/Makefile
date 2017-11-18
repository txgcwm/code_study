all:
	g++ -o ssl_server ssl_server.cpp common.cpp -lssl -lcrypto
	g++ -o ssl_client ssl_client.cpp common.cpp -lssl -lcrypto

clean:
	rm -f ssl_server ssl_client