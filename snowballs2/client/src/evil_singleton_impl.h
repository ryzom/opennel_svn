/**
 * \file evil_singleton_impl.h
 * \brief SBCLIENT_EVIL_SINGLETON_IMPL
 * \date 2008-02-02 13:25GMT
 * \author Jan Boon (Kaetemi)
 * SBCLIENT_EVIL_SINGLETON_IMPL
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

#ifndef SBCLIENT_EVIL_SINGLETON_IMPL_H
#define SBCLIENT_EVIL_SINGLETON_IMPL_H
#include <nel/misc/types_nl.h>
#include <nel/misc/debug.h>
#include <nel/misc/app_context.h>

/** Put this in the cpp. Include the namespace in className. */
#define SBCLIENT_EVIL_SINGLETON_IMPL(className) \
	className *className::_Instance = NULL; \
	className *className::getInstancePtr() \
	{ \
		if (!_Instance) \
		{ \
			nlassertex(NLMISC::INelContext::isContextInitialised(), ("You are trying to access a safe singleton without having initialized a NeL context. The simplest correction is to add 'NLMISC::CApplicationContext myApplicationContext;' at the very begining of your application.")); \
			void *ptr = NLMISC::INelContext::getInstance().getSingletonPointer(#className); \
			_Instance = reinterpret_cast<className*>(ptr); \
		} \
		return _Instance; \
	} \
	className &className::getInstance() \
	{ \
		getInstancePtr(); \
		nlassert(_Instance); \
		return *_Instance; \
	}

/** Put this inside the constructor. */
#define SBCLIENT_EVIL_SINGLETON_CONSTRUCTOR(className) \
	nlassert(!className::_Instance); \
	className::_Instance = this;

/** Put this inside the destructor. */
#define SBCLIENT_EVIL_SINGLETON_DESTRUCTOR(className) \
	nlassert(className::_Instance == this); \
	className::_Instance = NULL;

#endif /* #ifndef SBCLIENT_EVIL_SINGLETON_IMPL_H */

/* end of file */
