execute_process(COMMAND git rev-parse HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  OUTPUT_VARIABLE git_hash OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git diff-index --quiet HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  RESULT_VARIABLE  git_dirty)
file(WRITE git.h "#define GIT_HEAD_HASH \"${git_hash}\"\n#define GIT_DIRTY \"${git_dirty}\"")