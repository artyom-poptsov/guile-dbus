#include <libguile.h>
#include <dbus/dbus.h>
#include <stdio.h>

#include "common.h"
#include "error.h"
#include "dbus-connection-type.h"

static const char* GDBUS_CONNECTION_TYPE_NAME = "dbus-connection";

scm_t_bits dbus_connection_tag;


static SCM _mark(SCM dbus_connection)
{
    return SCM_BOOL_F;
}

static size_t _free(SCM dbus_connection)
{
    struct dbus_connection_data* gdbus_conn
        = _scm_to_dbus_connection_data(dbus_connection);
    dbus_connection_close(gdbus_conn->conn);
    return 0;
}

static SCM _equalp(SCM x1, SCM x2)
{
    return compare_objects(x1, x2,
                           (void* (*)(SCM x)) _scm_to_dbus_connection_data);
}

static int _print(SCM obj, SCM port, scm_print_state* pstate)
{
    (void) pstate;
    struct dbus_connection_data* gdbus_conn = _scm_to_dbus_connection_data(obj);
    scm_puts("#<dbus-connection ", port);
    scm_display(dbus_bus_type_to_scm(gdbus_conn->type), port);
    scm_puts(">", port);
    return 1;
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


void init_dbus_connection_type()
{
    dbus_connection_tag
        = scm_make_smob_type(GDBUS_CONNECTION_TYPE_NAME,
                             sizeof(struct dbus_connection_data));
    set_smob_callbacks(dbus_connection_tag, _mark, _free, _equalp, _print);

#include "dbus-connection-type.x"
}
