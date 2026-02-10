;; Simple integer and floating-point constants test
;; Tests basic i32, i64, f32, f64 constant instructions
(module
  ;; Function to test i32 constant
  (func (export "test_i32") (result i32)
    i32.const 42
  )
  
  ;; Function to test i64 constant
  (func (export "test_i64") (result i64)
    i64.const 9007199254740992
  )
  
  ;; Function to test f32 constant
  (func (export "test_f32") (result f32)
    f32.const 3.14159
  )
  
  ;; Function to test f64 constant
  (func (export "test_f64") (result f64)
    f64.const 2.718281828459045
  )
  
  ;; Function to test negative constants
  (func (export "test_negative") (result i32)
    i32.const -123
  )
  
  ;; Function to test zero constants
  (func (export "test_zero") (result i64)
    i64.const 0
  )
)