#[===================================================================[
   read version from source
#]===================================================================]

file (STRINGS src/ripple/protocol/impl/BuildInfo.cpp BUILD_INFO)
foreach (line_ ${BUILD_INFO})
  if (line_ MATCHES "versionString[ ]*=[ ]*\"(.+)\"")
    set (brtd_version ${CMAKE_MATCH_1})
  endif ()
endforeach ()
if (brtd_version)
  message (STATUS "brtd version: ${brtd_version}")
else ()
  message (FATAL_ERROR "unable to determine brtd version")
endif ()
