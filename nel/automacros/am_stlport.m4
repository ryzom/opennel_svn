# =========================================================================
# AM_PATH_STLPORT
#
#    Option:      none.
#    Description: check the instalation of the STLPort library and set the
#                 CXXFLAGS and LIBS variables to use it.

AC_DEFUN([AM_PATH_STLPORT], [
    stlport_places=$1
 
    have_stlport=0
 
    # Save the original values of the flags we tweak.
    stlport_check_lib_save_libs="$LIBS"
    stlport_check_lib_save_cxxflags="$CXXFLAGS"
 
    found=not
    for stlport_place in $stlport_places;
    do
      LIBS="$stlport_check_lib_save_libs"
      CXXFLAGS="$stlport_check_lib_save_cxxflags"
     
      LIBS="$LIBS -L$stlport_place/lib"
      CXXFLAGS="$CXXFLAGS -I$stlport_place/include/stlport"
      description="$stlport_place"
     
      AC_MSG_NOTICE([checking for STLport in $description])
      # Clear the header cache variable for each location
      changequote(,)
      cache_id="`echo ac_cv_header_stl__stlport_version_h \
               | sed -e 's/[^a-zA-Z0-9_]/_/g'`"
      changequote([,])
      unset $cache_id
      AC_CHECK_HEADER([stl/_stlport_version.h], [
        # We generate a separate cache variable for each prefix and libname
        # we search under.  That way, we avoid caching information that
        # changes if the user runs `configure' with a different set of
        # switches.
        changequote(,)
        cache_id="`echo nel_cv_check_stlport_stlport_version_h_in_${stlport_place} \
                 | sed -e 's/[^a-zA-Z0-9_]/_/g'`"
        changequote([,])

        AC_CACHE_VAL($cache_id,
          [
            eval "$cache_id=yes"
          ])
        result="`eval echo '$'$cache_id`"
        AC_MSG_RESULT($result)
      ], [result="no"])

      # If we found it, no need to search any more.
      if test "$result" = "yes"; then
        found="$stlport_place"
        break
      fi
      test "$found" != "not" && break
    done

    # Restore the original values of the flags we tweak.
    LIBS="$stlport_check_lib_save_libs"
    CXXFLAGS="$stlport_check_lib_save_cxxflags"

    case "$found" in
    "not")
      have_stlport=0
      AC_MSG_ERROR([STLport must be installed ( http://www.stlport.org).])
      ;;
    *)
      STLPORT_INCLUDES="-I$found/include/stlport"
      STLPORT_LIBS="-L$found/lib"
      have_stlport=1
      ;;
    esac
])

# =========================================================================
# End of file

