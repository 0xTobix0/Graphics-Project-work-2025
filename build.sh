#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Run CMake to generate build files
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build the project
echo "Building project..."
make -j$(nproc)

# Run the executable if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Running the application..."
    ./GraphicsProject
else
    echo "Build failed! Check the errors above."
fi
