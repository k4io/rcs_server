#pragma once
#include <stdio.h>
#include <iostream>
#include "sleepy_discord/sleepy_discord.h"
#include <Windows.h>
#include <map>
#include <thread>
#include <random>
#include <Lmcons.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string>
#include <sys/types.h> 
#include <winsock2.h>
#include <chrono>
#include <ctime>
#include <Wincrypt.h>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <time.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/sha.h>
#include "md5.h"

#pragma comment(lib, "ws2_32.lib")

#define BuffSize 1024

#define DEFAULT_BUFLEN 128
#define DEFAULT_ORDER_PORT "51001"
#define DEFAULT_CLIENT_PORT "51005"
#define DBINTERROR -1

#define CurrentClientVersion "v0.1"

#define SL_INFO = 0
#define SL_WARN = 1
#define SL_ERROR = 2
#define SL_FATAL = 3

class klog
{
public:
	static void init()
	{
		time_t now = time(0);
		struct tm* ntm = gmtime(&now);
		std::string _time = std::to_string(ntm->tm_year + 1900) + "-" + std::to_string(ntm->tm_mon + 1) + "-" + std::to_string(ntm->tm_mday) + "-" + std::to_string(ntm->tm_hour) + "-" + std::to_string(ntm->tm_min) + "-" + std::to_string(ntm->tm_sec);
		std::string fname = "logs\\log " + _time + ".log";

		//freopen(fname.c_str(), "w", stderr);
		freopen(fname.c_str(), "w", stdout);
		/*
		std::ofstream out(fname.c_str());
		std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
		std::ofstream out(fname.c_str());
		std::streambuf* coutbuf = std::cerr.rdbuf();
		std::cerr.rdbuf(out.rdbuf());*/
	}
	static void out(int sl, std::string msg)
	{
		time_t now = time(0);
		struct tm* ntm = gmtime(&now);

		std::string _time = std::to_string(ntm->tm_year + 1900) + "-" + std::to_string(ntm->tm_mon + 1) + "-" + std::to_string(ntm->tm_mday) + " " + std::to_string(ntm->tm_hour) + ":" + std::to_string(ntm->tm_min) + ":" + std::to_string(ntm->tm_sec);
		//std::string fname = "logs\\log " + _time + ".log";

		switch (sl)
		{
		case 0:
			//printf("[%s]	[info]: %s\n", _time.c_str(), msg.c_str());
			std::cerr << "[" << _time.c_str() << "]  [info]: " << msg.c_str() << std::endl;
			break;
		case 1:
			//printf("[%s]	[warning]: %s\n", _time.c_str(), msg.c_str());
			std::cerr << "[" << _time.c_str() << "]  [warning]: " << msg.c_str() << std::endl;
			break;
		case 2:
			//printf("[%s]	[error]: %s\n", _time.c_str(), msg.c_str());
			std::cerr << "[" << _time.c_str() << "]  [error]: " << msg.c_str() << std::endl;
			break;
		case 3:
			//printf("[%s]	[fatal]: %s\n", _time.c_str(), msg.c_str());
			std::cerr << "[" << _time.c_str() << "]  [fatal]: " << msg.c_str() << std::endl;
			break;
		}
	}
};

struct Order
{
	std::string order_id;
	std::string order_date;
	int accessLevel;
	int order_paid_for;
	int uid;
};

enum HashType
{
    HashSha1, HashMd5, HashSha256
};