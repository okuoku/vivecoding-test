(module
  (func $block_simple (export "block_simple") (param i32) (result i32)
    block (result i32)
      local.get 0
      br 0
      i32.const 99
    end
  )
)