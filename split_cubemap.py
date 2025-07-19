#!/usr/bin/env python3
from PIL import Image
import os
import sys

def split_cubemap(input_path, output_dir):
    """
    Split a cubemap image into 6 individual face images.
    Assumes a 4x3 grid layout where the faces are arranged as:
    
        [ ][+X][ ][ ]
        [-X][+Z][+X][-Z]
        [ ][-X][ ][ ]
        [ ][+Y][ ][ ]
    """
    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    
    # Open the cubemap image
    try:
        cubemap = Image.open(input_path)
        width, height = cubemap.size
        face_size = width // 4  # Assuming 4 faces per row
        
        # Define the regions for each face in the grid
        # (left, upper, right, lower)
        face_regions = {
            'right': (2 * face_size, 1 * face_size, 3 * face_size, 2 * face_size),  # +X
            'left': (0 * face_size, 1 * face_size, 1 * face_size, 2 * face_size),   # -X
            'top': (1 * face_size, 0 * face_size, 2 * face_size, 1 * face_size),    # +Y
            'bottom': (1 * face_size, 2 * face_size, 2 * face_size, 3 * face_size), # -Y
            'front': (1 * face_size, 1 * face_size, 2 * face_size, 2 * face_size),  # +Z
            'back': (3 * face_size, 1 * face_size, 4 * face_size, 2 * face_size)    # -Z
        }
        
        # Crop and save each face
        for face_name, region in face_regions.items():
            face = cubemap.crop(region)
            output_path = os.path.join(output_dir, f"{face_name}.png")
            face.save(output_path, "PNG")  # Save as PNG to preserve quality
            print(f"Saved {face_name} face to {output_path}")
            
        return True
        
    except Exception as e:
        print(f"Error processing image: {e}")
        return False

def main():
    if len(sys.argv) != 3:
        print("Usage: python split_cubemap.py <input_image> <output_directory>")
        sys.exit(1)
        
    input_path = sys.argv[1]
    output_dir = sys.argv[2]
    
    if not os.path.exists(input_path):
        print(f"Error: Input file '{input_path}' does not exist")
        sys.exit(1)
        
    success = split_cubemap(input_path, output_dir)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    # Input cubemap path
    cubemap_path = "textures/StandardCubeMap.png"
    # Output directory for individual faces
    output_dir = "textures/skybox_cubemap"
    
    # If command line arguments are provided, use those instead
    if len(sys.argv) >= 3:
        main()
    else:
        print(f"Splitting cubemap {cubemap_path} into individual faces...")
        success = split_cubemap(cubemap_path, output_dir)
        if success:
            print("Cubemap splitting complete!")
        else:
            print("Failed to split cubemap.")
            sys.exit(1)
