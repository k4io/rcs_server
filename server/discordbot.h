#pragma once
#include "includes.h"
#include "dbmanager.h"
#include "clienthandler.h"
//#include <stdio.h>
//#include <iostream>

class discord : public SleepyDiscord::DiscordClient {
public:
	//clienthandler ch;
	int nb = 0;
	std::string auth = "empty";
	using SleepyDiscord::DiscordClient::DiscordClient;
	void statuschanger()
	{
		manager db;
		while(true)
		{
			int useramount = 0;
			std::vector<std::string> _vss = db.explode(db.getAllUids(), '\n');
			for (auto a : _vss)
				useramount++;

			int pplus = 0;
			_vss = db.explode(db.getAllPremiumPlus(), '\n');
			for (auto a : _vss)
				pplus++;


			std::string _usStr = "Users: " + std::to_string(useramount) + " | Premium+ Slots: " + std::to_string(pplus) + "/50";
			updateStatus(_usStr, 0, SleepyDiscord::online, false);
			
			SleepEx(30000, false);
			//std::this_thread::sleep_for(std::chrono::milliseconds(30000));

			useramount = 0;
			_vss = db.explode(db.getAllUids(), '\n');
			for (auto a : _vss)
				useramount++;

			int p = 0;
			_vss = db.explode(db.getAllPremium(), '\n');
			for (auto a : _vss)
				p++;

			_usStr = "Users: " + std::to_string(useramount) + " | Premium Slots: " + std::to_string(p) + "/50";
			updateStatus(_usStr, 0, SleepyDiscord::online, false);
			SleepEx(30000, false);
		}
	}
	//using SleepyDiscord::DiscordClient::DiscordClient;
	void onReady(SleepyDiscord::Ready rdyDat) override
	{
		std::thread _t([this] { this->statuschanger(); });
		_t.detach();
	}


	void onMessage(SleepyDiscord::Message msg) override {
		manager db;
		klog log;
		//updateStatus("oooooooooooooooOOOOOOOOOOOO", 30, SleepyDiscord::online, false);
		if (msg.startsWith(".info")) {
			log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .info");
			sendMessage(msg.channelID, "> *Smoke'em*\n> Store: https://shoppy.gg/@smokedrugs69 \n> If you have purchased a package from the store please wait up to **10 minutes** and message *this bot* `.help.`");
		}
		else if (msg.startsWith(".help"))
		{
			log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .help");
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
			if (msg.author.ID != "147792783846014976" && msg.author.ID != "156508892854091776")
			{
				return;
			}
			log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .unlock");
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
			if (msg.author.ID != "147792783846014976" && msg.author.ID != "156508892854091776")
			{
				return;
			}
			log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .locked");
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
			log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .auth");
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
			try {
				log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " called .reg");
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
				if (db.explode(msg.content, ' ').size() != 3)
				{
					sendMessage(msg.channelID, "> *Sorry* but that is an invalid pair of settings!");
					return;
				}


				std::string uname = db.explode(msg.content, ' ')[1];
				std::string passwd = db.explode(msg.content, ' ')[2];
				std::string duid = msg.author.ID;
				int accessLevel = db.getOrder(auth).accessLevel;
				//MD5 md5;

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

				std::string pwdhash = db.hash(passwd);
				log.out(0, "[mysql] hash() called with data: " + passwd + " returned: " + pwdhash);
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

				log.out(0, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " has created an account with the id: " + std::to_string(db.getUid(uname)));
				if (!db.isOrderPaidFor(auth)) {
					log.out(1, "[discord] " + msg.author.username + "#" + msg.author.discriminator + " has redemeed order " + auth + " that is not yet paid for");
					sendMessage(msg.channelID, "> **WARNING**\n> \n> *This order has not yet been paid for, meaning you will not be able to use the software!*");
				}
				else {
					db.setSubDate("NOW()", accessLevel, db.getUid(uname));
					/*
				SleepyDiscord::Role _p;
				SleepyDiscord::Role _pr;

				for (auto r : roles) {
					if (r.ID == 745683977117106246)
					{
						//premium
						_p = r;
					}
					else if (r.ID == 745683179083661446)
					{
						//premium+
						_pr = r;
					}
				}

				std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Role>> _snow;
				//set discord role

				switch (accessLevel)
				{
				case 1: //vip 30d
					_snow = { _p };
					break;
				case 2: //vip+ 30d
					_snow = { _pr };
					break;
				case 3: //vip 6m
					_snow = { _p };
					break;
				case 4: //vip+ 6m
					_snow = { _pr };
					break;
				}

				//editMember("745677254499106826", msg.author, "", _snow, 0, 0, msg.channelID);*/

				}
				//db.ExecuteNonQuery("UPDATE users SET subscriptionStart=DATE(NOW()) WHERE uid=" + std::to_string(db.getUid(uname)) + ";");

				db.redeemOrder(auth, uname);
				auth = "empty";
				//success send download permalink
				sendMessage(msg.channelID, "> *Thank-you* for purchasing *smoke'em*, here is your download link: \n> \n> You can sign-in with the username and password you have provided!");
			}
			catch (...)
			{
				sendMessage(msg.channelID, "> *Sorry*  There was an error!");
				auth = "empty";
				return;
			}
		}
		SleepEx(150, false);
	}
};