#include <OpenGLPrj.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

const std::string program_name = ("Camera");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
float xx=0.0f;
float yy=3.0f;
float zz=0.0f;
static glm::vec3 cameraPos = glm::vec3(xx, yy, zz);
//static glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

static bool firstMouse = true;

// yaw is initialized to -90.0 degrees since a yaw of 0.0
// results in a direction vector pointing to the right so we
// initially rotate a bit to the left.
static float yaw = -90.0f;

static float pitch = 0.0f;
static float lastX = 800.0f / 2.0f;
static float lastY = 600.0f / 2.0f;
static float fov = 45.0f;

// timing
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          program_name.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    // ------------------------------------
    std::string shader_location("../res/shaders/");

    std::string used_shaders("shader");

    Shader ourShader(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //cube vertices
    float vertices[] = {
            //x      y      z       texture
            -5.0f, 0.0f, 5.0f,   0.0f, 0.0f,
            5.0f,  0.0f, 5.0f,   1.0f, 0.0f,
            5.0f,  0.0f, -5.0f,  1.0f, 1.0f,

            5.0f,  0.0f, -5.0f,  1.0f, 1.0f,
            -5.0f, 0.0f, -5.0f,  0.0f, 1.0f,
            -5.0f, 0.0f, 5.0f,   0.0f, 0.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f), //center
            //straight (5)
            glm::vec3(0.0f, 0.0f, -10.0f),
            glm::vec3(0.0f, 0.0f, -20.0f),
            glm::vec3(0.0f, 0.0f, -30.0f),
            glm::vec3(0.0f, 0.0f, -40.0f),
            glm::vec3(0.0f, 0.0f, -50.0f),
            //turn left (3)
            glm::vec3(-10.0f, 0.0f, -50.0f),
            glm::vec3(-20.0f, 0.0f, -50.0f),
            glm::vec3(-30.0f, 0.0f, -50.0f),
            //turn left again (2)
            glm::vec3(-30.0f, 0.0f, -40.0f),
            glm::vec3(-30.0f, 0.0f, -30.0f),
            //turn right (3)
            glm::vec3(-40.0f, 0.0f, -30.0f),
            glm::vec3(-50.0f, 0.0f, -30.0f),
            glm::vec3(-60.0f, 0.0f, -30.0f),
            //turn left (2)
            glm::vec3(-60.0f, 0.0f, -20.0f),
            glm::vec3(-60.0f, 0.0f, -10.0f),
            //turn right (2)
            glm::vec3(-70.0f, 0.0f, -10.0f),
            glm::vec3(-80.0f, 0.0f, -10.0f),
            //turn left (2)
            glm::vec3(-80.0f, 0.0f, 0.0f),
            glm::vec3(-80.0f, 0.0f, 10.0f),
            //turn left again (4)
            glm::vec3(-70.0f, 0.0f, 10.0f),
            glm::vec3(-60.0f, 0.0f, 10.0f),
            glm::vec3(-50.0f, 0.0f, 10.0f),
            glm::vec3(-40.0f, 0.0f, 10.0f),
            //turn right (3)
            glm::vec3(-40.0f, 0.0f, 20.0f),
            glm::vec3(-40.0f, 0.0f, 30.0f),
            glm::vec3(-40.0f, 0.0f, 40.0f),
            //turn left (2)
            glm::vec3(-30.0f, 0.0f, 40.0f),
            glm::vec3(-20.0f, 0.0f, 40.0f),
            //turn left (2)
            glm::vec3(-20.0f, 0.0f, 30.0f),
            glm::vec3(-20.0f, 0.0f, 20.0f),
            //turn right (3)
            glm::vec3(-10.0f, 0.0f, 20.0f),
            glm::vec3(0.0f, 0.0f, 20.0f),
            glm::vec3(10.0f, 0.0f, 20.0f)
            //34 in total
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
            GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(
            true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can
    // find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("../res/textures/container.jpg", &width,
                                    &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    // ---------
//  glGenTextures(1, &texture2);
//  glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
//  glTexParameteri(
//      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
//      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
//  data = stbi_load("../res/textures/awesomeface.png", &width, &height,
//                   &nrChannels, 0);
//  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel,
    // so make sure to tell OpenGL the data type is of GL_RGBA
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
//                 GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//  } else {
//    std::cout << "Failed to load texture" << std::endl;
//  }
//  stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has
    // to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
//  ourShader.setInt("texture2", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change
        // every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov),
                                                static_cast<float>(SCR_WIDTH) /
                                                static_cast<float>(SCR_HEIGHT),
                                                0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        // lookAt(position, target, up vector)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        //jump and crouch
        glm::mat4 transform = glm::mat4(1.0f);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            transform = glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            transform = glm::translate(transform, glm::vec3(0.0f, -1.0f, 0.0f));
        }
        // render container
        ourShader.use();

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 35; i++) {
            // calculate the model matrix for each object and pass it to shader before
            // drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 1.0f));
//      float angle = 30.0f;
//      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.0f, -1.0f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
//
//
    float cameraSpeed = 0.7f * deltaTime;
    for (int i=0; i<15.0f; i++) {
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }


//    //koga se kliknuva space se dvizi cameraPos nagore pa nadole po y oskata, drug nacin za resavanje?
//    float limit = 7.0f;
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//        transform = glm::translate(transform, glm::vec3(0.0f, yy, 0.0f));
//        if (yy < limit) {
//            cameraPos += cameraSpeed * cameraUp;
//            yy=limit;
//        }
//        if (yy > limit) {
//            cameraPos -= cameraSpeed * cameraUp;
//            yy = 0.0f;
//        }
//
//    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
    float xpos = static_cast<float>(xposd);
    float ypos = static_cast<float>(yposd);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    //constraint values for camera movements along the pitch
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    //no need for zoom
//  if (fov >= 1.0f && fov <= 45.0f)
//    fov -= static_cast<float>(yoffset);
//  if (fov <= 1.0f)
//    fov = 1.0f;
//  if (fov >= 45.0f)
//    fov = 45.0f;
}
