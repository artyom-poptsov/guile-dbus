#include <dbus/dbus.h>
#include <libguile.h>
#include <assert.h>

#include "common.h"
#include "symbols.h"
#include "error.h"
#include "dbus-message-type.h"

GDBUS_DEFINE(gdbus_make_dbus_message, "%make-dbus-message", 1, (SCM type),
             "Make a DBus message")
#define FUNC_NAME s_gdbus_make_dbus_message
{
    const symbol_mapping_t* c_type = dbus_message_type_from_scm(type);
    DBusMessage* message = dbus_message_new(c_type->value);
    return dbus_message_to_scm(message);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_is_message_p, "dbus-message?", 1, (SCM x),
            "Return #t if X is a DBus message, #f otherwise.")
{
/* #if USING_GUILE_BEFORE_2_2 */
    return scm_from_bool (SCM_SMOB_PREDICATE (gdbus_message_tag, x));
/* #else */
/*     return scm_from_bool (SCM_PORTP(x) */
/*                           && (SCM_PORT_TYPE(x) == dbus_message_tag)); */
/* #endif */
}


GDBUS_DEFINE(gdbus_message_new_method_call, "%make-dbus-message/method-call", 4,
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

    return dbus_message_to_scm(message);
}
#undef FUNC_NAME


GDBUS_DEFINE(gdbus_message_new_error, "%make-dbus-message/error", 3,
             (SCM message, SCM error_name, SCM error_message),
             "Make a new message that is a reply to another message.")
#define FUNC_NAME s_gdbus_message_new_error
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
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
    return dbus_message_to_scm(new_message);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_messsage_new_method_return,
             "%make-dbus-message/method-return", 1,
             (SCM method_call),
             "Constructs a message that is a reply to a method call.")
#define FUNC_NAME s_gdbus_messsage_new_method_return
{
    gdbus_message_t* data = gdbus_message_from_scm(method_call);
    DBusMessage* new_message = dbus_message_new_method_return(data->message);
    if (new_message == NULL) {
        gdbus_error(FUNC_NAME, "Could not create a message", method_call);
    }

    return dbus_message_to_scm(new_message);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_messsage_new_signal, "%make-dbus-message/signal", 3,
             (SCM path, SCM iface, SCM name),
             "Constructs a new message representing a signal emission.")
#define FUNC_NAME s_gdbus_messsage_new_signal
{
    DBusMessage* new_message = NULL;
    char* c_path  = NULL;
    char* c_iface = NULL;
    char* c_name  = NULL;

    scm_dynwind_begin(0);

    c_path = scm_to_locale_string(path);
    scm_dynwind_free(c_path);

    c_iface = scm_to_locale_string(iface);
    scm_dynwind_free(c_iface);

    c_name = scm_to_locale_string(name);
    scm_dynwind_free(c_name);

    new_message = dbus_message_new_signal(c_path, c_iface, c_name);
    if (new_message == NULL) {
        gdbus_error(FUNC_NAME, "Could not create a message",
                    scm_list_3(path, iface, name));
    }

    scm_dynwind_end();

    return dbus_message_to_scm(new_message);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_append_args, "%dbus-message-append-args", 2,
             (SCM message, SCM args),
             "Append arguments ARGS to the MESSAGE.")
