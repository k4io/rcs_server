#pragma once
#include "includes.h"
#include "dbmanager.h"
//#include <stdio.h>
//#include <iostream>

class discord : public SleepyDiscord::DiscordClient {
public:

	std::string auth = "";

	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message msg) override {
		manager db;
		if (msg.startsWith(".info")) {
			sendMessage(msg.channelID, "> *Smoke'em*\n> Store: https://shoppy.gg/@smokedrugs69 \n> If you have purchased a package from the store please wait up to **10 minutes** and message *this bot* `!help.`");
		}
		else if (msg.startsWith(".auth"))
		{
			if (msg.content.length() < 16)
			{
				sendMessage(msg.channelID, "> *Sorry* but that order-id is invalid!");
				return;
			}
			std::string tmp = db.explode(msg.content, ' ')[1];

			if (!db.doesOrderExist(tmp))
			{
				std::string st = "> *Sorry* but the order-id `";
				st+=tmp;
				st+="` is invalid!";
				sendMessage(msg.channelID, st);
				return;
			}
			else if (db.isOrderRedeemed(tmp))
			{
				std::string st = "> *Sorry* but the order-id `";
				st += tmp;
				st += "` has already been redeemed!\n\n> If you believe this to be wrongful please contact one of the admins in the discord, or kaio#7754.";
				sendMessage(msg.channelID, st);
				return;
			}
			auth = tmp;
		}
		else if (msg.startsWith(".reg"))
		{
			//check orderid is first valid then check if it is not already redeemed
			auth = msg.content;
		}
	}
};