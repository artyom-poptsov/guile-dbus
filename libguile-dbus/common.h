#ifndef __COMMON_H__
#define __COMMON_H__

#include <libguile.h>

/* This macro is a little bit shorter than the original SCM_DEFINE macro. */
#define GDBUS_DEFINE(c_name, scheme_name, req, arglist, docstring)      \
    SCM_DEFINE(c_name, scheme_name, req, 0, 0, arglist, docstring)


/* Structures.  */

struct symbol_mapping {
    char* symbol;
    int   value;
};


/* Procedures. */

SCM map_const_to_scm (
    const struct symbol_mapping *types,
    int value);

const struct symbol_mapping* map_scm_to_const (
    const struct symbol_mapping *types,
    SCM value);

SCM scm_object_hex_address (SCM obj);

#endif  /* ifndef __COMMON_H__ */
