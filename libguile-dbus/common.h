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

typedef struct symbol_mapping symbol_mapping_t;


/* Procedures. */

SCM map_const_to_scm(const symbol_mapping_t* types, int value);
const symbol_mapping_t* map_scm_to_const(const symbol_mapping_t* types,
                                         SCM value);

SCM scm_object_hex_address (SCM obj);
SCM compare_objects(SCM x1, SCM x2, void* (*converter)(SCM x));

void set_smob_callbacks(scm_t_bits tag,
                        SCM (*mark_cb)(SCM obj),
                        size_t (*free_cb)(SCM obj),
                        SCM (*equalp_cb)(SCM x1, SCM x2),
                        int (*print_cb)(SCM obj, SCM port, scm_print_state* ps));

#endif  /* ifndef __COMMON_H__ */
