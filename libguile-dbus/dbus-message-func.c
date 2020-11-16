#include <dbus/dbus.h>
#include <libguile.h>

#include "error.h"
#include "dbus-message-type.h"

SCM_DEFINE(gdbus_message_new_method_call,
           "%make-dbus-message/method-call", 4, 0, 0,
           (SCM destination, SCM path, SCM iface, SCM method),
           "Make a new message to invoke a method on a remote object.")
#define FUNC_NAME s_gdbus_message_new_method_call
{
    char* c_destination;
    char* c_path;
    char* c_iface;
    char* c_method;
    DBusMessage* message;

    scm_dynwind_begin(0);

    c_destination = scm_to_locale_string(destination);
    scm_dynwind_free(c_destination);

    c_path = scm_to_locale_string(path);
    scm_dynwind_free(c_path);

    c_iface = scm_to_locale_string(iface);
    scm_dynwind_free(c_iface);

    c_method = scm_to_locale_string(method);
    scm_dynwind_free(c_method);

    message = dbus_message_new_method_call(c_destination,
                                           c_path,
                                           c_iface,
                                           c_method);
    if (message == NULL) {
        gdbus_error(FUNC_NAME, "Could not create a message",
                    scm_list_4(destination, path, iface, method));
    }

    scm_dynwind_end();

    return _scm_from_dbus_message(message);
}
#undef FUNC_NAME
