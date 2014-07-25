#!/usr/bin/guile \
-s
!#

;; C dynamic library
(if (not (defined? 'lib-bms-dynamic-link))
    (define lib-bms-dynamic-link (dynamic-link "./libbms-scm.so")))
(dynamic-call "ondas_init" lib-bms-dynamic-link)

(define pi 3.1415963545)

(define (sqr x) (let ((y x)) (* y y)))

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

;; Finds zero of f by bipartition method
(define solve-bipartition
  (lambda (f x1 x2 error max-number)
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
      (list xm (not (= ntries max-number-of-tries))))))

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
         (fout (open-output-file filename)))
    (do ((x min (+ x delta)))
        ((> x max))
      (format fout "~A ~A\n" x (f x)))
    (close fout)
    (system (format #f "echo 'plot \"~A\" using 1:2 smooth unique' | /usr/bin/gnuplot -persist" filename))))

(define dtrap90 (lambda (x) (- (trap90 x))))

(define maxmin90 (flux-max-min trap90 0 (* 2 pi) 0.01))
(define (flux-of-trap90 x)
  (- (integrate dtrap90 0 x 0.01) (/ (+ (car maxmin90) (cadr maxmin90)) 2)))


