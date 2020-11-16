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

SCM_DEFINE(gdbus_make_dbus_message_error,
           "%make-dbus-message/error", 3, 0, 0,
           (SCM message, SCM error_name, SCM error_message),
           "Make a new message that is a reply to another message.")
#define FUNC_NAME s_gdbus_make_dbus_message_error
{
    struct dbus_message_data* data = _scm_to_dbus_message_data(message);
    char* c_error_name = NULL;
    char* c_error_message = NULL;
    DBusMessage* new_message = NULL;

    scm_dynwind_begin(0);

    c_error_name = scm_to_locale_string(error_name);
    scm_dynwind_free(c_error_name);

    c_error_message = scm_to_locale_string(error_message);
    scm_dynwind_free(c_error_message);

    new_message = dbus_message_new_error(data->message,
                                         c_error_name,
                                         c_error_message);
    if (new_message == NULL) {
        gdbus_error(FUNC_NAME, "Could not create a message",
                    scm_list_3(message, error_name, error_message));
    }

    scm_dynwind_end();
    return _scm_from_dbus_message(new_message);
}
#undef FUNC_NAME
