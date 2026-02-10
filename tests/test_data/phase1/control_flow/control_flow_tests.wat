;; Basic control flow test
;; Tests block, loop, if, br, br_if, return, call
(module
  
  ;; Helper function for testing calls
  (func $helper_add (param i32) (param i32) (result i32)
    local.get 0
    local.get 1
    i32.add
  )
  
  ;; Function to test block
  (func (export "test_block") (result i32)
    (block (result i32)
      i32.const 42
    )
  )
  
  ;; Function to test block with branch
  (func (export "test_block_branch") (result i32)
    (block (result i32)
      (br 0 (i32.const 99))  ;; Branch out with value 99
      i32.const 42           ;; This never executes
    )
  )
  
  ;; Function to test simple if (true)
  (func (export "test_if_true") (result i32)
    (if (result i32)
      (i32.const 1)         ;; condition (true)
      (then 
        i32.const 42
      )
      (else 
        i32.const 24
      )
    )
  )
  
  ;; Function to test simple if (false)
  (func (export "test_if_false") (result i32)
    (if (result i32)
      (i32.const 0)         ;; condition (false)
      (then 
        i32.const 42
      )
      (else 
        i32.const 24
      )
    )
  )
  
  ;; Function to test if without else
  (func (export "test_if_no_else") (result i32)
    i32.const 10
    (if 
      (i32.const 1)         ;; condition (true)
      (then 
        drop
        i32.const 20
      )
    )
    i32.const 30
    i32.add                 ;; 20 + 30 = 50
  )
  
  ;; Function to test loop (simple counter simulation)
  (func (export "test_loop_simple") (result i32)
    (loop $continue
      i32.const 5
      br_if $continue (i32.const 0)  ;; Never branch, just exit loop
      i32.const 10
    )
    i32.const 15
    i32.add                 ;; 10 + 15 = 25
  )
  
  ;; Function to test br with condition (false)
  (func (export "test_br_if_false") (result i32)
    (block (result i32)
      i32.const 42
      br_if 0 (i32.const 0)  ;; Don't branch (condition false)
      i32.const 10
      i32.add                 ;; 42 + 10 = 52
    )
  )
  
  ;; Function to test br with condition (true)
  (func (export "test_br_if_true") (result i32)
    (block (result i32)
      br 0 (i32.const 99)     ;; Branch with value 99
      i32.const 42           ;; Never executed
      i32.add                ;; Never executed
    )
  )
  
  ;; Function to test call
  (func (export "test_call") (result i32)
    i32.const 20
    i32.const 22
    call $helper_add         ;; 20 + 22 = 42
  )
  
  ;; Function to test return
  (func (export "test_return") (param i32) (result i32)
    local.get 0
    return                   ;; Return the parameter immediately
    i32.const 99             ;; Never executed
  )
  
  ;; Function to test nested control flow
  (func (export "test_nested") (result i32)
    (block (result i32)
      (if (result i32)
        (i32.const 1)        ;; condition (true)
        (then 
          (block (result i32)
            i32.const 10
            (if 
              (i32.const 1)  ;; condition (true)
              (then 
                i32.const 20
                i32.add      ;; 10 + 20 = 30
              )
            )
          )
        )
        (else 
          i32.const 50
        )
      )
    )
  )
)