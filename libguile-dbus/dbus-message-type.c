#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "dbus-message-type.h"

static const char* GDBUS_MESSAGE_TYPE_NAME = "dbus-message";

scm_t_bits gdbus_message_tag;

static SCM _mark(SCM dbus_message)
{
    return SCM_BOOL_F;
}

static size_t _free(SCM message)
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    dbus_message_unref(data->message);
    return 0;
}

static SCM _equalp(SCM x1, SCM x2)
{
    return compare_objects(x1, x2, (void* (*)(SCM x)) gdbus_message_from_scm);
}

static int _print(SCM obj, SCM port, scm_print_state* pstate)
{
    (void) pstate;
    /* struct dbus_message_data* data = _scm_to_dbus_message_data(obj); */
    scm_puts("#<dbus-message ", port);
    scm_display(scm_object_hex_address(obj), port);
    scm_puts(">", port);
    return 1;
}


gdbus_message_t* make_gdbus_message()
{
    return scm_gc_malloc(sizeof(gdbus_message_t), GDBUS_MESSAGE_TYPE_NAME);
}


SCM dbus_message_to_scm(DBusMessage* message)
{
    SCM smob;
    gdbus_message_t* gdbus_message = make_gdbus_message();
    gdbus_message->message = message;
    SCM_NEWSMOB(smob, gdbus_message_tag, gdbus_message);
    return smob;
}

gdbus_message_t* gdbus_message_from_scm(SCM x)
{
    scm_assert_smob_type(dbus_message_tag, x);
    return (gdbus_message_t *) SCM_SMOB_DATA(x);
}


void init_dbus_message_type()
{
    dbus_message_tag = scm_make_smob_type(GDBUS_MESSAGE_TYPE_NAME,
                                          sizeof(gdbus_message_t));
    set_smob_callbacks(gdbus_message_tag, _mark, _free, _equalp, _print);

#include "dbus-message-type.x"
}
