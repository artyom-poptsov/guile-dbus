#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "dbus-message-type.h"

static const char* GDBUS_MESSAGE_TYPE_NAME = "dbus-message";

scm_t_bits dbus_message_tag;

/**
 * See 'dbus-protocol.h'.
 */
static const struct symbol_mapping message_types[] = {
    { "invalid",       DBUS_MESSAGE_TYPE_INVALID       },
    { "method-call",   DBUS_MESSAGE_TYPE_METHOD_CALL   },
    { "method-return", DBUS_MESSAGE_TYPE_METHOD_RETURN },
    { "error",         DBUS_MESSAGE_TYPE_ERROR         },
    { "signal",        DBUS_MESSAGE_TYPE_SIGNAL        },
    { NULL,            -1                              }
};

static SCM mark_dbus_message(SCM dbus_message)
{
    return SCM_BOOL_F;
}

static size_t free_dbus_message(SCM message)
{
    struct dbus_message_data* data = _scm_to_dbus_message_data(message);
    dbus_message_unref(data->message);
    return 0;
}

static SCM equalp_dbus_message(SCM x1, SCM x2)
{
    struct dbus_message_data* d1 = _scm_to_dbus_message_data(x1);
    struct dbus_message_data* d2 = _scm_to_dbus_message_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

static int print_dbus_message(SCM obj, SCM port, scm_print_state* pstate)
{
    (void) pstate;
    /* struct dbus_message_data* data = _scm_to_dbus_message_data(obj); */
    scm_puts("#<dbus-message>", port);
    return 1;
}


struct dbus_message_data* _allocate_dbus_message_data()
{
    return scm_gc_malloc(sizeof(struct dbus_message_data),
                         GDBUS_MESSAGE_TYPE_NAME);
}


SCM _scm_from_dbus_message(DBusMessage* message)
{
    SCM smob;
    struct dbus_message_data* gdbus_message = _allocate_dbus_message_data();
    gdbus_message->message = message;
    SCM_NEWSMOB(smob, dbus_message_tag, gdbus_message);
    return smob;
}

struct dbus_message_data* _scm_to_dbus_message_data(SCM x)
{
    scm_assert_smob_type(dbus_message_tag, x);
    return (struct dbus_message_data *) SCM_SMOB_DATA(x);
}



GDBUS_DEFINE(gdbus_make_dbus_message, "%make-dbus-message", 1, (SCM type),
           "Make a DBus message")
#define FUNC_NAME s_gdbus_make_dbus_message
{
    const struct symbol_mapping* c_type = map_scm_to_const(message_types, type);
    DBusMessage* message = dbus_message_new(c_type->value);
    return _scm_from_dbus_message(message);
}
#undef FUNC_NAME


void init_dbus_message_type()
{
    dbus_message_tag = scm_make_smob_type(GDBUS_MESSAGE_TYPE_NAME,
                                          sizeof(struct dbus_message_data));
    scm_set_smob_mark(dbus_message_tag, mark_dbus_message);
    scm_set_smob_free(dbus_message_tag, free_dbus_message);
    scm_set_smob_print(dbus_message_tag, print_dbus_message);
    scm_set_smob_equalp(dbus_message_tag, equalp_dbus_message);

#include "dbus-message-type.x"
}
