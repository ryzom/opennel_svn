/**
 * \file ls_client.h
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

#ifndef SBCLIENT_LS_CLIENT_H
#define SBCLIENT_LS_CLIENT_H
#include <nel/misc/types_nl.h>

// Project includes
#include "member_callback_type.h"
#include "evil_singleton_decl.h"

// NeL includes
#include <nel/net/login_cookie.h>
#include <nel/net/callback_net_base.h>
#include <nel/misc/mutex.h>

// STL includes

namespace NLNET {
	class CCallbackClient;
}

namespace NLMISC {
	class IThread;
}

namespace SBCLIENT {

/**
 * \brief CLSClient
 * \date 2008-02-06 18:45GMT
 * \author Jan Boon (Kaetemi) [CLSClient, 2008]
 * \author Vianney Lecroart (Ace) [CLoginClientMtp, 2003]
 * \author Nevrax France [CLoginClient, 2000]
 * CLSClient, based on CLoginClient
 */
class CLSClient
{
	friend class _CLSClientAuthenticateThread;
public:
	struct CShard
	{
		CShard() : ShardId(-1), NbPlayers(0), Online(false) { }
		sint32 ShardId;
		uint8 NbPlayers;
		ucstring Name;
		bool Online;
		std::string Version;
		std::string DynPatchURL;
	};
	typedef std::vector<CShard> TShardList;
protected:	
	// pointers
	NLMISC::CMutex _Mutex;
	_CLSClientAuthenticateThread *_Runnable;
	NLMISC::IThread *_Thread;
	NLNET::CCallbackClient *_LSCallbackClient;
	SBCLIENT_CALLBACK _Callback;
	void *_Context;
	void *_Tag;
	
	// instances
	// ...
public:
	// instances
	TShardList ShardList;
	NLNET::CLoginCookie LoginCookie;
	std::string FrontEnd;
	std::string LastError;
public:
	CLSClient();
	virtual ~CLSClient();

	/// Check the LS connection for updates and callback if needed.
	/// Only call when you are waiting for an update.
	void update();

	/// If you decide to cancel before selecting a shard.
	/// Automatically called on deletion.
	void disconnect();

	/** Try to login with login and password. cpassword must be md5 
	* crypted (that's why it's a string and not an ucstring) application 
	* is the name of the application. the LS will return all shards 
	* that is available for this application (sample, snowballs, ...)
	* If the authentication is ok, the function return an empty 
	* string else it returns the reason of the failure.
	*/
	void authenticateUser(SBCLIENT_CALLBACK cb, void *context, void *tag, const std::string &ls, const ucstring &login, const std::string &cpassword, const std::string &application);

	/// Select a shard. If ok, we have a login cookie and frontend address.
	void selectShard(SBCLIENT_CALLBACK cb, void *context, void *tag, sint32 shardId);
	
	/// Get a shard by id, returns NULL if it doesn't exist.
	CLSClient::CShard *getShard(uint32 shardId);

	/// Encrypt a pasword
	static std::string encryptPassword(const ucstring &password);

private:
	inline void callback();
	inline void _disconnect();

	static NLNET::TCallbackItem _LSCallbackArray[2];

	static void cbAuthenticateUser(NLNET::CMessage &msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);
	static void cbSelectShard(NLNET::CMessage &msgin, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase);

	SBCLIENT_EVIL_SINGLETON_DECL(SBCLIENT::CLSClient);
}; /* class CLSClient */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_LS_CLIENT_H */

/* end of file */
