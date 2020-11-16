(define-module (dbus message)
  #:export (dbus-message
            dbus-message?
            make-dbus-message
            make-dbus-message/method-call
            make-dbus-message/error))


(define (make-dbus-message type)
  (%make-dbus-message type))

(define* (make-dbus-message/method-call #:key
                                        destination
                                        path
                                        iface
                                        method)
  (%dbus-message-new-method-call destination path iface method))

(define* (make-dbus-message/error #:key
                                  message
                                  error-name
                                  error-message)
  (%make-dbus-message/error message error-name error-message))


(load-extension "libguile-dbus" "init_dbus_message")
