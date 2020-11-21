#ifndef __DBUS_PENDING_CALL_H__
#define __DBUS_PENDING_CALL_H__

#include <libguile.h>
#include <dbus/dbus.h>

extern scm_t_bits gdbus_pending_call_tag;

/**
 * Guile-DBus message SMOB data.
 */
struct gdbus_pending_call {
    DBusPendingCall* call;
};

typedef struct gdbus_pending_call gdbus_pending_call_t;


SCM dbus_pending_call_to_scm(DBusPendingCall* call);
gdbus_pending_call_t* gdbus_pending_call_from_scm(SCM x);


void init_dbus_pending_call_type();

#endif  /* ifndef __DBUS_PENDING_CALL_H__ */
