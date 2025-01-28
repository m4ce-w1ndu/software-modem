# Enable FetchContent
include(FetchContent)

# Fetch and build PortAudio
FetchContent_Declare(
		portaudio
		GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
		GIT_TAG        master # Use a specific release version
)

FetchContent_MakeAvailable(portaudio)

# Create modem library
add_library(modem STATIC
	"SoftwareModem.cpp"
	"SoftwareModem.h"
	"ModemAudioPlayer.cpp"
	"ModemAudioPlayer.h"
)

# Link to library
target_link_libraries(modem PRIVATE portaudio)

# Specify includes
target_include_directories(modem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add compile options
target_compile_options(modem PRIVATE -Wall -Wextra -pedantic)
