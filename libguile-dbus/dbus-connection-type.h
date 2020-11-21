#ifndef __DBUS_CONNECTION_TYPE__
#define __DBUS_CONNECTION_TYPE__

#include <libguile.h>
#include <dbus/dbus.h>

/**
 * Guile-DBus connection SMOB data.
 */
struct gdbus_connection {
    /**
     * Pointer to a DBus connection object.
     */
    DBusConnection* conn;

    /**
     * Well-known bus DBus connection type.  See 'dbus-shared.h'.
     */
    DBusBusType type;
};

typedef struct gdbus_connection gdbus_connection_t;


void init_dbus_connection_type();


gdbus_connection_t* _scm_to_dbus_connection_data(SCM x);
SCM _scm_from_dbus_connection(DBusConnection* conn, DBusBusType type);
SCM dbus_bus_type_to_scm(DBusBusType type);

#endif  /* ifndef __DBUS_CONNECTION_TYPE__ */
