;; Basic arithmetic operations test
(module
  ;; Integer arithmetic tests
  (func $test_i32_add (export "test_i32_add") (result i32)
    i32.const 10
    i32.const 32
    i32.add
  )
  
  (func $test_i32_sub (export "test_i32_sub") (result i32)
    i32.const 100
    i32.const 25
    i32.sub
  )
  
  (func $test_i32_mul (export "test_i32_mul") (result i32)
    i32.const 12
    i32.const 8
    i32.mul
  )
  
  (func $test_i32_complex (export "test_i32_complex") (result i32)
    i32.const 5
    i32.const 10
    i32.add        ;; 5 + 10 = 15
    i32.const 3
    i32.mul        ;; 15 * 3 = 45
    i32.const 15
    i32.sub        ;; 45 - 15 = 30
  )
  
  ;; 64-bit integer arithmetic tests
  (func $test_i64_add (export "test_i64_add") (result i64)
    i64.const 10000000000
    i64.const 20000000000
    i64.add
  )
  
  (func $test_i64_sub (export "test_i64_sub") (result i64)
    i64.const 50000000000
    i64.const 25000000000
    i64.sub
  )
  
  (func $test_i64_mul (export "test_i64_mul") (result i64)
    i64.const 1000000
    i64.const 1000
    i64.mul
  )
  
  ;; Float arithmetic tests
  (func $test_f32_add (export "test_f32_add") (result f32)
    f32.const 1.5
    f32.const 2.5
    f32.add
  )
  
  (func $test_f32_sub (export "test_f32_sub") (result f32)
    f32.const 10.0
    f32.const 3.5
    f32.sub
  )
  
  (func $test_f32_mul (export "test_f32_mul") (result f32)
    f32.const 4.0
    f32.const 2.5
    f32.mul
  )
  
  (func $test_f64_add (export "test_f64_add") (result f64)
    f64.const 1.23456789
    f64.const 9.87654321
    f64.add
  )
  
  (func $test_f64_sub (export "test_f64_sub") (result f64)
    f64.const 100.0
    f64.const 42.123456789
    f64.sub
  )
  
  (func $test_f64_mul (export "test_f64_mul") (result f64)
    f64.const 12.34
    f64.const 56.78
    f64.mul
  )
  
  ;; Mixed arithmetic chain
  (func $test_mixed_arithmetic (export "test_mixed_arithmetic") (result i32)
    i32.const 2
    i32.const 3
    i32.add        ;; 2 + 3 = 5
    i32.const 4
    i32.mul        ;; 5 * 4 = 20
    i32.const 10
    i32.sub        ;; 20 - 10 = 10
    i32.const 5
    i32.add        ;; 10 + 5 = 15
  )
)