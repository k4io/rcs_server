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

#define DEFAULT_BUFLEN 128
#define DEFAULT_ORDER_PORT "51001"
#define DEFAULT_CLIENT_PORT "51005"
#define DBINTERROR -69

//Local includes
//#include "discordbot.h"

struct Order
{
	std::string order_id;
	std::string order_date;
	int accessLevel;
	std::string order_paid_for;
	int uid;
};