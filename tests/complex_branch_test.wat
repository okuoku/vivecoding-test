(module
  (func $complex_branch_test (export "complex_branch_test") 
    (param $x i32) (param $y i32) (param $z i32) (result i32)
    (local $result i32)
    block $outer
      block $middle  
        block $inner
          local.get $x
          i32.const 0
          i32.eq
          br_if $inner
          local.get $x
          i32.const 1
          i32.eq
          br_if $middle
          local.get $x
          i32.const 2
          i32.eq
          br_if $outer
          local.get $y
          local.set $result
          br $inner
        end $inner
        local.get $z
        local.set $result
        br $middle
      end $middle
      i32.const 100
      local.set $result
    end $outer
    local.get $result
  )
)