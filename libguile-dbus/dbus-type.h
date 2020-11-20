#ifndef __DBUS_TYPE_H__
#define __DBUS_TYPE_H__

SCM dbus_type_to_scm(int type);
const struct symbol_mapping* dbus_type_from_scm(SCM type);
SCM dbus_value_to_scm(int type, DBusBasicValue value);

#endif  /* ifndef __DBUS_TYPE_H__ */
