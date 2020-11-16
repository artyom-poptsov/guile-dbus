(define-module (dbus message)
  #:export (dbus-message
            dbus-message?
            make-dbus-message
            make-dbus-message/method-call))


(define (make-dbus-message type)
  (%make-dbus-message type))

(define* (make-dbus-message/method-call #:key
                                        destination
                                        path
                                        iface
                                        method)
  (%dbus-message-new-method-call destination path iface method))


(load-extension "libguile-dbus" "init_dbus_message")
