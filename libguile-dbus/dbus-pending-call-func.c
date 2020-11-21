#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "dbus-pending-call-type.h"

GDBUS_DEFINE(gdbus_pending_call_block, "dbus-pending-call-block", 1,
             (SCM call),
             "Block until the pending call is completed. ")
#define FUNC_NAME s_gdbus_pending_call_block
{
    gdbus_pending_call_t* gdbus_call = gdbus_pending_call_from_scm(call);
    dbus_pending_call_block(gdbus_call->call);
    return SCM_UNDEFINED;
}
#undef FUNC_NAME


void init_dbus_pending_call_func()
{
#include "dbus-pending-call-func.x"
}
