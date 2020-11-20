(define-module (dbus message)
  #:export (dbus-message
            dbus-message?
            make-dbus-message
            make-dbus-message/method-call
            make-dbus-message/method-return
            make-dbus-message/signal
            make-dbus-message/error
            dbus-message-get-args
            dbus-message-set-interface
            dbus-message-get-interface
            dbus-message-has-interface?))


(define (make-dbus-message type)
  (%make-dbus-message type))

(define* (make-dbus-message/method-call #:key
                                        destination
                                        path
                                        iface
                                        method)
  (%dbus-message-new-method-call destination path iface method))

(define (make-dbus-message/method-return message)
  (%make-dbus-message/method-return message))

(define* (make-dbus-message/signal #:key
                                   path
                                   iface
                                   name)
  (%make-dbus-message/signal path iface name))

(define* (make-dbus-message/error #:key
                                  message
                                  error-name
                                  error-message)
  (%make-dbus-message/error message error-name error-message))



(define (dbus-message-set-interface message iface)
  (%dbus-message-set-interface message iface))

(define (dbus-message-get-interface message)
  (%dbus-message-get-interface message))



(define (dbus-message-append-args message args)
  (%dbus-message-append messsage args))

(define (dbus-message-get-args message #:optional types)
  (%dbus-message-get-args message types))


(load-extension "libguile-dbus" "init_dbus_message")
