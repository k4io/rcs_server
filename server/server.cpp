#include "includes.h"
#include "discordbot.h"
#include "clienthandler.h"

void getOrders()
{
	orderchecker orders;
	orders.start();
}

void clients(manager db)
{
	clienthandler handler;
	handler.start();
}

void runbot()
{
	discord bot("NzQ4MzI5NjE5MDY4ODEzNDQz.X0b2RA.mn5eOh4_PwHGm12gAq0N_Iek86A", SleepyDiscord::USER_CONTROLED_THREADS);

	bot.run();
}

void checkAccounts()
{
	accountchecker ac;
	ac.start();
}
int main()
{
	klog log;
	log.init();

	log.out(0, "[main] Starting...");
	
	std::thread botThread(runbot);
	botThread.detach();

	//Db test
	manager db;
	db.connect();

	//Client handling
	std::thread clientThread(clients, db);
	clientThread.detach();
	//order checkin
	std::thread orderThread(getOrders);
	orderThread.detach();

	//account checkin
	std::thread accThread(checkAccounts);
	accThread.detach();
	log.out(0, "[main] Ready");

	for (;;) SleepEx(1,false);
}