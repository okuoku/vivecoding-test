(module
  (func $return_value (export "return_value") (param i32) (result i32)
    local.get 0
    i32.const 10
    i32.add
    return
    i32.const 99
  )
)