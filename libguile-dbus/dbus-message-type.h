#ifndef __DBUS_MESSAGE_TYPE__
#define __DBUS_MESSAGE_TYPE__

#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"

extern scm_t_bits dbus_message_tag;


extern const struct symbol_mapping message_types[];

/**
 * Guile-DBus message SMOB data.
 */
struct gdbus_message {
    DBusMessage* message;
};

typedef struct gdbus_message gdbus_message_t;


gdbus_message_t* _scm_to_dbus_message_data(SCM x);
SCM _scm_from_dbus_message(DBusMessage* message);


void init_dbus_message_type();

#endif  /* ifndef __DBUS_MESSAGE_TYPE__ */
