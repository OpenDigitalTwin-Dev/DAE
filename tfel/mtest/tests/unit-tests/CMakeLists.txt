if(CMAKE_CONFIGURATION_TYPES)
  foreach(conf ${CMAKE_CONFIGURATION_TYPES})
    if(HAVE_CASTEM)
      get_property(MFrontCastemBehavioursBuildPath
	TARGET MFrontCastemBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_CASTEM)
    if(HAVE_ASTER)
      get_property(MFrontAsterBehavioursBuildPath
	TARGET MFrontAsterBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_ASTER)
    if(HAVE_CYRANO)
      get_property(MFrontCyranoBehavioursBuildPath
	TARGET MFrontCyranoBehaviours PROPERTY LOCATION_${conf})
    endif(HAVE_CYRANO)
    configure_file("${PROJECT_SOURCE_DIR}/mtest/tests/unit-tests/MTestParserTest.cxx.in"
      ${PROJECT_BINARY_DIR}/mtest/tests/unit-tests/MTestParserTest_${conf}.cxx @ONLY)
    add_executable(MTestParserTest_${conf} EXCLUDE_FROM_ALL
      ${PROJECT_BINARY_DIR}/mtest/tests/unit-tests/MTestParserTest_${conf}.cxx)
    add_test(NAME MTestParserTest_${conf}
      COMMAND MTestParserTest_${conf}
      CONFIGURATIONS ${conf})
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST MTestParserTest_${conf}
    	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
    if(HAVE_CASTEM)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontCastemBehaviours)
    endif(HAVE_CASTEM)
    if(HAVE_ASTER)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontAsterBehaviours)
    endif(HAVE_ASTER)
    if(HAVE_CYRANO)
      set_property(TEST MTestParserTest_${conf} APPEND PROPERTY DEPENDS MFrontCyranoBehaviours)
    endif(HAVE_CYRANO)   
    target_link_libraries(MTestParserTest_${conf} TFELMTest
      MFrontLogStream TFELGlossary TFELSystem TFELTests TFELException)
    add_dependencies(check MTestParserTest_${conf})
  endforeach(conf ${CMAKE_CONFIGURATION_TYPES})
else(CMAKE_CONFIGURATION_TYPES)
  if(HAVE_CASTEM)
    get_property(MFrontCastemBehavioursBuildPath
      TARGET MFrontCastemBehaviours PROPERTY LOCATION)
  endif(HAVE_CASTEM)
  if(HAVE_ASTER)
    get_property(MFrontAsterBehavioursBuildPath
      TARGET MFrontAsterBehaviours PROPERTY LOCATION)
  endif(HAVE_ASTER)
  if(HAVE_CYRANO)
    get_property(MFrontCyranoBehavioursBuildPath
      TARGET MFrontCyranoBehaviours PROPERTY LOCATION)
  endif(HAVE_CYRANO)
  configure_file("${PROJECT_SOURCE_DIR}/mtest/tests/unit-tests/MTestParserTest.cxx.in"
    ${PROJECT_BINARY_DIR}/mtest/tests/unit-tests/MTestParserTest.cxx @ONLY)
  add_executable(MTestParserTest EXCLUDE_FROM_ALL
    ${PROJECT_BINARY_DIR}/mtest/tests/unit-tests/MTestParserTest.cxx)
  add_test(NAME MTestParserTest COMMAND MTestParserTest)
  target_link_libraries(MTestParserTest TFELMTest
    MFrontLogStream TFELGlossary TFELSystem TFELTests TFELException)
  if(HAVE_CASTEM)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontCastemBehaviours)
  endif(HAVE_CASTEM)
  if(HAVE_ASTER)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontAsterBehaviours)
  endif(HAVE_ASTER)
  if(HAVE_CYRANO)
    set_property(TEST MTestParserTest APPEND PROPERTY DEPENDS MFrontCyranoBehaviours)
  endif(HAVE_CYRANO)
  add_dependencies(check MTestParserTest)
endif(CMAKE_CONFIGURATION_TYPES)

macro(test_mtest test_arg)
  add_executable(${test_arg} EXCLUDE_FROM_ALL ${test_arg}.cxx)
  add_test(NAME ${test_arg} COMMAND ${test_arg})
  add_dependencies(check ${test_arg})
    if((CMAKE_HOST_WIN32) AND (NOT MSYS))
      set_property(TEST ${test_arg}
    	PROPERTY ENVIRONMENT "PATH=$<TARGET_FILE_DIR:TFELMTest>\;$<TARGET_FILE_DIR:TFELMFront>\;$<TARGET_FILE_DIR:MFrontLogStream>\;$<TARGET_FILE_DIR:TFELMaterial>\;$<TARGET_FILE_DIR:TFELNUMODIS>\;$<TARGET_FILE_DIR:TFELMathParser>\;$<TARGET_FILE_DIR:TFELGlossary>\;$<TARGET_FILE_DIR:TFELSystem>\;$<TARGET_FILE_DIR:TFELUtilities>\;$<TARGET_FILE_DIR:TFELException>\;$<TARGET_FILE_DIR:TFELTests>\;$<TARGET_FILE_DIR:TFELUnicodeSupport>\;$<TARGET_FILE_DIR:TFELConfig>\;$ENV{PATH}")
    endif((CMAKE_HOST_WIN32) AND (NOT MSYS))
  target_link_libraries(${test_arg}
    TFELMTest      TFELMaterial
    TFELMathParser TFELMath
    TFELSystem     TFELGlossary
    TFELUtilities  TFELException TFELTests)
endmacro(test_mtest)

test_mtest(PipeTest)
test_mtest(EvolutionTest)
test_mtest(GasEquationOfStateTest)
