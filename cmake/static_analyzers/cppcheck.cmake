function(enable_cppcheck WARNINGS_AS_ERRORS CPPCHECK_OPTIONS)

  find_program(CPPCHECK cppcheck REQUIRED)
  
  if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
    set(CPPCHECK_TEMPLATE "vs")
  else()
    set(CPPCHECK_TEMPLATE "gcc")
  endif()

  if("${CPPCHECK_OPTIONS}" STREQUAL "")
    # Enable all warnings that are actionable by the user of this toolset
    # style should enable the other 3, but we'll be explicit just in case
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK}
        --template=${CPPCHECK_TEMPLATE}
        --enable=style,performance,warning,portability
        --inline-suppr
        # We cannot act on a bug/missing feature of cppcheck
        --suppress=cppcheckError
        --suppress=internalAstErrorasd
        # if a file does not have an internalAstError, we get an unmatchedSuppression error
        --suppress=unmatchedSuppression
        # noisy and incorrect sometimes
        --suppress=passedByValue
        # ignores code that cppcheck thinks is invalid C++
        --suppress=syntaxError
        --suppress=preprocessorErrorDirective
        --inconclusive
        --error-exitcode=2)
  else()
    # if the user provides a CPPCHECK_OPTIONS with a template specified, it will override this template
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --template=${CPPCHECK_TEMPLATE} ${CPPCHECK_OPTIONS})
  endif()

  if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
  endif()
endfunction()