#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "dbus-pending-call-type.h"
#include "dbus-message-type.h"

GDBUS_DEFINE(gdbus_pending_call_block, "dbus-pending-call-block", 1,
             (SCM call),
             "Block until the pending call is completed. ")
#define FUNC_NAME s_gdbus_pending_call_block
{
    dbus_pending_call_block(dbus_pending_call_from_scm(call));
    return SCM_UNDEFINED;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_pending_call_cancel, "dbus-pending-call-cancel", 1,
             (SCM call),
             "\
Cancels the pending call, such that any reply \
or error received will just be ignored.")
#define FUNC_NAME s_gdbus_pending_call_cancel
{
    dbus_pending_call_cancel(dbus_pending_call_from_scm(call));
    return SCM_UNDEFINED;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_pending_call_steal_reply, "dbus-pending-call-steal-reply", 1,
             (SCM call),
    "")
#define FUNC_NAME s_gdbus_pending_call_steal_reply
{
    DBusPendingCall* c_call = dbus_pending_call_from_scm(call);
    DBusMessage* msg = dbus_pending_call_steal_reply(c_call);
    return msg ?  dbus_message_to_scm(msg) : SCM_BOOL_F;
}
#undef FUNC_NAME


void init_dbus_pending_call_func()
{
#include "dbus-pending-call-func.x"
}
