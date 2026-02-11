(module
  (func $if_simple (export "if_simple") (param i32) (result i32)
    local.get 0
    if (result i32)
      i32.const 42
    else
      i32.const 24
    end
  )
)