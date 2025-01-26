# Create test executable
add_executable(modem_example
	"main.cpp"
)

# Link library
target_link_libraries(modem_example PRIVATE modem)

# Compile options
target_compile_options(modem_example PRIVATE -Wall -Wextra -pedantic)
