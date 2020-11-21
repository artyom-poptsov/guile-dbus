(define-module (dbus pending-call)
  #:export (dbus-pending-call
            dbus-pending-call-block))


(load-extension "libguile-dbus" "init_dbus_pending_call")