#define FUNC_NAME s_gdbus_message_append_args
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    DBusMessageIter iter;
    int list_size;
    SCM param;
    int idx;

    SCM_ASSERT(scm_to_bool(scm_list_p(args)), args, SCM_ARG2, FUNC_NAME);

    dbus_message_iter_init_append(data->message, &iter);

    list_size = scm_to_int32(scm_length(args));
    for (idx = 0; idx < list_size; ++idx) {
        param = scm_list_ref(args, scm_from_int(idx));
        if (scm_list_p(param)) {
            SCM scm_type  = scm_list_ref(param, scm_from_int(0));
            SCM scm_value = scm_list_ref(param, scm_from_int(1));
            const symbol_mapping_t* symbol = dbus_type_from_scm(scm_type);
            if (! symbol) {
                gdbus_error(FUNC_NAME, "Unknown type", scm_type);
            }
            switch (symbol->value) {
            case DBUS_TYPE_BYTE: {
                unsigned char value = scm_to_uchar(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_INT16: {
                dbus_int16_t value = scm_to_short(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_INT32: {
                dbus_int32_t value = scm_to_int32(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_UINT16: {
                dbus_uint16_t value = scm_to_uint16(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_UINT32: {
                dbus_uint32_t value = scm_to_uint32(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_INT64: {
                dbus_int64_t value = scm_to_int64(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }
            case DBUS_TYPE_STRING: {
                char* value = scm_to_locale_string(scm_value);
                dbus_message_iter_append_basic(&iter, symbol->value, &value);
                break;
            }

            case DBUS_TYPE_ARRAY: {
                int length = scm_to_int(scm_vector_length(scm_value));
                DBusMessageIter container_iter;
                dbus_bool_t ret = dbus_message_iter_open_container(
                    &iter,
                    DBUS_TYPE_ARRAY,
                    DBUS_TYPE_STRING_AS_STRING,
                    &container_iter);

                assert(ret == TRUE);

                for (int idx = 0; idx < length; ++idx) {
                    SCM v = scm_vector_ref(scm_value, scm_from_int(idx));
                    char* cv = scm_to_locale_string(v);
                    dbus_message_iter_append_basic(
                        &container_iter,
                        symbol->value,
                        &cv);
                }

                dbus_message_iter_close_container(&iter, &container_iter);
                break;
            }

            case DBUS_TYPE_UINT64:
            case DBUS_TYPE_DOUBLE:
                gdbus_error(FUNC_NAME, "Unsupported yet",
                            scm_list_2(message, args));
            }
        }
    }

    /* if (! dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &param)) { */
    /*     gdbus_error(FUNC_NAME, "Out of memory", scm_list_3(message, args)); */
    /* } */

    return SCM_UNDEFINED;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_get_args, "%dbus-message-get-args", 2,
             (SCM message, SCM types),
             "")
#define FUNC_NAME s_gdbus_message_get_args
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    DBusMessageIter iter;
    SCM result = scm_list_n(SCM_UNDEFINED);

    SCM_ASSERT(scm_to_bool(scm_list_p(types)) || scm_is_false(types), types,
               SCM_ARG2, FUNC_NAME);

    if (scm_is_false(types)) {
        if (! dbus_message_iter_init(data->message, &iter)) {
            return result;
        }

        for (; dbus_message_iter_has_next(&iter); dbus_message_iter_next(&iter)) {
            int c_type = dbus_message_iter_get_arg_type(&iter);
            SCM type = dbus_type_to_scm(c_type);
            DBusBasicValue c_value;

            dbus_message_iter_get_basic(&iter, &c_value);

            SCM value = dbus_value_to_scm(c_type, c_value);
            result = scm_append(scm_list_2(result, scm_list_2(type, value)));
        }
    } else {
        if (! dbus_message_iter_init(data->message, &iter)) {
            gdbus_error(FUNC_NAME, "Message has no arguments", message);
        }

        int types_count = scm_to_int(scm_length(types));
        const symbol_mapping_t* required_type;

        for (int idx = 0; idx < types_count; ++idx) {
            int c_type = dbus_message_iter_get_arg_type(&iter);
            if (c_type == DBUS_TYPE_INVALID) {
                gdbus_error(FUNC_NAME, "Message has not enough arguments",
                            scm_list_2(message, types));
            }

            required_type
                = dbus_type_from_scm(scm_list_ref(types, scm_from_int(idx)));

            if (c_type != required_type->value) {
                gdbus_error(FUNC_NAME, "Types mismatch",
                            scm_list_3(message, types, scm_from_int(idx)));
            }

            SCM type = dbus_type_to_scm(c_type);
            DBusBasicValue c_value;

            dbus_message_iter_get_basic(&iter, &c_value);

            SCM value = dbus_value_to_scm(c_type, c_value);
            result = scm_append(scm_list_2(result, scm_list_1(scm_list_2(type, value))));

            dbus_message_iter_next(&iter);
        }
    }

    return result;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_set_interface, "%dbus-message-set-interface", 2,
             (SCM message, SCM iface),
             "Sets the interface this message is being sent to (for"
             " DBUS_MESSAGE_TYPE_METHOD_CALL) or the interface a signal is being emitted"
             " from (for DBUS_MESSAGE_TYPE_SIGNAL).")
#define FUNC_NAME s_gdbus_message_set_interface
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    char* c_iface = NULL;

    SCM_ASSERT(scm_is_string(iface) || scm_is_false(iface), iface, SCM_ARG2, FUNC_NAME);

    scm_dynwind_begin(0);
    if (scm_is_string(iface)) {
        c_iface = scm_to_locale_string(iface);
        scm_dynwind_free(c_iface);
    }

    if (! dbus_message_set_interface(data->message, c_iface)) {
        gdbus_error(FUNC_NAME, "Out of memory", scm_list_2(message, iface));
    }

    scm_dynwind_end();
    return SCM_UNDEFINED;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_get_interface, "%dbus-message-get-interface", 1,
             (SCM message),
             "Gets the interface this message is being sent to "
             " or emitted from.")
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    const char* interface = dbus_message_get_interface(data->message);
    return scm_from_locale_string(interface);
}

GDBUS_DEFINE(gdbus_message_has_interface_p, "dbus-message-has-interface?", 2,
             (SCM message, SCM interface),
             "Checks if the @var{message} has an @var{interface}.")
#define FUNC_NAME s_gdbus_message_has_interface_p
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    dbus_bool_t result;
    char* c_interface;

    SCM_ASSERT(scm_is_string(interface), interface, SCM_ARG2, FUNC_NAME);

    scm_dynwind_begin(0);

    c_interface = scm_to_locale_string(interface);
    scm_dynwind_free(c_interface);

    result = dbus_message_has_interface(data->message, c_interface);

    scm_dynwind_end();

    return scm_from_bool(result);
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_get_sender, "dbus-message-get-sender", 1,
             (SCM message),
             "Gets the unique name of the connection which originated"
             " this @var{message}, or SCM_BOOL_F if unknown or inapplicable.")
#define FUNC_NAME s_gdbus_message_get_sender
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    const char* result = dbus_message_get_sender(data->message);
    return result ? scm_from_locale_string(result) : SCM_BOOL_F;
}
#undef FUNC_NAME

GDBUS_DEFINE(gdbus_message_get_serial, "dbus-message-get-serial", 1,
             (SCM message),
             "Returns the serial of a message or 0 if none has been specified.")
#define FUNC_NAME s_gdbus_message_get_serial
{
    gdbus_message_t* data = gdbus_message_from_scm(message);
    return scm_from_int(dbus_message_get_serial(data->message));
}
#undef FUNC_NAME


void init_dbus_message_func()
{
#include "dbus-message-func.x"
}
