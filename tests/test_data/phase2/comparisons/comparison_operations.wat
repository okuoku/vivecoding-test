;; Comparison operations test
(module
  ;; Integer equality/inequality tests
  (func $test_i32_eq_true (export "test_i32_eq_true") (result i32)
    i32.const 42
    i32.const 42
    i32.eq         ;; returns 1 (true)
  )
  
  (func $test_i32_eq_false (export "test_i32_eq_false") (result i32)
    i32.const 42
    i32.const 24
    i32.eq         ;; returns 0 (false)
  )
  
  (func $test_i32_ne_true (export "test_i32_ne_true") (result i32)
    i32.const 42
    i32.const 24
    i32.ne         ;; returns 1 (true)
  )
  
  (func $test_i32_ne_false (export "test_i32_ne_false") (result i32)
    i32.const 42
    i32.const 42
    i32.ne         ;; returns 0 (false)
  )
  
  ;; Integer signed comparison tests
  (func $test_i32_lt_s_true (export "test_i32_lt_s_true") (result i32)
    i32.const 10
    i32.const 20
    i32.lt_s       ;; returns 1 (true)
  )
  
  (func $test_i32_lt_s_false (export "test_i32_lt_s_false") (result i32)
    i32.const 30
    i32.const 20
    i32.lt_s       ;; returns 0 (false)
  )
  
  (func $test_i32_gt_s_true (export "test_i32_gt_s_true") (result i32)
    i32.const 30
    i32.const 20
    i32.gt_s       ;; returns 1 (true)
  )
  
  (func $test_i32_le_s_true (export "test_i32_le_s_true") (result i32)
    i32.const 20
    i32.const 20
    i32.le_s       ;; returns 1 (true)
  )
  
  (func $test_i32_ge_s_true (export "test_i32_ge_s_true") (result i32)
    i32.const 20
    i32.const 20
    i32.ge_s       ;; returns 1 (true)
  )
  
  ;; Integer unsigned comparison tests
  (func $test_i32_lt_u_true (export "test_i32_lt_u_true") (result i32)
    i32.const 10
    i32.const 20
    i32.lt_u       ;; returns 1 (true)
  )
  
  (func $test_i32_lt_u_edge (export "test_i32_lt_u_edge") (result i32)
    i32.const -1   ;; 0xFFFFFFFF as unsigned
    i32.const 0
    i32.lt_u       ;; returns 0 (false), since -1 > 0 as unsigned
  )
  
  ;; 64-bit integer comparisons
  (func $test_i64_eq_true (export "test_i64_eq_true") (result i64)
    i64.const 12345678900
    i64.const 12345678900
    i64.eq         ;; returns 1 (true)
  )
  
  (func $test_i64_lt_s_true (export "test_i64_lt_s_true") (result i64)
    i64.const 10000000000
    i64.const 20000000000
    i64.lt_s       ;; returns 1 (true)
  )
  
  ;; Float comparisons
  (func $test_f32_eq_true (export "test_f32_eq_true") (result i32)
    f32.const 1.5
    f32.const 1.5
    f32.eq         ;; returns 1 (true)
  )
  
  (func $test_f32_eq_false (export "test_f32_eq_false") (result i32)
    f32.const 1.5
    f32.const 2.5
    f32.eq         ;; returns 0 (false)
  )
  
  (func $test_f32_lt_true (export "test_f32_lt_true") (result i32)
    f32.const 1.5
    f32.const 2.5
    f32.lt         ;; returns 1 (true)
  )
  
  (func $test_f32_lt_false (export "test_f32_lt_false") (result i32)
    f32.const 2.5
    f32.const 1.5
    f32.lt         ;; returns 0 (false)
  )
  
  (func $test_f64_eq_true (export "test_f64_eq_true") (result i32)
    f64.const 3.141592653589793
    f64.const 3.141592653589793
    f64.eq         ;; returns 1 (true)
  )
  
  (func $test_f64_lt_true (export "test_f64_lt_true") (result i32)
    f64.const 1.23456789
    f64.const 9.87654321
    f64.lt         ;; returns 1 (true)
  )
  
  ;; Complex comparison chain
  (func $test_comparison_chain (export "test_comparison_chain") (result i32)
    i32.const 10
    i32.const 20
    i32.lt_s       ;; 10 < 20 = 1 (true)
    i32.const 5
    i32.const 3
    i32.gt_s       ;; 5 > 3 = 1 (true)
    i32.and        ;; 1 & 1 = 1 (true)
    i32.const 42
    i32.const 42
    i32.eq         ;; 42 == 42 = 1 (true)
    i32.and        ;; 1 & 1 = 1 (true)
  )
)