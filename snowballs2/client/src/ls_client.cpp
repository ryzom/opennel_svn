/**
 * \file ls_client.cpp
 * \brief CLSClient
 * \date 2008-02-06 18:45GMT
 * \author Jan Boon (Kaetemi)
 * CLSClient, based on CLoginClient
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
 * Based on CLoginClientMtp, MTP Target, Copyright (C) 2003  Melting Pot
 * Based on CLoginClient, NEVRAX NEL, Copyright (C) 2000  Nevrax Ltd.
 * 
 * This file is part of OpenNeL Snowballs.
 * OpenNeL Snowballs is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 * 
 * OpenNeL Snowballs is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenNeL Snowballs; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#include <nel/misc/types_nl.h>
#include "ls_client.h"

// Project includes
#include "evil_singleton_impl.h"

// NeL includes
// #include <nel/misc/debug.h>
#include <nel/misc/md5.h>
#include <nel/net/callback_client.h>

// STL includes

using namespace std;
using namespace NLMISC;
using namespace NLNET;

namespace SBCLIENT {

CLSClient::CLSClient() : _Callback(NULL), _LSCallbackClient(NULL)
{
	SBCLIENT_EVIL_SINGLETON_CONSTRUCTOR(SBCLIENT::CLSClient);
}

CLSClient::~CLSClient()
{
	disconnect();
	SBCLIENT_EVIL_SINGLETON_DESTRUCTOR(SBCLIENT::CLSClient);
}

void CLSClient::update()
{
	// do nothing if we're not waiting for anything
	if (!_Callback) 
	{
		// if we're not waiting, but the connection exists
		// without a shardlist, this means that we need
		// to disconnect from the server
		if (_LSCallbackClient && ShardList.empty())
			_disconnect();
		return;
	}
	
	if (!_LSCallbackClient)
	{
		LastError = "No connection created to LS";
		callback(); return;
	}
	
	if (!_LSCallbackClient->connected())
	{
		_disconnect();
		LastError = "Disconnected from LS";
		callback(); return;
	}
	
	_LSCallbackClient->update();
}

void CLSClient::disconnect()
{
	_Callback = NULL;
	ShardList.clear();
	_disconnect();
}

void CLSClient::_disconnect()
{
	if (_LSCallbackClient)
	{
		if (_LSCallbackClient->connected())
			_LSCallbackClient->disconnect();
		delete _LSCallbackClient;
		_LSCallbackClient = NULL;
	}
}

void CLSClient::authenticateUser(SBCLIENT_CALLBACK cb, void *context, void *tag, const string &ls, const ucstring &login, const string &cpassword, const string &application)
{
	nlassert(!_Callback); _Callback = cb; nlassert(_Callback);
	_Context = context; _Tag = tag;

	LastError = "";

	try
	{
		_disconnect();
		string addr = ls;
		if(addr.find(":") == string::npos)
			addr += ":49997";
		_LSCallbackClient = new CCallbackClient();
		_LSCallbackClient->connect(CInetAddress(addr));
		_LSCallbackClient->addCallbackArray(_LSCallbackArray, 
			sizeof(_LSCallbackArray) / sizeof(_LSCallbackArray[0]));
	}
	catch (ESocket &e)
	{
		_disconnect();
		LastError = toString("Connection failed to LS (addr:%s): %s", ls.c_str(), e.what());
		callback(); return;
	}
	
	// create and send the VLP message
	CMessage msgout("VLP");
	msgout.serial(const_cast<ucstring&>(login));
	msgout.serial(const_cast<string&>(cpassword));
	msgout.serial(const_cast<string&>(application));
	_LSCallbackClient->send(msgout);

	return;
}

void CLSClient::selectShard(SBCLIENT_CALLBACK cb, void *context, void *tag, sint32 shardId)
{
	nlassert(!_Callback); _Callback = cb; nlassert(_Callback);
	_Context = context; _Tag = tag;

	if (!_LSCallbackClient)
	{
		LastError = "No connection created to LS";
		callback(); return;
	}

	if (!_LSCallbackClient->connected())
	{
		_disconnect();
		LastError = "Disconnected from LS";
		callback(); return;
	}

	if (!ShardList.size())
	{
		_disconnect();
		LastError = "No shard available";
		callback(); return;
	}

	CLSClient::CShard *shard = getShard(shardId);
	if (!shard)
	{
		_disconnect();
		LastError = "Invalid shard selected";
		callback(); return;
	}

	// create and send the CS message with the ShardId choice to the LS
	CMessage msgout("CS");
	msgout.serial(shard->ShardId);
	_LSCallbackClient->send(msgout);
}

CLSClient::CShard *CLSClient::getShard(uint32 shardId)
{
	for(TShardList::iterator it = ShardList.begin(); it != ShardList.end(); it++)
		if(it->ShardId == shardId) return &(*it);
	return NULL;
}

string CLSClient::encryptPassword(const ucstring &password)
{
	CHashKeyMD5 hk = getMD5((uint8 *)password.c_str(), password.size());
	string cpassword = hk.toString();
	nlinfo("The encrypted password is %s", cpassword.c_str());
	return cpassword;
}

void CLSClient::callback()
{
	nlassert(_Callback);
	if (!LastError.empty())
		nlwarning("CLSClient::callback(): %s", LastError.c_str());
	SBCLIENT_CALLBACK cb = _Callback;
	_Callback = NULL;
	cb(_Context, _Tag);
}

TCallbackItem CLSClient::_LSCallbackArray[] = {
	{ "VLP", CLSClient::cbAuthenticateUser }, 
	{ "SCS", CLSClient::cbSelectShard }, 
};

void CLSClient::cbAuthenticateUser(CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
	CLSClient &ls_client = CLSClient::getInstance();
	msgin.serial(ls_client.LastError);
	if (ls_client._Callback)
	{
		if (ls_client.LastError.empty())
		{
			uint32 nb_shard;
			msgin.serial(nb_shard);

			if (!nb_shard) ls_client.LastError = "No shard available";
			else
			{
				ls_client.ShardList.clear();
				for (uint i = 0; i < nb_shard; i++)
				{
					CShard shard;
					msgin.serial(
						shard.Name, 
						shard.NbPlayers, 
						shard.ShardId);
					shard.Online = true;
					shard.Version = "";
					shard.DynPatchURL = "";
					// todo: implement this if nobody has problems with it...
					//  client ls connection is implemented waaaay too different
					//  from the old http login connection :/  (kinda useless)
					//  oh and SERIOUSLY, NbPlayers in a uint8?
					//msgin.serial(
					//	shard.ShardId,
					//	shard.NbPlayers,
					//	shard.Name,
					//	shard.Online,
					//	shard.Version,
					//	shard.DynPatchURL);
					ls_client.ShardList.push_back(shard);
				}
			}
		}
		ls_client.callback();
	}
	else
	{
		nlwarning("CLSClient::cbAuthenticateUser: VLP unexpected (%s)", ls_client.LastError.c_str());
	}
}

void CLSClient::cbSelectShard(CMessage &msgin, TSockId from, CCallbackNetBase &netbase)
{
	CLSClient &ls_client = CLSClient::getInstance();
	msgin.serial(ls_client.LastError);
	if (ls_client._Callback)
	{
		if (ls_client.LastError.empty())
		{
			std::string cookie; // hm wasn't it just possible to
			msgin.serial(cookie); // serialize cookie itself?
			ls_client.LoginCookie.setFromString(cookie);
			msgin.serial(ls_client.FrontEnd);
		}
		ls_client.callback();
	}
	else
	{
		nlwarning("CLSClient::cbSelectShard: SCS unexpected (%s)", ls_client.LastError.c_str());
	}
}

SBCLIENT_EVIL_SINGLETON_IMPL(SBCLIENT::CLSClient);

} /* namespace SBCLIENT */

/* end of file */
