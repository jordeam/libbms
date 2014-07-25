#!/usr/bin/guile \
-s
!#

(use-modules (ice-9 popen)
             (ice-9 optargs))

;; C dynamic library
(if (not (defined? 'lib-bms-dynamic-link))
    (define lib-bms-dynamic-link (dynamic-link "libbms-scm.so")))
(dynamic-call "ondas_init" lib-bms-dynamic-link)

(define pi 3.1415963545)

(define (sqr x) (let ((y x)) (* y y)))

(define (deg->rd x) (* (/ x 180) pi))

(define (rd->deg x) (* (/ x pi) 180))

(define (integrate f x1 x2 dx)
  (let ((soma 0)
        (x x1))
    (while (< x x2)
           (set! soma (+ soma (* (f x) dx)))
           (set! x (+ x dx)))
    soma))

;; return the maximum and minimum values of flux of function f, inside
;; x1 and x2, with a pass of dx
(define (flux-max-min f x1 x2 dx)
  ;; the defined integral of a funtion will always starts from zero
  (let ((max 0)
        (min 0))
    ;; eval maximum and minimum values
    (do ((x x1 (+ x dx)))
        ((> x x2))
      (let ((acc (integrate f x1 x dx)))
        (if (> acc max)
            (set! max acc))
        (if (< acc min)
            (set! min acc))))
    (list max min)))

;; Solve  by bipartition method.  It has  a simple  scan: if  signs of
;; f(x1) and f(x2) are the same, simply returns #f
(define (solve-bipartition-1 f x1 x2 error)
  (call-with-current-continuation
   (lambda (return)
     (let ((x1 (min x1 x2))
           (x2 (max x1 x2))
           (xm 0)
           (fm 0))
       (let ((f1 (f x1))
             (f2 (f x2)))
         (cond
          ((< (abs f1) error)
           (return x1))
          ((< (abs f2) error)
           (return x2))
          ((> (* f1 f2) 0)
           ;; they have the same sign, return false
           (return #f))
          (else
           (while (> (abs (begin
                            (set! xm (/ (+ x1 x2) 2.0))
                            (set! fm (f xm))
                            fm))
                     error)
             (cond
              ((< (* f1 fm) 0)
               (set! x2 xm))
              (else ;; (< (* f2 fm) 0)
               (set! x1 xm))))
           xm)))))))

;; Finds zero of f by bipartition method, in n-int subintervals (TODO)
(define (solve-bipartition f x1 x2 error n-int)
  (let ((delta-x (/ (- x2 x1) n-int))
        (res '()))
    (do ((x x1 (+ x delta-x))
         (i 0 (1+ i)))
        ((>= i n-int))
      (let ((r (solve-bipartition-1 f x (+ x delta-x) error)))
        (if r
            (set! res (append res (list r))))))
    (if (= (length res) 0)
        #f
        res)))

(define (solve-bipartition-old f x1 x2 error max-number)
  (let ((temp x1)
        (ntries 0)
        (dx 0)
        (xm 0)
        (fm 0)
        (f1 0)
        (f2 0)
        (max-number-of-tries (or max-number max-number-of-tries)))
    (if (> x1 x2)
        (begin
          (set! temp x1)
          (set! x1 x2)
          (set! x2 temp)))
    (set! ntries 0)
    (set! dx (/ (- x2 x1) max-number-of-tries))
    (while (and (> (abs (begin
                          (set! xm (/ (+ x1 x2) 2.0))
                          (set! fm (f xm))
                          fm))
                   error)
                (< ntries max-number-of-tries))
      (set! f1 (f x1))
      (set! f2 (f x2))
      (cond
       ((< (* f1 fm) 0) (set! x2 xm))
       ((< (* f2 fm) 0) (set! x1 xm))
       (else (set! ntries (+ 1 ntries))
             (set! x1 (+ x1 dx)))))
    (list xm (not (= ntries max-number-of-tries)))))

(define (H-sin n f)
  (let ((fsin (lambda (theta) (* (f theta) (sin (* n theta))))))
    (/ (integrate fsin 0 (* 2 pi) (/ pi 1000)) pi)))

(define (H-cos n f)
  (let ((fsin (lambda (theta) (* (f theta) (cos (* n theta))))))
    (/ (integrate fsin 0 (* 2 pi) (/ pi 1000)) pi)))

(define (HARM n f)
  (sqrt (+ (sqr (H-sin n f)) (sqr (H-cos n f)))))

(define (DC f)
  (/ (integrate f 0 (* 2 pi) (/ pi 1000)) (* 2 pi)))

(define (RMS f)
  (sqrt (/ (integrate (lambda (x) (sqr (f x))) 0 (* 2 pi) (/ pi 1000))
           (* 2 pi))))

(define (THD f)
  (let ((HR1 (/ (HARM 1 f) (sqrt 2))))
    (/ (sqrt (- (sqr (RMS f)) (sqr (DC f)) (sqr HR1)))
       HR1)))

(define (ripple f)
  (let ((dc (DC f)))
    (/ (sqrt (- (sqr (RMS f)) (sqr dc)))
       dc)))

(define (gnuplot f min max delta)
  (let* ((filename (tmpnam))
         (fout (open-output-file filename))
         (n (floor (abs (/ (- max min) delta)))))
    (do ((i 1 (1+ i))
         (x min (+ x delta)))
        ((> i n))
      (format fout "~A ~A\n" x (f x)))
    (close fout)
    (system (format #f "echo 'set mouse; plot \"~A\" using 1:2 smooth unique' | /usr/bin/gnuplot -persist" filename))))

(define* (gnuplot-vector v-funcs #:key title xlabel ylabel #:rest cmds)
          (let ((op (open-output-pipe "/usr/bin/gnuplot -persist"))
                (first #t))
            (format op "set mouse\n")
            (if title
                (format op "set title '~A'\n" title))
            (if xlabel
                (format op "set xlabel '~A'\n" xlabel))
            (if ylabel
                (format op "set ylabel '~A'\n" ylabel))
            ;; for each graph:
            (for-each
             (lambda (v-func)
               (let* ((f (car v-func))
                      (min (cadr v-func))
                      (max (caddr v-func))
                      (delta (cadddr v-func))
                      (extra (list-ref v-func 4))
                      (n (floor (abs (/ (- max min) delta))))
                      (filename (tmpnam))
                      (fout (open-output-file filename)))
                 (do ((i 1 (1+ i))
                      (x min (+ x delta)))
                     ((> i n))
                   (format fout "~A ~A\n" (* i delta) (f (* i delta))))
                 (close fout)
                 (format op "~A \"~A\" using 1:2 smooth unique ~A;\n"
                         (if first (begin (set! first #f) "plot") "replot")
                         filename extra)))
             v-funcs)
            (for-each
             (lambda (cmd)
               (format op "~A\n" cmd))
             cmds)
            (close-pipe op)))

(define* (f->dat filename f xmin xmax step #:optional comment)
  (let ((fout (open-output-file filename))
        (n (floor (abs (/ (- xmax xmin) step)))))
    (if comment
        (format fout "~A\n" comment))
    (do ((i 1 (1+ i))
         (x xmin (+ x step)))
        ((> i n))
      (format fout "~A ~A\n" x (f x)))
    (close fout)))
    
(define* (f-vector->dat filename f-vec xmin xmax step #:optional comment)
  (let ((fout (open-output-file filename))
        (n (floor (abs (/ (- xmax xmin) step)))))
    (if comment
        (format fout "~A\n" comment))
    (do ((i 1 (1+ i))
         (x xmin (+ x step)))
        ((> i n))
      (display x fout)
      (for-each
       (lambda (f)
         (format fout " ~A" (f x)))
       f-vec)
      (newline fout))
    (close fout)))

(define dtrap90 (lambda (x) (- (trap90 x))))

(define maxmin90 (flux-max-min trap90 0 (* 2 pi) 0.01))
(define (flux-of-trap90 x)
  (- (integrate dtrap90 0 x 0.01) (/ (+ (car maxmin90) (cadr maxmin90)) 2)))


