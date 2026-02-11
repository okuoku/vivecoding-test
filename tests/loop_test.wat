(module
  (func $loop_test (export "loop_test") (param $n i32) (result i32)
    (local $sum i32)
    i32.const 0
    local.set $sum
    loop $my_loop
      local.get $sum
      local.get $n
      i32.add
      local.set $sum
      local.get $n
      i32.const -1
      i32.add
      local.tee $n
      br_if $my_loop
    end
    local.get $sum
  )
)