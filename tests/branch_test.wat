(module
  (func $branch_test (export "branch_test") (param $x i32) (result i32)
    (local $result i32)
    block $outer
      block $inner
        local.get $x
        i32.const 0
        i32.eq
        br_if $inner
        local.get $x
        i32.const 1
        i32.eq
        br_if $outer
        local.get $x
        local.set $result
        br $outer
      end $inner
      i32.const 42
      local.set $result
    end $outer
    local.get $result
  )
)