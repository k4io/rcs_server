
if(NOT "C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitinfo.txt" IS_NEWER_THAN "C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: 'C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/cpr"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: 'C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/cpr'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  clone --no-checkout "https://github.com/whoshuu/cpr.git" "cpr"
    WORKING_DIRECTORY "C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/whoshuu/cpr.git'")
endif()

execute_process(
  COMMAND "C:/Program Files/Git/cmd/git.exe"  checkout b748095 --
  WORKING_DIRECTORY "C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/cpr"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'b748095'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "C:/Program Files/Git/cmd/git.exe"  submodule update --recursive --init 
    WORKING_DIRECTORY "C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/cpr"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: 'C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/cpr'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitinfo.txt"
    "C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: 'C:/Users/kai/source/repos/server/server/out/build/x64-Debug (default)/cpr-download/cpr-download-prefix/src/cpr-download-stamp/cpr-download-gitclone-lastrun.txt'")
endif()

