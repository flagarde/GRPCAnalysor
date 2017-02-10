# add the binary tree to the search path for include files
file(COPY ./ DESTINATION ${PROJECT_INCLUDE_DIR})
configure_file ("Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")
