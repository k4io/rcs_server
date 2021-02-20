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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <winsock2.h>
#include <chrono>
#include <ctime>
#include <Wincrypt.h>
#include <iomanip>
#include <sstream>
#include <time.h>
#include "md5.h"

#define DEFAULT_BUFLEN 128
#define DEFAULT_ORDER_PORT "51001"
#define DEFAULT_CLIENT_PORT "51005"
#define DBINTERROR -1

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