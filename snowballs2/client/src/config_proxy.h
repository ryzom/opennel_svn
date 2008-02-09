/**
 * \file config_proxy.h
 * \brief CConfigProxy
 * \date 2008-02-02 13:01GMT
 * \author Jan Boon (Kaetemi)
 * CConfigProxy
 * 
 * $Id$
 */

/* 
 * Copyright (C) 2008  Jan Boon (Kaetemi)
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

#ifndef SBCLIENT_CONFIG_PROXY_H
#define SBCLIENT_CONFIG_PROXY_H
#include <nel/misc/types_nl.h>

#include "member_callback_type.h"

#include <nel/misc/config_file.h>
#include <nel/misc/rgba.h>
#include <nel/misc/ucstring.h>

#include <map>

namespace SBCLIENT {

/**
 * \brief CConfigProxy
 * \date 2008-02-02 13:01GMT
 * \author Jan Boon (Kaetemi)
 * CConfigProxy
 */
class CConfigProxy
{
protected:
	// pointers
	NLMISC::CConfigFile *_ConfigFile;
	
	// instances
	std::string _Id;
	std::string _IdU;
public:
	CConfigProxy(const std::string &id);
	virtual ~CConfigProxy();

	NLMISC::CConfigFile::CVar &getVar(const std::string &varName);
	NLMISC::CConfigFile::CVar *getVarPtr(const std::string &varName);
	bool exists(const std::string &varName);

	float getValue(const std::string &varName, float defaultValue);
	double getValue(const std::string &varName, double defaultValue);
	int getValue(const std::string &varName, int defaultValue);
	std::string getValue(const std::string &varName, const std::string &defaultValue);
	ucstring getValue(const std::string &varName, const ucstring &defaultValue);
	bool getValue(const std::string &varName, bool defaultValue);
	NLMISC::CRGBA getValue(const std::string &varName, NLMISC::CRGBA &defaultValue);

	void setCallback(const std::string &varName, TConfigCallback cb, void *context, void *tag);
	void setCallbackAndCall(const std::string &varName, TConfigCallback cb, void *context, void *tag);
	void dropCallback(const std::string &varName);
}; /* class CConfigProxy */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_CONFIG_PROXY_H */

/* end of file */
