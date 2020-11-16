#ifndef __ERROR_H__
#define __ERROR_H__

#include <libguile.h>

#define GUILE_UDEV_ERROR "guile-dbus-error"

void gdbus_error(const char* proc, const char* msg, SCM args);

#endif  /* ifndef __ERROR_H__ */
