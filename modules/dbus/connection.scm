(define-module (dbus connection)
  #:export (dbus-connection
            make-dbus-connection
            dbus-connection?
            dbus-connection-send
            dbus-connection-send/with-reply
            dbus-connection-flush
            dbus-connection-open?))


(define* (make-dbus-connection #:key (type 'session))
  (%make-dbus-connection type))

(define* (dbus-connection-send/with-reply connection message
                                          #:optional (timeout -1))
  (%dbus-connection-send/with-reply connection message timeout))


(load-extension "libguile-dbus" "init_dbus_pending_call")
(load-extension "libguile-dbus" "init_dbus_connection")

