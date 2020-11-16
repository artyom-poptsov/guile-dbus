(define-module (dbus message)
  #:export (dbus-message
            make-dbus-message
            dbus-message?))


(define* (make-dbus-message #:key (type 'invalid))
  (%make-dbus-message type))


(load-extension "libguile-dbus" "init_dbus_message")
