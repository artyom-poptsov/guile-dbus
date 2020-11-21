#include "dbus-connection-type.h"
#include "dbus-connection-func.h"

void init_dbus_connection()
{
    init_dbus_connection_type();
    init_dbus_connection_func();
}
