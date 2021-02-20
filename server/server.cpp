#include "includes.h"
#include "discordbot.h"
#include "clienthandler.h"

void getOrders()
{
	orderchecker orders;
	orders.start();
}

void clients()
{
	clienthandler handler;
	handler.start();
}

void runbot()
{
	discord bot("NzQ4MzI5NjE5MDY4ODEzNDQz.X0b2RA.mn5eOh4_PwHGm12gAq0N_Iek86A", SleepyDiscord::USER_CONTROLED_THREADS);
	bot.run();
}

int main()
{
#ifdef __linux__
	char* uname = "";
	getlogin_r(uname, MAX_PATH);
	printf("<$> Hello %s!\n", uname);

#elif _WIN32
	char un[MAX_PATH];
	DWORD un_len = MAX_PATH + 1;
	GetUserName(un, &un_len);
	printf("<$> Hello %s!\n", un);

#else
	printf("<$> OS Not supported!\n<$> Contact kaio#7754.\n");
	exit(0);
#endif



	printf("<$> Initializing modules: Discord, Database handler, Client handler, Server input handler\n");
	std::thread botThread(runbot);
	botThread.detach();

	//Db test
	manager db;
	db.connect();

	//Client handling
	std::thread clientThread(clients);
	clientThread.detach();

	//order checkin
	std::thread orderThread(getOrders);
	orderThread.detach();

	for (;;) SleepEx(1,false);
}