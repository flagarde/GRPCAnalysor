SET_OPTION(CXX_STANDARD 17) # C++14...
SET_OPTION(CXX_STANDARD_REQUIRED ON) #...is required...
SET_OPTION(CXX_EXTENSIONS ON) #...without compiler extensions like gnu++11

# add the binary tree to the search path for include files
include_directories("${PROJECT_INCLUDE_DIR}")
add_subdirectory(Streamout)
add_subdirectory(LCIOOutputProcessor)
add_subdirectory(Progress)
#add_subdirectory(TriventBIF)
add_subdirectory(Trivent)
#add_subdirectory(SpillStudy)
add_subdirectory(IJKfiller)
add_subdirectory(XYZfiller)
#add_subdirectory(Clusterisation)
add_subdirectory(BifHelper)
add_subdirectory(TimeSelecter)
add_subdirectory(AEGIS)
#add_subdirectory(Analysis2)
#add_subdirectory(Trivent2)
