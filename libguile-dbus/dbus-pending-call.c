#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "dbus-pending-call.h"


static const char* GDBUS_PENDING_CALL_TYPE_NAME = "dbus-pending-call";

scm_t_bits dbus_pending_call_tag;

static SCM mark_dbus_pending_call(SCM obj)
{
    return SCM_BOOL_F;
}

static size_t free_dbus_pending_call(SCM obj)
{
    struct dbus_pending_call_data* data = _scm_to_dbus_pending_call_data(obj);
    dbus_pending_call_unref(data->call);
    return 0;
}

static int print_dbus_pending_call(SCM obj, SCM port, scm_print_state* pstate)
{
    (void) pstate;
    scm_puts("#<", port);
    scm_puts(GDBUS_PENDING_CALL_TYPE_NAME, port);
    scm_puts(" ", port);
    scm_display(scm_object_hex_address(obj), port);
    scm_puts(">", port);
    return 1;
}

static SCM equalp_dbus_pending_call(SCM x1, SCM x2)
{
    return compare_objects(x1, x2,
                           (void* (*)(SCM x)) _scm_to_dbus_pending_call_data);
}


struct dbus_pending_call_data* make_dbus_pending_call_data()
{
    return scm_gc_malloc(sizeof(struct dbus_pending_call_data),
                         GDBUS_PENDING_CALL_TYPE_NAME);
}


SCM _scm_from_dbus_pending_call(DBusPendingCall* call)
{
    SCM smob;
    struct dbus_pending_call_data* gdbus_call = make_dbus_pending_call_data();
    gdbus_call->call = call;
    SCM_NEWSMOB(smob, dbus_pending_call_tag, gdbus_call);
    return smob;
}

struct dbus_pending_call_data* _scm_to_dbus_pending_call_data(SCM x)
{
    scm_assert_smob_type(dbus_pending_call_tag, x);
    return (struct dbus_pending_call_data *) SCM_SMOB_DATA(x);
}


void init_dbus_pending_call_type()
{
    dbus_pending_call_tag
        = scm_make_smob_type(GDBUS_PENDING_CALL_TYPE_NAME,
                             sizeof(struct dbus_pending_call_data));
    scm_set_smob_mark(dbus_pending_call_tag,   mark_dbus_pending_call);
    scm_set_smob_free(dbus_pending_call_tag,   free_dbus_pending_call);
    scm_set_smob_print(dbus_pending_call_tag,  print_dbus_pending_call);
    scm_set_smob_equalp(dbus_pending_call_tag, equalp_dbus_pending_call);

#include "dbus-pending-call.x"
}

