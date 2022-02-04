#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "symbols.h"


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

/**
 * See 'dbus-protocol.h'.
 */
static const struct symbol_mapping dbus_message_types[] = {
    { "invalid",       DBUS_MESSAGE_TYPE_INVALID       },
    { "method-call",   DBUS_MESSAGE_TYPE_METHOD_CALL   },
    { "method-return", DBUS_MESSAGE_TYPE_METHOD_RETURN },
    { "error",         DBUS_MESSAGE_TYPE_ERROR         },
    { "signal",        DBUS_MESSAGE_TYPE_SIGNAL        },
    { NULL,            -1                              }
};

static const struct symbol_mapping dbus_bus_types[] = {
    { "session", DBUS_BUS_SESSION },
    { "system",  DBUS_BUS_SYSTEM  },
    { "starter", DBUS_BUS_STARTER },
    { NULL,      -1               }
};


SCM dbus_type_to_scm(int type)
{
    return map_const_to_scm(dbus_types, type);
}

const symbol_mapping_t* dbus_type_from_scm(SCM type)
{
    return map_scm_to_const(dbus_types, type);
}

SCM dbus_value_to_scm(int type, DBusBasicValue value)
{
    switch (type) {

    case DBUS_TYPE_BYTE:
        return scm_from_uchar(value.byt);

    case DBUS_TYPE_INT16:
        return scm_from_short(value.i16);

    case DBUS_TYPE_UINT16:
        return scm_from_uint16(value.u16);

    case DBUS_TYPE_INT32:
        return scm_from_int32(value.i32);

    case DBUS_TYPE_UINT32:
        return scm_from_uint32(value.u32);

    case DBUS_TYPE_INT64:
        return scm_from_int64(value.i64);

    case DBUS_TYPE_UINT64:
        return scm_from_uint64(value.u64);

    case DBUS_TYPE_DOUBLE:
        return scm_from_double(value.dbl);

    case DBUS_TYPE_OBJECT_PATH:
    case DBUS_TYPE_SIGNATURE:
    case DBUS_TYPE_STRING:
        return scm_from_locale_string(value.str);

    case DBUS_TYPE_BOOLEAN:
         switch (value.bool_val) {
         case TRUE:  return SCM_BOOL_T;
         case FALSE: return SCM_BOOL_F;
         default:    return SCM_BOOL_F; /* possibly throw errer here instead */
         }

    case DBUS_TYPE_UNIX_FD:
         return scm_from_int(value.fd);

    default:
        return SCM_BOOL_F;      /* The type is unsupported yet. */
    }
}


SCM dbus_message_type_to_scm(int type)
{
    return map_const_to_scm(dbus_message_types, type);
}

const symbol_mapping_t* dbus_message_type_from_scm(SCM type)
{
    return map_scm_to_const(dbus_message_types, type);
}



SCM dbus_bus_type_to_scm(DBusBusType type)
{
    return map_const_to_scm(dbus_bus_types, type);
}

const symbol_mapping_t* dbus_bus_type_from_scm(SCM type)
{
    return map_scm_to_const(dbus_bus_types, type);
}
