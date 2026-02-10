;; Special floating-point values test
;; Tests NaN, Infinity, negative infinity, and negative zero
(module
  ;; Function to test positive infinity
  (func (export "test_f32_inf") (result f32)
    f32.const inf
  )
  
  ;; Function to test negative infinity
  (func (export "test_f32_neg_inf") (result f32)
    f32.const -inf
  )
  
  ;; Function to test NaN
  (func (export "test_f32_nan") (result f32)
    f32.const nan
  )
  
  ;; Function to test positive infinity (f64)
  (func (export "test_f64_inf") (result f64)
    f64.const inf
  )
  
  ;; Function to test negative infinity (f64)
  (func (export "test_f64_neg_inf") (result f64)
    f64.const -inf
  )
  
  ;; Function to test NaN (f64)
  (func (export "test_f64_nan") (result f64)
    f64.const nan
  )
  
  ;; Function to test negative zero (f32)
  (func (export "test_f32_neg_zero") (result f32)
    f32.const -0.0
  )
  
  ;; Function to test negative zero (f64)
  (func (export "test_f64_neg_zero") (result f64)
    f64.const -0.0
  )
)