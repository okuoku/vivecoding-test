(module
  (func $branch_simple (export "branch_simple") (param i32) (result i32)
    block (result i32)
      local.get 0
      i32.const 5
      i32.eq
      br_if 0
      i32.const 10
    end
  )
)