;; Extreme values test
;; Tests maximum and minimum values for integer types
(module
  ;; Function to test i32 maximum value
  (func (export "test_i32_max") (result i32)
    i32.const 2147483647  ;; 2^31 - 1
  )
  
  ;; Function to test i32 minimum value
  (func (export "test_i32_min") (result i32)
    i32.const -2147483648  ;; -2^31
  )
  
  ;; Function to test i32 unsigned maximum
  (func (export "test_i32_unsigned_max") (result i32)
    i32.const 4294967295  ;; 2^32 - 1
  )
  
  ;; Function to test i64 maximum value
  (func (export "test_i64_max") (result i64)
    i64.const 9223372036854775807  ;; 2^63 - 1
  )
  
  ;; Function to test i64 minimum value
  (func (export "test_i64_min") (result i64)
    i64.const -9223372036854775808  ;; -2^63
  )
  
  ;; Function to test i64 unsigned maximum
  (func (export "test_i64_unsigned_max") (result i64)
    i64.const 18446744073709551615  ;; 2^64 - 1
  )
  
  ;; Function to test very large positive number
  (func (export "test_large_positive") (result i64)
    i64.const 12345678901234567890
  )
  
  ;; Function to test very large negative number
  (func (export "test_large_negative") (result i64)
    i64.const -1234567890123456789
  )
)