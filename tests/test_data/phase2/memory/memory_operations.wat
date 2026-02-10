;; Basic memory load/store test
(module
  (memory 1)  ;; One page (64KB) of memory
  (export "memory" (memory 0))
  
  ;; Store values at different offsets and read them back
  (func $test_i32_store_load (export "test_i32_store_load") (result i32)
    i32.const 42        ;; value to store
    i32.const 0         ;; offset 0
    i32.store           ;; store at offset 0
    i32.const 0         ;; offset 0  
    i32.load            ;; load from offset 0
  )
  
  (func $test_i64_store_load (export "test_i64_store_load") (result i64)
    i64.const 12345678900
    i32.const 8         ;; offset 8 (i64 needs 8 bytes)
    i64.store
    i32.const 8
    i64.load
  )
  
  (func $test_f32_store_load (export "test_f32_store_load") (result f32)
    f32.const 3.14159
    i32.const 16        ;; offset 16
    f32.store
    i32.const 16
    f32.load
  )
  
  (func $test_f64_store_load (export "test_f64_store_load") (result f64)
    f64.const 2.718281828459045
    i32.const 24        ;; offset 24 (f64 needs 8 bytes)
    f64.store
    i32.const 24
    f64.load
  )
  
  ;; Test with offsets
  (func $test_offset_store_load (export "test_offset_store_load") (result i32)
    i32.const 100
    i32.const 100       ;; offset 100 bytes
    i32.store
    i32.const 100
    i32.load
  )
  
  ;; Test multiple stores/loads
  (func $test_multiple_operations (export "test_multiple_operations") (result i32)
    ;; Store first value
    i32.const 10
    i32.const 0
    i32.store
    ;; Store second value
    i32.const 20
    i32.const 4
    i32.store
    ;; Store third value
    i32.const 30
    i32.const 8
    i32.store
    ;; Load and sum all values
    i32.const 0
    i32.load
    i32.const 4
    i32.load
    i32.add
    i32.const 8
    i32.load
    i32.add
  )
)