#pragma once
#include "includes.h"
#include "dbmanager.h"
//#include <stdio.h>
//#include <iostream>

class discord : public SleepyDiscord::DiscordClient {
public:

	std::string auth = "empty";

	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message msg) override {
		manager db;
		if (msg.startsWith(".info")) {
			sendMessage(msg.channelID, "> *Smoke'em*\n> Store: https://shoppy.gg/@smokedrugs69 \n> If you have purchased a package from the store please wait up to **10 minutes** and message *this bot* `.help.`");
		}
		else if (msg.startsWith(".help"))
		{
			if (msg.serverID.string().length() > 1)
			{
				sendMessage(msg.channelID, "> *Sorry* but this command is only available in PM!");
				return;
			}
			std::string s = "> Once you have purchased the script you can simply register and retrieve your download link almost instantly.\n> Our bot retrieves new orders every 10 minutes so you won't be waiting long.\n> For more information on how to sign-up please see here -> https://i.imgur.com/4aDwj3f.png\n > Please type `.auth *orderid*` when you first message the bot otherwise you wont be able to register.";
			sendMessage(msg.channelID, s);
		}
		else if (msg.startsWith(".unlock") && msg.content.length() > 8)
		{
			std::string tmp = db.explode(msg.content, ' ')[1];
			if (db.isAccountLocked(tmp) != 1)
			{
				std::string s = "> Account  *" + tmp + "*  is not locked!";
				sendMessage(msg.channelID, s);
				return;
			}
			else {
				db.unlockAccount(tmp);
				std::string s = "> Unlocked  **" + tmp + "!**";
				sendMessage(msg.channelID, s);
				return;
			}
		}
		else if (msg.startsWith(".locked") && msg.content.length() > 8)
		{
			std::string tmp = db.explode(msg.content, ' ')[1];
			if (db.isAccountLocked(tmp) == 1)
			{
				std::string s = "> Account  *" + tmp + "*  was locked for reasons: " + db.getLockedReason(tmp);
				sendMessage(msg.channelID, s);
				return;
			} else {
				std::string s = "> Account  *" + tmp + "*  is not locked!";
				sendMessage(msg.channelID, s);
			}
		}
		else if (msg.startsWith(".auth"))
		{
			if (msg.serverID.string().length() > 1)
			{
				sendMessage(msg.channelID, "> *Sorry* but this command is only available in PM!");
				return;
			}
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
			std::string st = "> *Thank-you!* \n> \n> The order-id `";
			st += tmp;
			st += "` has been accepted, please type `.reg *username* *password*` to register an account with the bot.";
			sendMessage(msg.channelID, st);
		}
		else if (msg.startsWith(".reg"))
		{
			if (msg.serverID.string().length() > 1)
			{
				sendMessage(msg.channelID, "> *Sorry* but this command is only available in PM!");
				return;
			}
			if (auth == "empty")
			{
				sendMessage(msg.channelID, "> *Sorry* but you cannot register without a valid order-id!");
				return;
			}
			if (msg.content.length() < 16)
			{
				sendMessage(msg.channelID, "> *Sorry* but that is an invalid pair of settings!");
				return;
			}
			if(db.explode(msg.content, ' ').size() != 3)
			{
				sendMessage(msg.channelID, "> *Sorry* but that is an invalid pair of settings!");
				return;
			}


			std::string uname = db.explode(msg.content, ' ')[1];
			std::string passwd = db.explode(msg.content, ' ')[2];
			std::string duid = msg.author.ID;
			int accessLevel = db.getOrder(auth).accessLevel;
			MD5 md5;

			std::string badchars = "";
			for (auto a : db.badchars)
				badchars += a;

			for (auto a : db.badchars)
				for (auto c : uname)
					for (auto t : passwd)
						if (c == a || t == a) {
							std::string s = "> *Sorry* but your username or password cannot contain these characters: " + badchars + "!";
							sendMessage(msg.channelID, s);
							return;
						}

			std::string pwdhash = md5.digestString(strdup(passwd.c_str()));
			if (db.doesUsernameExist(uname))
			{
				sendMessage(msg.channelID, "> *Sorry*  but that username already exists!");
				return;
			}
			if (db.doesDuidExist(duid))
			{
				sendMessage(msg.channelID, "> *Sorry* \n > Your discord account is already associated with an account in our database, please contact kaio#7754!");
				return;
			}

			db.addUser(uname, pwdhash, accessLevel, "not_set", duid);

			if (!db.isOrderPaidFor(auth))
				sendMessage(msg.channelID, "> **WARNING**\n> \n> *This order has not yet been paid for, meaning you will not be able to use the software!*");
			else {
				db.setSubDate("NOW()", accessLevel, db.getUid(uname));
				
				//set discord role
				std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Role>> roleToAdd;
				SleepyDiscord::Role _r;

				switch (accessLevel)
				{
				case 1: //vip 30d
					_r.name = "Premium";
					break;
				case 2: //vip+ 30d
					_r.name = "Premium+";
					break;
				case 3: //vip 6m
					_r.name = "Premium";
					break;
				case 4: //vip+ 6m
					_r.name = "Premium+";
					break;
				}

				editMember("745677254499106826", msg.author, "", roleToAdd, 0, 0, msg.channelID);
				
			}
				//db.ExecuteNonQuery("UPDATE users SET subscriptionStart=DATE(NOW()) WHERE uid=" + std::to_string(db.getUid(uname)) + ";");

			db.redeemOrder(auth, uname);
			//success send download permalink
			sendMessage(msg.channelID, "> *Thank-you* for purchasing *smoke'em*, here is your download link: \n> \n> You can sign-in with the username and password you have provided!");
		}
		SleepEx(150, false);
	}
};