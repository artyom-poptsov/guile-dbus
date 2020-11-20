#include <dbus/dbus.h>

#include "common.h"
#include "dbus-type.h"


/**
 * See 'dbus-protocol.h'.
 */
static const struct symbol_mapping dbus_types[] = {
    /* Type code that is never equal to a legitimate type code. */
    { "invalid",     DBUS_TYPE_INVALID     },
    /* Primitive data types. */
    { "byte",        DBUS_TYPE_BYTE        },
    { "boolean",     DBUS_TYPE_BOOLEAN     },
    { "int16",       DBUS_TYPE_INT16       },
    { "int32",       DBUS_TYPE_INT32       },
    { "uint16",      DBUS_TYPE_UINT16      },
    { "uint32",      DBUS_TYPE_UINT32      },
    { "int64",       DBUS_TYPE_INT64       },
    { "uint64",      DBUS_TYPE_UINT64      },
    { "double",      DBUS_TYPE_DOUBLE      },
    { "string",      DBUS_TYPE_STRING      },
    { "object-path", DBUS_TYPE_OBJECT_PATH },
    { "signature",   DBUS_TYPE_SIGNATURE   },
    { "unix-fd",     DBUS_TYPE_UNIX_FD     },
    /* Compound data types. */
    { "array",       DBUS_TYPE_ARRAY       },
    { "variant",     DBUS_TYPE_VARIANT     },
    { "struct",      DBUS_TYPE_STRUCT      },
    { "dict-entry",  DBUS_TYPE_DICT_ENTRY  },
    { NULL,          -1                    }
};


SCM dbus_type_to_scm(int type)
{
    return map_const_to_scm(dbus_types, type);
}

const struct symbol_mapping* dbus_type_from_scm(SCM type)
{
    return map_scm_to_const(dbus_types, type);
}
