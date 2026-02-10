;; Variable access test
;; Tests local.get, local.set, local.tee, global.get, global.set
(module
  
  ;; Define some global variables
  (global $global_i32 (mut i32) (i32.const 100))
  (global $global_i64 (mut i64) (i64.const 2000000000))
  (global $global_const_i32 i32 (i32.const 42))  ;; immutable
  
  ;; Function to test local.get
  (func (export "test_local_get") (param i32) (result i32)
    local.get 0      ;; Get the first parameter
  )
  
  ;; Function to test local.set
  (func (export "test_local_set") (param i32) (result i32)
    (local i32)      ;; Define local variable 1
    local.set 1 (i32.const 55)     ;; Set local 1 to 55
    local.get 1                     ;; Return local 1
  )
  
  ;; Function to test local.tee (set and return)
  (func (export "test_local_tee") (param i32) (result i32)
    (local i32)      ;; Define local variable 1
    local.tee 1 (i32.const 77)     ;; Set local 1 to 77 and return it
    drop            ;; Discard the original parameter
    local.get 1     ;; Return local 1 again (should be 77)
  )
  
  ;; Function to test multiple locals
  (func (export "test_multiple_locals") (param i32) (param i32) (result i32)
    (local i32) (local i32)  ;; Define 2 more locals
    local.set 2 (i32.const 10)     ;; Set local 2 = 10
    local.set 3 (i32.const 20)     ;; Set local 3 = 20
    
    local.get 0    ;; param 0
    local.get 2    ;; local 2 (10)
    i32.add        ;; param 0 + 10
    
    local.get 1    ;; param 1  
    local.get 3    ;; local 3 (20)
    i32.add        ;; param 1 + 20
    
    i32.add        ;; (param 0 + 10) + (param 1 + 20)
  )
  
  ;; Function to test global.get
  (func (export "test_global_get") (result i32)
    global.get $global_i32     ;; Should return 100
  )
  
  ;; Function to test global.set
  (func (export "test_global_set") (param i32) (result i32)
    global.set $global_i32 (local.get 0)    ;; Set global to parameter
    global.get $global_i32                   ;; Return new global value
  )
  
  ;; Function to test immutable global
  (func (export "test_immutable_global") (result i32)
    global.get $global_const_i32    ;; Should always return 42
  )
  
  ;; Function to test i64 global
  (func (export "test_global_i64") (result i64)
    global.get $global_i64     ;; Should return 2000000000
  )
  
  ;; Function to test local/global interaction
  (func (export "test_local_global_mix") (param i32) (result i32)
    (local i32)      ;; local variable 1
    local.set 1 (global.get $global_i32)     ;; local 1 = global (100)
    global.set $global_i32 (local.get 0)     ;; global = parameter
    local.get 1    ;; return old global value (100)
  )
)