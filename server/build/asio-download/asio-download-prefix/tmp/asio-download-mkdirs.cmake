# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/kai/source/repos/server/server/deps/sleepy-discord/deps/asio"
  "C:/Users/kai/source/repos/server/server/build/asio-build"
  "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix"
  "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix/tmp"
  "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix/src/asio-download-stamp"
  "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix/src"
  "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix/src/asio-download-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
  file(MAKE_DIRECTORY "C:/Users/kai/source/repos/server/server/build/asio-download/asio-download-prefix/src/asio-download-stamp/${subDir}")
endforeach()
