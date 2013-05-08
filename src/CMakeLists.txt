find_package(matlab CONFIG PATHS cmake)
include(cmake/MatlabMexMacro.cmake)

if(MATLAB_FOUND)

  #
  # mex targets
  #
  foreach(name tps_free 
               tps_fit 
               tps_make 
               tps_update
               tps_eval
               tps_energy)
    set(tgt ${name}_mex)
    add_mex_file(${tgt} src/${name}.mex.c)
    target_link_libraries(${tgt} tps)
    install(TARGETS ${tgt}
            DESTINATION matlab/@ThinPlateSpline/private)
    endforeach()

  #
  # install the matlab sources
  #
  file(GLOB sources *.m)
  install(FILES ${sources} DESTINATION matlab)
  file(GLOB sources @ThinPlateSpline/*.m)
  install(FILES ${sources} DESTINATION matlab/@ThinPlateSpline)
  file(GLOB sources @ThinPlateSpline/private/*.m)
  install(FILES ${sources} DESTINATION matlab/@ThinPlateSpline/private)
endif()