#ifndef __DBUS_TYPE_H__
#define __DBUS_TYPE_H__

SCM dbus_type_to_scm(int type);
const symbol_mapping_t* dbus_type_from_scm(SCM type);
SCM dbus_value_to_scm(int type, DBusBasicValue value);

SCM dbus_message_type_to_scm(int type);
const symbol_mapping_t* dbus_message_type_from_scm(SCM type);

SCM dbus_bus_type_to_scm(DBusBusType type);
const symbol_mapping_t* dbus_bus_type_from_scm(SCM type);

#endif  /* ifndef __DBUS_TYPE_H__ */
