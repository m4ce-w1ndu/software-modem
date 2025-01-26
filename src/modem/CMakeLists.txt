# Create modem library
add_library(modem STATIC
	"SoftwareModem.cpp"
	"SoftwareModem.h"
)

# Specify includes
target_include_directories(modem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add compile options
target_compile_options(modem PRIVATE -Wall -Wextra -pedantic)
