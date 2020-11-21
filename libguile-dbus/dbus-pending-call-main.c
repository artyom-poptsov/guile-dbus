#include "dbus-pending-call-type.h"
#include "dbus-pending-call-func.h"

void init_dbus_pending_call()
{
    init_dbus_pending_call_type();
    init_dbus_pending_call_func();
}
