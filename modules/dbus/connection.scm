(define-module (dbus connection)
  #:export (dbus-connection
            make-dbus-connection
            dbus-connection?
            dbus-connection-send))


(define* (make-dbus-connection #:key (type 'session))
  (%make-dbus-connection type))


(load-extension "libguile-dbus" "init_dbus_connection")
