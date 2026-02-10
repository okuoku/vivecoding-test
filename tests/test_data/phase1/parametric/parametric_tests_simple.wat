;; Simple parametric instructions test
;; Tests basic drop and select instructions
(module
  ;; Function to test drop with single value
  (func (export "test_drop_single") (result i32)
    i32.const 123    ;; This value will be dropped
    drop
    i32.const 456    ;; This is the actual result
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
)