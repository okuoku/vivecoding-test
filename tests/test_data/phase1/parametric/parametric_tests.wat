;; Parametric instructions test
;; Tests drop and select instructions
(module
  ;; Function to test drop with single value
  (func (export "test_drop_single") (result i32)
    i32.const 123    ;; This value will be dropped
    drop
    i32.const 456    ;; This is the actual result
  )
  
  ;; Function to test drop with expression
  (func (export "test_drop_expression") (result i32)
    i32.const 10    ;; This expression result will be dropped
    i32.const 20
    i32.add         ;; 10 + 20 = 30 (will be dropped)
    drop
    i32.const 99    ;; Final result
  )
  
  ;; Function to test select with true condition
  (func (export "test_select_true") (result i32)
    i32.const 1     ;; true condition
    i32.const 42    ;; true value
    i32.const 24    ;; false value
    select          ;; Should select 42
  )
  
  ;; Function to test select with false condition
  (func (export "test_select_false") (result i32)
    i32.const 0     ;; false condition
    i32.const 42    ;; true value
    i32.const 24    ;; false value
    select          ;; Should select 24
  )
  
  ;; Function to test select with i64 values
  (func (export "test_select_i64") (result i64)
    i32.const 1     ;; condition (must be i32)
    i64.const 10000000000  ;; true value
    i64.const 20000000000  ;; false value
    select          ;; Should select first i64 value
  )
  
  ;; Function to test select with f32 values
  (func (export "test_select_f32") (result f32)
    i32.const 0     ;; condition (must be i32)
    f32.const 3.14  ;; true value
    f32.const 2.71  ;; false value
    select          ;; Should select 2.71
  )
  
  ;; Function to test select with f64 values
  (func (export "test_select_f64") (result f64)
    i32.const 1     ;; condition (must be i32)
    f64.const 1.41421356237  ;; true value
    f64.const 1.73205080757  ;; false value
    select                    ;; Should select sqrt(2)
  )
)