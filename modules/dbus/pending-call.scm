(define-module (dbus pending-call)
  #:export (dbus-pending-call
            dbus-pending-call-block
            dbus-pending-call-cancel
            dbus-pending-call-steal-reply
            dbus-pending-call-completed?))


(load-extension "libguile-dbus" "init_dbus_pending_call")
