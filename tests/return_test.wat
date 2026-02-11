(module
  (func $return_test (export "return_test") (param $x i32) (param $y i32) (result i32)
    local.get $x
    i32.const 0
    i32.eq
    if
      i32.const -1
      return
    end
    local.get $x
    local.get $y
    i32.add
  )
)