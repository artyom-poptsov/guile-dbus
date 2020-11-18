(add-to-load-path (getenv "abs_top_srcdir"))

(use-modules (srfi srfi-64)
             (dbus message))

(test-begin "message")

(test-assert "make-dbus-message"
  (let ((message (make-dbus-message)))
    (dbus-message? message)))

(test-end "message")
