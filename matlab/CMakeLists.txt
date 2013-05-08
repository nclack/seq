find_package(matlab CONFIG PATHS cmake)
include(cmake/MatlabMexMacro.cmake)

if(MATLAB_FOUND)

  #
  # mex targets
  #
  foreach(name SeqLength
               SeqRead
             )
    set(tgt mex${name})
    add_mex_file(${tgt} src/${name}.mex.c)
    target_link_libraries(${tgt} seq)
    install(TARGETS ${tgt}
            DESTINATION matlab/@Seq/private)
    endforeach()

  #
  # install the matlab sources
  #
  file(GLOB sources *.m)
  install(FILES ${sources} DESTINATION matlab)
  file(GLOB sources @Seq/*.m)
  install(FILES ${sources} DESTINATION matlab/@Seq)
  file(GLOB sources @Seq/private/*.m)
  install(FILES ${sources} DESTINATION matlab/@Seq/private)
endif()