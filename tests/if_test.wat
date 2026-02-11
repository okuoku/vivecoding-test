(module
  (func $if_test (export "if_test") (param $cond i32) (param $then_val i32) (param $else_val i32) (result i32)
    local.get $cond
    if (result i32)
      local.get $then_val
    else
      local.get $else_val
    end
  )
)