#ifndef __DBUS_PENDING_CALL_H__
#define __DBUS_PENDING_CALL_H__

#include <libguile.h>
#include <dbus/dbus.h>

extern scm_t_bits dbus_pending_call_tag;

/**
 * Guile-DBus message SMOB data.
 */
struct dbus_pending_call_data {
    DBusPendingCall* call;
};

SCM _scm_from_dbus_pending_call(DBusPendingCall* call);
struct dbus_pending_call_data* _scm_to_dbus_pending_call_data(SCM x);

void init_dbus_pending_call_type();

#endif  /* ifndef __DBUS_PENDING_CALL_H__ */
