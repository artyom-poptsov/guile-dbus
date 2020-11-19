(add-to-load-path (getenv "abs_top_srcdir"))

(use-modules (srfi srfi-64)
             (dbus message))

(test-begin "message")

(test-assert "make-dbus-message"
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message? message)))

(test-assert "dbus-message-{set,get}-interface"
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message-set-interface message "org.freedesktop.DBus.Introspectable")
    (string=? (dbus-message-get-interface message)
              "org.freedesktop.DBus.Introspectable")))

(test-end "message")
