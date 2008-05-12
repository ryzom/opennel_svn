#ifndef PROPDEFS_H_
#define PROPDEFS_H_

/**
 * Property definitions
 * 
 * All properties used in the mode setting/retrieval code
 * are stored in this header file. Aside of the define with
 * the associated name constant, the documentation to each
 * of them contains the datatype it's supposed to have (and
 * used as in the mode setting/retrieval code).
 */


// ModeManagers

/**
 * bool
 */
#define P_USE_XINERAMA		"use_xinerama"

/**
 * bool
 */
#define P_USE_GLX_TEST		"use_glx_test"

/**
 * bool
 */
#define P_USE_MODE_FILTER	"mode_filter"

/**
 * bool
 */
#define P_USE_RANDR12		"use_randr12"

/**
 * char**
 */
#define P_USER_MODES		"user_modes"

#endif /*PROPDEFS_H_*/
