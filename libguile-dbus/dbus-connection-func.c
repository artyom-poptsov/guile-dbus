#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"
#include "error.h"
#include "symbols.h"
#include "dbus-connection-type.h"
#include "dbus-message-type.h"
#include "dbus-pending-call-type.h"

GDBUS_DEFINE(gdbus_make_dbus_connection, "%make-dbus-connection", 1, (SCM type),
             "Make a DBus connection.")
#define FUNC_NAME s_gdbus_make_dbus_connection
{
    const symbol_mapping_t* c_type = dbus_bus_type_from_scm(type);
    DBusError error;
    dbus_error_init(&error);
    DBusConnection* conn = dbus_bus_get(c_type->value, &error);
    if (dbus_error_is_set(&error)) {
        dbus_error_free(&error);
        gdbus_error(FUNC_NAME, "Connection error",
                    scm_from_locale_string(error.message));
    }
    return dbus_connection_to_scm(conn, c_type->value);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_open_p,
             "dbus-connection-open?", 1,
             (SCM connection),
             "Gets whether the connection is currently open.")
#define FUNC_NAME s_gdbus_connection_open_p
{
    const gdbus_connection_t* conn_data = gdbus_connection_from_scm(connection);
    dbus_bool_t result = dbus_connection_get_is_connected(conn_data->conn);
    return (result == TRUE) ? SCM_BOOL_T : SCM_BOOL_F;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_authenticated_p,
             "dbus-connection-authenticated?", 1,
             (SCM connection),
             "Gets whether the connection was authenticated.")
#define FUNC_NAME s_gdbus_connection_authenticated_p
{
    const gdbus_connection_t* conn_data = gdbus_connection_from_scm(connection);
    dbus_bool_t result = dbus_connection_get_is_authenticated(conn_data->conn);
    return (result == TRUE) ? SCM_BOOL_T : SCM_BOOL_F;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_server_id,
             "dbus-connection-server-id", 1,
             (SCM connection),
             "Gets the ID of the server address we are authenticated to,"
             " if this connection is on the client side.  Otherwise return"
             " #f.")
#define FUNC_NAME s_gdbus_connection_server_id
{
    const gdbus_connection_t* conn_data = gdbus_connection_from_scm(connection);
    char* result = dbus_connection_get_server_id(conn_data->conn);
    return (result == NULL) ? SCM_BOOL_F : scm_from_locale_string(result);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_max_message_size,
             "dbus-connection-max-message-size", 1,
             (SCM connection),
             "Gets the maximum message size.")
#define FUNC_NAME s_gdbus_connection_max_message_size
{
    const gdbus_connection_t* conn_data = gdbus_connection_from_scm(connection);
    int64_t result = dbus_connection_get_max_message_size(conn_data->conn);
    return scm_from_int64(result);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_send, "dbus-connection-send", 2,
             (SCM connection, SCM message),
             "Adds a message to the outgoing message queue."
             " Throws guile-dbus-error on OOM errors.")
#define FUNC_NAME s_gdbus_connection_send
{
    const gdbus_connection_t* conn_data = gdbus_connection_from_scm(connection);
    const gdbus_message_t* msg_data = gdbus_message_from_scm(message);
    dbus_bool_t result = dbus_connection_send(conn_data->conn,
                                              msg_data->message,
                                              NULL);
    if (! result) {
        gdbus_error(FUNC_NAME, "Out of memory",
                    scm_list_2(connection, message));
    }
    return SCM_UNDEFINED;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_send_with_reply,
             "%dbus-connection-send/with-reply", 3,
             (SCM connection, SCM message, SCM timeout),
             "Queues a message to send.")
#define FUNC_NAME s_gdbus_connection_send_with_reply
{
    const gdbus_connection_t* conn_data
        = gdbus_connection_from_scm(connection);
    const gdbus_message_t* msg_data = gdbus_message_from_scm(message);
    int c_timeout = scm_to_int(timeout);
    DBusPendingCall* call;
    dbus_bool_t result = dbus_connection_send_with_reply(conn_data->conn,
                                                         msg_data->message,
                                                         &call,
                                                         c_timeout);
    if (! result) {
        gdbus_error(FUNC_NAME, "Out of memory",
                    scm_list_3(connection, message, timeout));
    }

    return dbus_pending_call_to_scm(call);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_connection_flush,
             "dbus-connection-flush", 1,
             (SCM connection),
             "Blocks until the outgoing message queue is empty")
#define FUNC_NAME s_gdbus_connection_flush
{
    const gdbus_connection_t* conn_data
        = gdbus_connection_from_scm(connection);

    dbus_connection_flush(conn_data->conn);

    return SCM_UNDEFINED;
}
#undef FUNC_NAME


void init_dbus_connection_func()
{
#include "dbus-connection-func.x"
}
