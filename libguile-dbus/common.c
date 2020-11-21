#include <libguile.h>

#include "common.h"

/* Convert the a VALUE to a Scheme symbol */
SCM map_const_to_scm(const struct symbol_mapping *types, int value)
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
const struct symbol_mapping* map_scm_to_const(const struct symbol_mapping *types,
                                              SCM value)
{
    const struct symbol_mapping *t;
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
