#include <array>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "shader.h"
#include <span>
#include <utility.h>
#include "my_glwindow.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


constexpr float SCR_WIDTH = 800;
constexpr float SCR_HEIGHT = 600;

constexpr auto float_size = sizeof(float);


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


bool gladInit()
{
    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    return  true;
}


int main(int argc, char* argv[])
{
        glWindow window("LearnOpenGL", SCR_WIDTH, SCR_HEIGHT);

        window.initWindow(4, 6);

        window.setWindowSizeCallback(framebuffer_size_callback);
        window.setMouseCallback(mouse_callback);
        window.setScrollCallback(scroll_callback);

        if (!gladInit()) {
            return -1;
        }

        // build and compile our shader zprogram
        // ------------------------------------
        Shader lightingShader("1colors.vs", "1colors.fs");
        Shader lightCubeShader("1light_cube.vs", "1light_cube.fs");

        constexpr std::array<float, 180> vertices = {
                -0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
                -0.5f,  0.5f, -0.5f, 
                -0.5f, -0.5f, -0.5f, 

                -0.5f, -0.5f,  0.5f, 
                0.5f, -0.5f,  0.5f,  
                0.5f,  0.5f,  0.5f,  
                0.5f,  0.5f,  0.5f,  
                -0.5f,  0.5f,  0.5f, 
                -0.5f, -0.5f,  0.5f, 

                -0.5f,  0.5f,  0.5f, 
                -0.5f,  0.5f, -0.5f, 
                -0.5f, -0.5f, -0.5f, 
                -0.5f, -0.5f, -0.5f, 
                -0.5f, -0.5f,  0.5f, 
                -0.5f,  0.5f,  0.5f, 

                0.5f,  0.5f,  0.5f,  
                0.5f,  0.5f, -0.5f,  
                0.5f, -0.5f, -0.5f,  
                0.5f, -0.5f, -0.5f,  
                0.5f, -0.5f,  0.5f,  
                0.5f,  0.5f,  0.5f,  

                -0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f,  
                0.5f, -0.5f,  0.5f,  
                0.5f, -0.5f,  0.5f,  
                -0.5f, -0.5f,  0.5f, 
                -0.5f, -0.5f, -0.5f, 

                -0.5f,  0.5f, -0.5f, 
                0.5f,  0.5f, -0.5f,  
                0.5f,  0.5f,  0.5f,  
                0.5f,  0.5f,  0.5f,  
                -0.5f,  0.5f,  0.5f, 
                -0.5f,  0.5f, -0.5f, 
        };

        unsigned int VBO, cubeVAO;
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, std::span(vertices).size_bytes(), vertices.data(), GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        
        // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
        unsigned int lightCubeVAO;
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        window.start([&]() {
                float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window.window());

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // be sure to activate shader when setting uniforms/drawing objects
                lightingShader.use();
                lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
                lightingShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

                

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader.setMat4("projection", projection);

                /// 观察矩阵
                /// 把世界坐标系“搬”到相机坐标系
                const glm::mat4 view = camera.GetViewMatrix();
                lightingShader.setMat4("view", view);

                /// world transformation
                /// 把模型摆到世界里的位置（平移/旋转/缩放）
                glm::mat4 model = glm::mat4(1.0f);
                lightingShader.setMat4("model", model);

                // render the cube
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);


                lightCubeShader.use();
                lightCubeShader.setMat4("projection", projection);
                lightCubeShader.setMat4("view", view);

                model = glm::mat4(1.0f);
                model = glm::translate(model, lightPos);                /// 平移到光源位置           
                model = glm::scale(model, glm::vec3(0.2f));     /// 缩小到 0.2 倍
                lightCubeShader.setMat4("model", model);

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
        });


	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

        return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	const float xpos = static_cast<float>(xposIn);
    	const float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	const float xoffset = xpos - lastX;
	const float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window)
{
    	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}