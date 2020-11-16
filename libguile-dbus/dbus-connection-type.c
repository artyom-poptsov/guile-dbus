#include <libguile.h>
#include <dbus/dbus.h>
#include <stdio.h>

#include "common.h"
#include "error.h"
#include "dbus-connection-type.h"

static const char* GDBUS_CONNECTION_TYPE_NAME = "dbus-connection";

scm_t_bits dbus_connection_tag;

static const struct symbol_mapping bus_types[] = {
    { "session", DBUS_BUS_SESSION },
    { "system",  DBUS_BUS_SYSTEM  },
    { "starter", DBUS_BUS_STARTER },
    { NULL,      -1               }
};


static SCM mark_dbus_connection(SCM dbus_connection)
{
    return SCM_BOOL_F;
}

static size_t free_dbus_connection(SCM dbus_connection)
{
    struct dbus_connection_data* gdbus_conn
        = _scm_to_dbus_connection_data(dbus_connection);
    dbus_connection_close(gdbus_conn->conn);
    return 0;
}

static SCM equalp_dbus_connection(SCM x1, SCM x2)
{
    struct dbus_connection_data* d1 = _scm_to_dbus_connection_data(x1);
    struct dbus_connection_data* d2 = _scm_to_dbus_connection_data(x2);
    if ((! d1) || (! d2)) {
        return SCM_BOOL_F;
    } else if (d1 != d2) {
        return SCM_BOOL_F;
    } else {
        return SCM_BOOL_T;
    }
}

static int print_dbus_connection(SCM obj, SCM port, scm_print_state* pstate)
{
    (void) pstate;
    struct dbus_connection_data* gdbus_conn = _scm_to_dbus_connection_data(obj);
    scm_puts("#<dbus-connection ", port);
    scm_display(dbus_bus_type_to_scm(gdbus_conn->type), port);
    scm_puts(">", port);
    return 1;
}


SCM dbus_bus_type_to_scm(DBusBusType type)
{
    return map_const_to_scm(bus_types, type);
}

struct dbus_connection_data* _allocate_dbus_connection_data()
{
    return scm_gc_malloc(sizeof(struct dbus_connection_data),
                         GDBUS_CONNECTION_TYPE_NAME);
}

SCM _scm_from_dbus_connection(DBusConnection* conn, DBusBusType type)
{
    SCM smob;
    struct dbus_connection_data* gdbus_conn = _allocate_dbus_connection_data();

    gdbus_conn->type = type;
    gdbus_conn->conn = conn;

    SCM_NEWSMOB(smob, dbus_connection_tag, gdbus_conn);

    return smob;
}

struct dbus_connection_data* _scm_to_dbus_connection_data(SCM x)
{
    scm_assert_smob_type(dbus_connection_tag, x);
    return (struct dbus_connection_data *) SCM_SMOB_DATA(x);
}


GDBUS_DEFINE(gdbus_make_dbus_connection, "%make-dbus-connection", 1, (SCM type),
             "Make a DBus connection.")
#define FUNC_NAME s_gdbus_make_dbus_connection
{
    const struct symbol_mapping* c_type = map_scm_to_const(bus_types, type);
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


void init_dbus_connection_type()
{
    dbus_connection_tag
        = scm_make_smob_type(GDBUS_CONNECTION_TYPE_NAME,
                             sizeof(struct dbus_connection_data));
    scm_set_smob_mark(dbus_connection_tag, mark_dbus_connection);
    scm_set_smob_free(dbus_connection_tag, free_dbus_connection);
    scm_set_smob_print(dbus_connection_tag, print_dbus_connection);
    scm_set_smob_equalp(dbus_connection_tag, equalp_dbus_connection);

#include "dbus-connection-type.x"
}
