(module
  (func $multi_if_test (export "multi_if_test") 
    (param $cond1 i32) (param $cond2 i32) (param $val1 i32) (param $val2 i32) (param $val3 i32) (result i32)
    local.get $cond1
    if (result i32)
      local.get $cond2
      if (result i32)
        local.get $val1
      else
        local.get $val2
      end
    else
      local.get $val3
    end
  )
)