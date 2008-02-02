/** \file snowballs_config.h
 * Snowballs build configuration. Used for debug features.
 * This file is only supposed to be used in snowballs_client.h/cpp.
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
 * 
 * $Id$
 */

#include <nel/misc/types_nl.h>



// use the default log.log file (not erased on use)
// #define SBCLIENT_USE_LOG_LOG false



// the config file name
#define SBCLIENT_CONFIG_FILE "snowballs_client.cfg"



// enable compiling sound related things
#define SBCLIENT_WITH_SOUND 1



// use snowballs log file
// #define SBCLIENT_USE_LOG 1

// snowballs log file name
#define SBCLIENT_LOG_FILE "snowballs_client.log"

// clear snowballs log before use
#define SBCLIENT_ERASE_LOG true



// name for client used in config
#define SBCLIENT_NAME "Client"



// some default defines
#if FINAL_VERSION
#	if !defined(SBCLIENT_USE_LOG_LOG)
#		define SBCLIENT_USE_LOG_LOG false
#	endif
#	if !defined(SBCLIENT_USE_LOG)
#		define SBCLIENT_USE_LOG 0
#	endif
#endif

#if !defined(SBCLIENT_USE_LOG_LOG)
#	define SBCLIENT_USE_LOG_LOG true
#endif
#if !defined (SBCLIENT_USE_LOG)
#	define SBCLIENT_USE_LOG 1
#endif
