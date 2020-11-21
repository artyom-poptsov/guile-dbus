#include <libguile.h>
#include <string.h>

#include "common.h"

/* Convert the a VALUE to a Scheme symbol */
SCM map_const_to_scm(const symbol_mapping_t* types, int value)
{
    const struct symbol_mapping *t;
    for (t = types; t->symbol; ++t) {
        if (t->value == value)
            return scm_from_locale_symbol(t->symbol);
    }
    return SCM_BOOL_F;
}

/* Convert the Scheme symbol VALUE to a C constant.

   Return the apropriate structure that contains the needed
   constant. */
const symbol_mapping_t* map_scm_to_const(const symbol_mapping_t* types,
                                         SCM value)
{
    const symbol_mapping_t* t;
    char *sym = scm_to_locale_string (scm_symbol_to_string (value));
    for (t = types; t->symbol; ++t)
    {
        if (! strcmp(t->symbol, sym))
            return t;
    }
    return NULL;
}


/* Return an address of the object OBJ as an hexadecimal number represented as
   a string. */
SCM scm_object_hex_address(SCM obj)
{
    return scm_number_to_string(scm_object_address(obj), scm_from_uint(16U));
}

SCM compare_objects(SCM x1, SCM x2, void* (*converter)(SCM x))
{
    void* d1 = converter(x1);
    void* d2 = converter(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

void set_smob_callbacks(scm_t_bits tag,
                        SCM (*mark_cb)(SCM obj),
                        size_t (*free_cb)(SCM obj),
                        SCM (*equalp_cb)(SCM x1, SCM x2),
                        int (*print_cb)(SCM obj, SCM port, scm_print_state* ps))
{
    scm_set_smob_mark(tag, mark_cb);
    scm_set_smob_free(tag, free_cb);
    scm_set_smob_print(tag, print_cb);
    scm_set_smob_equalp(tag, equalp_cb);
}
