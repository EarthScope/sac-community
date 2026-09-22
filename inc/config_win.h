/* This is here for Visual Studio Express on Windows */
#define WIN32 1
#define HAVE_FUNC_FMIN 1
#define HAVE_FUNC_FMAX 1
#define HAVE_FUNC_MIN 1
#define HAVE_FUNC_MAX 1
#define HAVE_FUNC_LROUND 1
#define HAVE_FUNC_PUTENV 1

#define MISSING_FUNC_VSNPRINTF   1
#define MISSING_FUNC_VASPRINTF   1
#define MISSING_FUNC_ASPRINTF    1
#define MISSING_FUNC_SNPRINTF    1
#define MISSING_FUNC_STRCASECMP  1
#define MISSING_FUNC_STRNCASECMP 1
#define MISSING_FUNC_INDEX       1
#define MISSING_FUNC_RINDEX      1
#define MISSING_FUNC_MKSTEMPS    1
#define MISSING_FUNC_STRLCPY     1
#define MISSING_FUNC_STRLCAT     1
#define MISSING_FUNC_SLEEP       1
#define MISSING_FUNC_DIRNAME     1
#define MISSING_FUNC_BASENAME    1

#define COMPLEX_STRUCT_DEFINED 1
#define F_OK 0

#define SACAUX "C:\\Program Files\\Sac\\winaux"

#define PACKAGE_BUGREPORT "https://github.com/earthscope/sac"
#define BUILD_DATE        "09/21/2026"
#define PACKAGE_VERSION   "103.0"

/* GUI_APP - If defined (to any value, 0, 1, ...)
      Create a new window command windown on execution

   GUI_APP - If not defined (commented out), run sac only from the command line
      This is only really useful for testing the numerical capabilities of sac
      Plotting windows are not available if GUI_APP is commented out
 */
#define GUI_APP 1
