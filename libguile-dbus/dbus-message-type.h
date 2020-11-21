#ifndef __DBUS_MESSAGE_TYPE__
#define __DBUS_MESSAGE_TYPE__

#include <libguile.h>
#include <dbus/dbus.h>

#include "common.h"

extern scm_t_bits gdbus_message_tag;


extern const struct symbol_mapping message_types[];

/**
 * Guile-DBus message SMOB data.
 */
struct gdbus_message {
    DBusMessage* message;
};

typedef struct gdbus_message gdbus_message_t;


gdbus_message_t* make_gdbus_message();
SCM dbus_message_to_scm(DBusMessage* message);
gdbus_message_t* gdbus_message_from_scm(SCM x);


void init_dbus_message_type();

#endif  /* ifndef __DBUS_MESSAGE_TYPE__ */
