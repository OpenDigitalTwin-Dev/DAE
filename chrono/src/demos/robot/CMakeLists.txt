# Demo programs for simulations with models from the Chrono::Robots library
# ------------------------------------------------------------------------------

message(STATUS "Demo programs for ROBOT models...")

if(NOT ENABLE_MODULE_VEHICLE)
  message(STATUS "  Warning: the ROBOT demos require Chrono::Vehicle!")
  return()
endif()

# ------------------------------------------------------------------------------

set(DEMOS "")

if(ENABLE_MODULE_IRRLICHT OR ENABLE_MODULE_VSG)
  set(DEMOS ${DEMOS}
      demo_ROBOT_Curiosity_Rigid
      demo_ROBOT_Curiosity_SCM    
      demo_ROBOT_Turtlebot_Rigid
      demo_ROBOT_Viper_Rigid
      demo_ROBOT_Viper_SCM
  )

  if(ENABLE_MODULE_SENSOR)
    set(DEMOS ${DEMOS}
        demo_ROBOT_Curiosity_SCM_Sensor
        demo_ROBOT_Viper_SCM_Sensor
    )
  endif()
endif()

if(ENABLE_MODULE_IRRLICHT)
  set(DEMOS ${DEMOS}
      demo_ROBOT_LittleHexy
  )
endif()

if(ENABLE_MODULE_VSG OR ENABLE_MODULE_OPENGL)
  if(ENABLE_MODULE_FSI)
    set(DEMOS ${DEMOS}
        demo_ROBOT_Viper_SPH
    )
  endif()
endif()

# ------------------------------------------------------------------------------
# Include directories, compiler flags, libraries

set(COMPILER_FLAGS "${CH_CXX_FLAGS}")
set(LINKER_FLAGS "${CH_LINKERFLAG_EXE}")
list(APPEND LIBS "ChronoEngine")
list(APPEND LIBS "ChronoEngine_vehicle")
list(APPEND LIBS "ChronoModels_robot")

if(ENABLE_MODULE_IRRLICHT)
    include_directories(${CH_IRRLICHT_INCLUDES})
    set(COMPILER_FLAGS "${COMPILER_FLAGS} ${CH_IRRLICHT_CXX_FLAGS}")
    list(APPEND LIBS "ChronoEngine_irrlicht")
    list(APPEND LIBS "ChronoEngine_vehicle_irrlicht")
endif()

if(ENABLE_MODULE_VSG)
    include_directories(${CH_VSG_INCLUDES})
    list(APPEND LIBS "ChronoEngine_vsg")
    list(APPEND LIBS "ChronoEngine_vehicle_vsg")
endif()

if(ENABLE_MODULE_OPENGL)
    include_directories(${CH_OPENGL_INCLUDES})
    list(APPEND LIBS "ChronoEngine_opengl")
endif()

if(ENABLE_MODULE_SENSOR)
  include_directories(${CH_SENSOR_INCLUDES})
  list(APPEND LIBS "ChronoEngine_sensor")
endif()

if(ENABLE_MODULE_FSI)
  include_directories(${CH_FSI_INCLUDES})
  list(APPEND LIBS "ChronoEngine_fsi")
endif()

#--------------------------------------------------------------
# OS-specific settings

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CH_LINKERFLAG_EXE  "${CH_LINKERFLAG_EXE} -framework IOKit -framework Cocoa -framework OpenGL")
endif()

# ------------------------------------------------------------------------------
# Add all executables
# ------------------------------------------------------------------------------

foreach(PROGRAM ${DEMOS})
    message(STATUS "...add ${PROGRAM}")

    add_executable(${PROGRAM} ${PROGRAM}.cpp)
    source_group("" FILES ${PROGRAM}.cpp)

    set_target_properties(${PROGRAM} PROPERTIES COMPILE_FLAGS "${COMPILER_FLAGS}" LINK_FLAGS "${LINKER_FLAGS}")
    if(APPLE)
      set_target_properties(${PROGRAM} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${MACOSX_BUNDLE_INFO_PLIST})
    endif()
    set_property(TARGET ${PROGRAM} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROGRAM}>")
    target_link_libraries(${PROGRAM} ${LIBS})

    install(TARGETS ${PROGRAM} DESTINATION ${CH_INSTALL_DEMO})
endforeach()

# ------------------------------------------------------------------------------
# Add subdirectories

add_subdirectory(robosimian)
