#pragma once
#include "includes.h"
#include "dbmanager.h"



class clienthandler {
public:
	std::vector<SSL*> clientlist;
	klog out;
	clienthandler();
	~clienthandler();
	void start();
	void stop();
	void init_ssl();
	void cleanup();
	void configure_context(SSL_CTX *ctx);
	int create_socket(int port);
	SSL_CTX* create_context();
	void handleConnection();
	void sendpacket(SSL* _ssl, std::string request);
	std::string encryptDecrypt(std::string toEncrypt);
	int receivepacket(SSL* _ssl, char buf[BuffSize]);
private:
	manager db;
	std::string clientAddr;
	SSL* ssl;
	int client;
	char reply[1024];
};

class orderchecker
{
public:
	orderchecker();
	~orderchecker();
	int start();
	void stop();
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
};


class accountchecker
{
public:
	char* strptime(const char* s, const char* f, struct tm* tm) {
		std::istringstream input(s);
		input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
		input >> std::get_time(tm, f);
		if (input.fail()) {
			return nullptr;
		}
		return (char*)(s + input.tellg());
	}
	accountchecker();
	~accountchecker();
	void start();
	void stop();
};