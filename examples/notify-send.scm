#!/usr/bin/guile \
-L modules -e main -s
!#

;; Invoke it as follows:
;;   ./notify-send.scm "title" "message"

(use-modules (dbus connection)
             (dbus message))


(define %destination "org.freedesktop.Notifications")
(define %path        "/org/freedesktop/Notifications")
(define %iface       "org.freedesktop.Notifications")
(define %method      "Notify")


(define (main args)
  (let ((conn (make-dbus-connection))
        (msg  (make-dbus-message/method-call
               #:destination %destination
               #:path        %path
               #:iface       %iface
               #:method      %method)))

    (let ((application-name '(string "Guile"))
          (id               '(uint32  0))
          (icon             '(string ""))
          (title            `(string ,(list-ref args 1)))
          (text             `(string ,(list-ref args 2)))
          (actions          `(array ("s"    ,(vector))))
          (hints            `(array ("{sv}" ,(vector))))
          (timeout          '(int32  100)))

      (dbus-message-append-args msg (list application-name
                                          id
                                          icon
                                          title
                                          text
                                          actions
                                          hints
                                          timeout)))
    (dbus-connection-send/with-reply-and-block conn msg 20000)
    (newline)))
