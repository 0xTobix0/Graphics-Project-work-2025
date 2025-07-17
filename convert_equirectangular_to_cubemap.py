import cv2
import numpy as np
import os
import OpenEXR
import Imath
from PIL import Image

def read_exr(exr_path):
    """Read an EXR file and return a numpy array."""
    if not os.path.isfile(exr_path):
        raise FileNotFoundError(f"File not found: {exr_path}")
    
    # Read EXR file
    exr_file = OpenEXR.InputFile(exr_path)
    
    # Get the data window information
    dw = exr_file.header()['dataWindow']
    width = dw.max.x - dw.min.x + 1
    height = dw.max.y - dw.min.y + 1
    
    # Read the three color channels
    FLOAT = Imath.PixelType(Imath.PixelType.FLOAT)
    (r, g, b) = [np.frombuffer(exr_file.channel(c, FLOAT), dtype=np.float32) 
                 for c in "RGB"]
    
    # Reshape and combine channels
    img = np.zeros((height, width, 3), dtype=np.float32)
    img[:,:,0] = r.reshape(height, width)
    img[:,:,1] = g.reshape(height, width)
    img[:,:,2] = b.reshape(height, width)
    
    # Convert to 8-bit for OpenCV processing
    img_8bit = (np.clip(img, 0, 1) * 255).astype(np.uint8)
    return img_8bit

def create_cubemap_faces(equirect_img, output_dir, size=1024):
    """Convert an equirectangular image to 6 cubemap faces."""
    # Define the 6 cube faces (pitch, yaw)
    faces = [
        ('right',   0, 90),    # +X (right)
        ('left',    0, -90),   # -X (left)
        ('top',    90, 0),     # +Y (top)
        ('bottom', -90, 0),    # -Y (bottom)
        ('front',   0, 0),     # +Z (front)
        ('back',    0, 180)    # -Z (back)
    ]
    
    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    
    # Process each face
    for name, pitch, yaw in faces:
        # Create rotation matrices
        pitch_rad = np.radians(pitch)
        yaw_rad = np.radians(yaw)
        
        # Create a grid of coordinates for the output image
        x = np.linspace(-1, 1, size)
        y = np.linspace(1, -1, size)
        xv, yv = np.meshgrid(x, y)
        
        # Convert to spherical coordinates
        xyz = np.dstack((xv, np.ones_like(xv), -yv))
        xyz = xyz / np.linalg.norm(xyz, axis=2, keepdims=True)
        
        # Apply rotation for this face
        rot_x = np.array([
            [1, 0, 0],
            [0, np.cos(pitch_rad), -np.sin(pitch_rad)],
            [0, np.sin(pitch_rad), np.cos(pitch_rad)]
        ])
        
        rot_y = np.array([
            [np.cos(yaw_rad), 0, np.sin(yaw_rad)],
            [0, 1, 0],
            [-np.sin(yaw_rad), 0, np.cos(yaw_rad)]
        ])
        
        # Rotate the coordinates
        xyz = np.dot(xyz, rot_x.T)
        xyz = np.dot(xyz, rot_y.T)
        
        # Convert to spherical coordinates
        phi = np.arctan2(xyz[..., 0], xyz[..., 2])
        theta = np.arcsin(xyz[..., 1])
        
        # Convert to UV coordinates for the equirectangular image
        u = (phi / (2 * np.pi) + 0.5) * equirect_img.shape[1]
        v = (0.5 - theta / np.pi) * equirect_img.shape[0]
        
        # Sample the equirectangular image
        map_x = u.astype(np.float32)
        map_y = v.astype(np.float32)
        
        # Use OpenCV's remap for the actual sampling
        face_img = cv2.remap(equirect_img, map_x, map_y, 
                           interpolation=cv2.INTER_LINEAR,
                           borderMode=cv2.BORDER_WRAP)
        
        # Save the face
        output_path = os.path.join(output_dir, f"{name}.jpg")
        cv2.imwrite(output_path, face_img)
        print(f"Saved {output_path}")

if __name__ == "__main__":
    # Input and output paths
    input_exr = "textures/charolettenbrunn_park_4k.exr"
    output_dir = "textures/skybox"
    
    try:
        print(f"Reading EXR file: {input_exr}")
        equirect_img = read_exr(input_exr)
        print(f"Converting to cubemap faces in directory: {output_dir}")
        create_cubemap_faces(equirect_img, output_dir, size=2048)
        print("Conversion complete!")
    except Exception as e:
        print(f"Error: {str(e)}")
