(add-to-load-path (getenv "abs_top_srcdir"))

(use-modules (srfi srfi-64)
             (dbus message))

(test-begin "message")

(test-assert "make-dbus-message"
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message? message)))

(test-assert "equal?"
  (let ((m1 (make-dbus-message 'method-call))
        (m2 (make-dbus-message 'method-call)))
    (and (equal? m1 m1)
         (not (equal? m1 m2)))))

(test-assert "dbus-message-{set,get}-interface"
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message-set-interface message "org.freedesktop.DBus.Introspectable")
    (string=? (dbus-message-get-interface message)
              "org.freedesktop.DBus.Introspectable")))

(test-assert "dbus-message-has-interface"
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message-set-interface message "org.freedesktop.DBus.Introspectable")
    (dbus-message-has-interface? message
                                 "org.freedesktop.DBus.Introspectable")))

(test-equal "dbus-message-{append,get}-args"
  42
  (let ((message (make-dbus-message 'method-call)))
    (dbus-message-append-args message '((byte 42)))
    (let ((args (dbus-message-get-args message '(byte))))
      (cadar args))))

(test-end "message")
