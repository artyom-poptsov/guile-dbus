#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "error.h"
#include "symbols.h"
#include "dbus-connection-type.h"

GDBUS_DEFINE(gdbus_make_dbus_connection, "%make-dbus-connection", 1, (SCM type),
             "Make a DBus connection.")
#define FUNC_NAME s_gdbus_make_dbus_connection
{
    const struct symbol_mapping* c_type = dbus_bus_type_from_scm(type);
    DBusError error;
    dbus_error_init(&error);
    DBusConnection* conn = dbus_bus_get(c_type->value, &error);
    if (dbus_error_is_set(&error)) {
        dbus_error_free(&error);
        gdbus_error(FUNC_NAME, "Connection error",
                    scm_from_locale_string(error.message));
    }
    return _scm_from_dbus_connection(conn, c_type->value);
}
#undef FUNC_NAME


void init_dbus_connection_func()
{
#include "dbus-connection-func.x"
}
