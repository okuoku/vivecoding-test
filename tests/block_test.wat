(module
  (func $block_test (export "block_test") (param $x i32) (result i32)
    (local $result i32)
    block $my_block
      local.get $x
      i32.const 10
      i32.gt_s
      br_if $my_block
      i32.const 1
      local.set $result
      br $my_block
    end $my_block
    local.get $result
  )
)