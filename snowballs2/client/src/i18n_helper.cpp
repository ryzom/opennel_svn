/**
 * \file i18n_helper.cpp
 * \brief CI18NHelper
 * \date 2008-01-21 22:56GMT
 * \author Jan Boon (Kaetemi)
 * CI18NHelper
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

#include <nel/misc/types_nl.h>
#include "i18n_helper.h"

// #include <nel/misc/debug.h>
#include <nel/misc/i18n.h>

using namespace std;
using namespace NLMISC;

namespace SBCLIENT {

CI18NHelper::CI18NHelper(const string &prefix, bool keepPrefix)
	: Prefix(prefix), KeepPrefix(keepPrefix)
{
	
}

CI18NHelper::~CI18NHelper()
{
	
}

string CI18NHelper::split(const string &label)
{
	if (label.substr(0, Prefix.size()) == Prefix)
	{
		if (KeepPrefix) return label;
		return label.substr(Prefix.size(), label.size() - Prefix.size());
	}
	return "";
}

ucstring CI18NHelper::get(const string &label, const ucstring &notexist)
{
	std::string lsplit = split(label);
	if (lsplit.empty()) return ucstring::makeFromUtf8(label);
	if (_Overrides.find(lsplit) != _Overrides.end()) return _Overrides[lsplit];
	if (notexist.empty()) return CI18N::get(lsplit);
	if (NLMISC::CI18N::hasTranslation(lsplit)) return CI18N::get(lsplit);
	return notexist;
}

bool CI18NHelper::has(const string &label)
{
	std::string lsplit = split(label);
	if (lsplit.empty()) return true; // no translation needed is ok
	if (_Overrides.find(lsplit) != _Overrides.end()) return true;
	return NLMISC::CI18N::hasTranslation(lsplit);
}

void CI18NHelper::set(const string &label, const ucstring &translation)
{
	std::string lsplit = split(label);
	if (lsplit.empty()) lsplit = label; // allow without prefix
	_Overrides[lsplit] = translation;
}

} /* namespace SBCLIENT */

/* end of file */
