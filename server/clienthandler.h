#pragma once
#include "includes.h"
#include "dbmanager.h"

class clienthandler {
public:
	clienthandler();
	~clienthandler();
	void start();
	void stop();
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