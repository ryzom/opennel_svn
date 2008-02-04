/**
 * \file evil_singleton_decl.h
 * \brief SBCLIENT_EVIL_SINGLETON_DECL
 * \date 2008-02-02 13:25GMT
 * \author Jan Boon (Kaetemi)
 * SBCLIENT_EVIL_SINGLETON_DECL
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

#ifndef SBCLIENT_EVIL_SINGLETON_DECL_H
#define SBCLIENT_EVIL_SINGLETON_DECL_H
#include <nel/misc/types_nl.h>

/** Singleton that must be manually initialized, 
 * because you shouldn't be lazy. */
#define SBCLIENT_EVIL_SINGLETON_DECL(className) \
	private: \
		static className *_Instance; \
	public: \
		static className *getInstancePtr(); \
		static className &getInstance(); \
	private:

#endif /* #ifndef SBCLIENT_EVIL_SINGLETON_DECL_H */

/* end of file */