# CMake generated Testfile for 
# Source directory: /Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun
# Build directory: /Users/amounra/Documents/Max 8/Packages/aumhaa/build/source/projects/aumhaa.shaderFun
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(aumhaa.shaderFun_test "/Users/amounra/Documents/Max 8/Packages/aumhaa/tests/aumhaa.shaderFun_test")
  set_tests_properties(aumhaa.shaderFun_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;66;add_test;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;0;;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;39;include;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(aumhaa.shaderFun_test "/Users/amounra/Documents/Max 8/Packages/aumhaa/tests/aumhaa.shaderFun_test")
  set_tests_properties(aumhaa.shaderFun_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;66;add_test;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;0;;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;39;include;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(aumhaa.shaderFun_test "/Users/amounra/Documents/Max 8/Packages/aumhaa/tests/MinSizeRel/aumhaa.shaderFun_test")
  set_tests_properties(aumhaa.shaderFun_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;66;add_test;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;0;;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;39;include;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(aumhaa.shaderFun_test "/Users/amounra/Documents/Max 8/Packages/aumhaa/tests/RelWithDebInfo/aumhaa.shaderFun_test")
  set_tests_properties(aumhaa.shaderFun_test PROPERTIES  _BACKTRACE_TRIPLES "/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;66;add_test;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/min-api/test/min-object-unittest.cmake;0;;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;39;include;/Users/amounra/Documents/Max 8/Packages/aumhaa/source/projects/aumhaa.shaderFun/CMakeLists.txt;0;")
else()
  add_test(aumhaa.shaderFun_test NOT_AVAILABLE)
endif()