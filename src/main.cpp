#define GL_SILENCE_DEPRECATION
#include "../external/glad-3.3/include/glad/gl.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <string>

// Include standard headers
#include <iostream>
#include <memory>
#include <vector>

// Include GLAD first to ensure OpenGL headers are included in the correct order
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Include project headers
#include "shader.h"
#include "shader_fwd.h"
#include "shader_manager.h"
#include "skybox.h"
#include "butterfly.h"
#include "text_renderer.h"

// FPS counter variables
float fps = 0.0f;
int frameCount = 0;
float lastFpsUpdate = 0.0f;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

// Shaders - managed by shader_manager.h
extern ShaderPtr ourShader;
extern ShaderPtr skyboxShader;
extern ShaderPtr lightShader;

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Luminous Field", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Load OpenGL function pointers with GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);
    
    // Initialize shaders using the shader manager
    InitializeShaderManager();
    
    // Debug shader loading
    if (!butterflyShader) {
        std::cerr << "ERROR: Butterfly shader failed to load!" << std::endl;
    } else {
        std::cout << "Butterfly shader loaded successfully (ID: " << butterflyShader->ID << ")" << std::endl;
    }
    
    // Initialize text renderer with larger font size for better visibility
    TextRenderer textRenderer(SCR_WIDTH, SCR_HEIGHT);
    if (!textRenderer.Load("fonts/Roboto-Regular.ttf", 32)) {
        std::cerr << "Failed to load Roboto font for text rendering" << std::endl;
        // Try system font as fallback
        if (!textRenderer.Load("/System/Library/Fonts/Supplemental/Arial.ttf", 32)) {
            std::cerr << "Failed to load fallback Arial font" << std::endl;
            // Try another common system font
            if (!textRenderer.Load("/System/Library/Fonts/SFNS.ttf", 32)) {
                std::cerr << "Failed to load any font for text rendering" << std::endl;
            } else {
                std::cout << "Successfully loaded SFNS system font" << std::endl;
            }
        } else {
            std::cout << "Successfully loaded Arial system font" << std::endl;
        }
    } else {
        std::cout << "Successfully loaded Roboto font" << std::endl;
    }
    
    // Create butterfly with OBJ model - use path to the new model
    std::string butterflyModelPath = "/Users/namangupta/Downloads/new butterfly/source/Bake/Matiposa_001.obj";
    std::cout << "Loading butterfly model from: " << butterflyModelPath << std::endl;
    std::vector<std::unique_ptr<Butterfly>> butterflies;
    
    // Create a single butterfly for now
    Butterfly* butterfly = new Butterfly(*butterflyShader, butterflyModelPath);
    
    // Position the butterfly in front of the camera
    butterfly->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    
    // Set a reasonable scale
    butterfly->SetScale(0.1f);
    
    // Add to the vector
    butterflies.emplace_back(butterfly);
    
    // Print debug info
    std::cout << "Butterfly created at position: (" 
              << butterfly->GetPosition().x << ", " 
              << butterfly->GetPosition().y << ", " 
              << butterfly->GetPosition().z << ")" << std::endl;
    
    // Load skybox
    std::vector<std::string> faces = {
        "textures/skybox_cubemap/right.png",
        "textures/skybox_cubemap/left.png",
        "textures/skybox_cubemap/top.png",
        "textures/skybox_cubemap/bottom.png",
        "textures/skybox_cubemap/front.png",
        "textures/skybox_cubemap/back.png"
    };
    Skybox skybox(faces, *skyboxShader);
    
    // Simple triangle vertices (x, y, z, r, g, b)
    float triangle[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left - red
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right - green
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // top - blue
    };
    
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Set up cube VAO (keeping this for future use)
    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Update butterfly positions (they were already created earlier)
    for (size_t i = 0; i < butterflies.size(); ++i) {
        // Position butterflies in different locations
        float angle = (float)i / butterflies.size() * 2.0f * 3.14159f;
        float radius = 3.0f + (i * 2.0f);
        float x = sin(angle) * radius;
        float z = cos(angle) * radius;
        
        butterflies[i]->SetPosition(glm::vec3(x, 1.5f, z));
        butterflies[i]->SetScale(0.005f);
    }
    
    // For timing
    float lastFrame = 0.0f;
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Initialize text renderer
    // Text renderer is already initialized above
    
    // FPS counter variables
    float lastFpsUpdate = 0.0f;
    int frameCount = 0;
    float fps = 0.0f;
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Input
        processInput(window);
        
        // Render
        // ------
        // Calculate FPS
        frameCount++;
        if (currentFrame - lastFpsUpdate >= 1.0f) {
            fps = frameCount / (currentFrame - lastFpsUpdate);
            frameCount = 0;
            lastFpsUpdate = currentFrame;
            
            // Update window title with FPS
            std::string title = "Butterfly Scene - " + std::to_string(static_cast<int>(fps)) + " FPS";
            glfwSetWindowTitle(window, title.c_str());
        }
        
        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Calculate projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // Update and draw butterflies
        glEnable(GL_DEPTH_TEST);
        for (size_t i = 0; i < butterflies.size(); ++i) {
            auto& butterfly = butterflies[i];
            if (butterfly) {
                // Update butterfly state
                butterfly->Update(deltaTime);
                
                // Get position for debugging
                glm::vec3 pos = butterfly->GetPosition();
                
                // Only print position occasionally to reduce console spam
                static int frameCount = 0;
                if (frameCount++ % 60 == 0) {  // Print every 60 frames
                    std::cout << "Drawing butterfly " << i << " at (" 
                              << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
                }
                
                // Draw the butterfly
                if (butterflies.size() > 0) {
                    butterflies[0]->Draw(view, projection);
                }
                
                // Check for OpenGL errors after drawing
                GLenum err;
                while ((err = glGetError()) != GL_NO_ERROR) {
                    std::cerr << "OpenGL error after drawing butterfly " << i << ": " << err << std::endl;
                }
            }
        }
        
        // Draw skybox with depth testing but depth writing disabled
        glDepthMask(GL_FALSE);  // Disable writing to depth buffer
        skyboxShader->use();
        
        // Remove translation from the view matrix for the skybox
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
        skyboxShader->setMat4("view", skyboxView);
        skyboxShader->setMat4("projection", projection);
        
        // Draw skybox (only once)
        skybox.Draw(skyboxView, projection);
        
        // Restore depth writing
        glDepthMask(GL_TRUE);
        
        // Draw FPS counter with background for better visibility
        std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
        
        // Draw semi-transparent background for better text visibility
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Draw background rectangle
        float textWidth = fpsText.length() * 15.0f; // Approximate width
        float textHeight = 30.0f;
        float margin = 10.0f;
        
        // Draw the text with a shadow for better visibility
        textRenderer.RenderText(fpsText, 20.0f, 40.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f)); // Shadow
        textRenderer.RenderText(fpsText, 18.0f, 38.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f)); // Main text
        
        // Re-enable depth testing for 3D rendering
        glEnable(GL_DEPTH_TEST);
        
        // Make sure to use the main shader for other objects
        ourShader->use();
        
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup shaders using the shader manager
    cleanupShaders();
    
    // Cleanup OpenGL resources
    
    // Cleanup window
    glfwDestroyWindow(window);
    
    // Terminate GLFW
    glfwTerminate();
    return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// GLFW: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// GLFW: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// GLFW: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}

// GLFW: handle keyboard input events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
