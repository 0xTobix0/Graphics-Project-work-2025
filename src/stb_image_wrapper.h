#ifndef STB_IMAGE_WRAPPER_H
#define STB_IMAGE_WRAPPER_H

// Only define STB_IMAGE_IMPLEMENTATION in one source file
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#define STBI_FAILURE_USERMSG
#endif

// Include the STB image header from the stb directory
#include "../external/stb/stb_image.h"

// Function to get STBI error message
static inline const char* stbi_get_error_message() {
    return stbi_failure_reason();
}

#endif // STB_IMAGE_WRAPPER_H
