find_package(Python 3 COMPONENTS Interpreter)

set(myroot ${CMAKE_CURRENT_LIST_DIR})
set(projroot ${CMAKE_CURRENT_LIST_DIR}/__dummy)


execute_process(COMMAND
    ${Python_EXECUTABLE} ${myroot}/analyze.py
    INPUT_FILE ${projroot}/compile_commands.json
    OUTPUT_FILE rs_project_cmds.jsonl
)

execute_process(COMMAND
    ${Python_EXECUTABLE} ${myroot}/inject_preprocess.py
    -b check
    INPUT_FILE rs_project_cmds.jsonl
    OUTPUT_FILE rs_project_preprocess.jsonl
)

execute_process(COMMAND
    ${Python_EXECUTABLE} ${myroot}/exec_project.py
    INPUT_FILE rs_project_preprocess.jsonl
)

execute_process(COMMAND
    ${Python_EXECUTABLE} ${myroot}/analyze_include_tree.py
    INPUT_FILE rs_project_preprocess.jsonl
)
