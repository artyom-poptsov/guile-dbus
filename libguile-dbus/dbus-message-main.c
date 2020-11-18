#include "dbus-message-type.h"
#include "dbus-message-func.h"

void init_dbus_message()
{
    init_dbus_message_type();
    init_dbus_message_func();
}
