/** \file login_service.h
 * <File description>
 *
 * $Id$
 */

/* Copyright, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX NeL Network Services.
 * NEVRAX NeL Network Services is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX NeL Network Services is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NeL Network Services; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef NL_LOGIN_SERVICE_H
#define NL_LOGIN_SERVICE_H

// we have to include windows.h because mysql.h uses it but not include it
#ifdef NL_OS_WINDOWS
#include <windows.h>
#endif

#include <mysql.h>

#include "nel/misc/types_nl.h"

#include "nel/misc/debug.h"
#include "nel/misc/config_file.h"
#include "nel/misc/displayer.h"
#include "nel/misc/log.h"

#include "nel/net/service.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;

// Constantes



// Structures

struct CFrontEnd
{
	CFrontEnd(uint16 sid, bool patching, const std::string& patchingURI) : SId(sid), Patching(patching), PatchURI(patchingURI)	{}

	uint16		SId;		// Service Id of the frontend on the remote shard
	bool		Patching;	// Is FS in patching mode
	std::string	PatchURI;	// Patch URI
};

struct CShard
{
	CShard (uint32 shardId, uint16 sid) : ShardId(shardId), NbPlayers(0), SId(sid) {}
	
	uint32	ShardId;	// uniq shard id generated by the database, used to identify a WS
	uint32	NbPlayers;	// number of player for *this* LS, the total number of player for a shard is in the database
	uint16	SId;		// uniq service id used to identify the connection when a callback happen

	std::vector<CFrontEnd>	FrontEnds;	// frontends on the shard
};





// Variables

extern CLog*				Output;

extern MYSQL*				DatabaseConnection;

extern std::vector<CShard>	Shards;





/**
 * Encapsulation of MySQL result
 */
class CMySQLResult
{
public:

	/// Constructor
	CMySQLResult(MYSQL_RES* res = NULL);
	/// Constructor
	CMySQLResult(MYSQL* database);
	/// Destructor
	~CMySQLResult();

	/// Cast operator
	operator MYSQL_RES*();

	/// Affectation
	CMySQLResult&	operator = (MYSQL_RES* res);


	/// Test success
	bool			success() const;
	/// Test failure
	bool			failed() const;



	/// Number of rows of result
	uint			numRows();
	/// Fetch row
	MYSQL_ROW		fetchRow();


private:

	MYSQL_RES*	_Result;

};


// Functions

void displayShards ();
void displayUsers ();
sint findUser (uint32 Id);
void beep (uint freq = 400, uint nb = 2, uint beepDuration = 100, uint pauseDuration = 100);

/*
 * disconnectClient is true if we need to disconnect the client on connected on the login system (during the login process)
 * disconnectShard is true if we need to send a message to the shard to disconnect the client from the shard
 */
//void disconnectClient (CUser &user, bool disconnectClient, bool disconnectShard);



#endif // NL_LOGIN_SERVICE_H

/* End of login_service.h */
