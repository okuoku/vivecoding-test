(module
  (func $loop_simple (export "loop_simple") (param i32) (result i32)
    local.get 0
    i32.const 0
    i32.const 10
    loop (result i32)
      local.get 0
      i32.add
      i32.const 1
      i32.add
      br 0
    end
  )
)