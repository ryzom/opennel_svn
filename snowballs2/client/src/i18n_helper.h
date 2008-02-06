/**
 * \file i18n_helper.h
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

#ifndef SBCLIENT_I18N_HELPER_H
#define SBCLIENT_I18N_HELPER_H
#include <nel/misc/types_nl.h>

#include "evil_singleton_decl.h"

#include <nel/misc/ucstring.h>

namespace SBCLIENT {

/**
 * \brief CI18NHelper
 * \date 2008-01-21 22:56GMT
 * \author Jan Boon (Kaetemi)
 * Allows overriding language, and using prefixes to choose between uxt and direct string.
 */
class CI18NHelper
{
public:
	// instances
	/** The characters in front of the label */
	std::string Prefix;
	/** If the prefix is also in the uxt labels */
	bool KeepPrefix;
protected:
	// instances
	std::map<std::string, ucstring> _Overrides;
public:
	CI18NHelper(const std::string &prefix, bool keepPrefix);
	virtual ~CI18NHelper();

	/** Returns non-empty label if the label is prefixed */
	std::string split(const std::string &label);
	/** Returns the translation if it exists, notexist can be empty 
	if label doesn't have prefix, label is converted from utf-8 to uc
	and returned as the translation */
	ucstring get(const std::string &label, const ucstring &notexist = std::string());
	/** Returns if the translation exists */
	bool has(const std::string &label);
	/** Sets or adds the local translation override, label must have prefix */
	void set(const std::string &label, const ucstring &translation);

}; /* class CI18NHelper */

} /* namespace SBCLIENT */

#endif /* #ifndef SBCLIENT_I18N_HELPER_H */

/* end of file */
