#include <array>
#include <filesystem>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <format>
#include <sys/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::filesystem::path getExecutableDir();

#ifdef _WIN32
  #include <windows.h>
#elif defined(__linux__)
  #include <unistd.h>
  #include <limits.h>
#elif defined(__APPLE__)
  #include <mach-o/dyld.h>
#endif

constexpr float SCR_WIDTH = 800;
constexpr float SCR_HEIGHT = 600;

constexpr auto float_size = sizeof(float);

template <class T, std::size_t N>
constexpr std::size_t byte_count(const std::array<T, N>& a) noexcept {
    return N * sizeof(T);
}



struct Image 
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = nullptr;
	// std::byte* data2;
};

Image readImage(const char* path);


static float alpha = 0.2;

int main(int, char**)
{
    	std::cout << "矩阵变换!\n";


	/// 初始化窗口
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);     // 设为当前线程的主上下文
	// 我们还需要注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	constexpr std::array<float, 180> vertices = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	
	/// 生成一个带有 ID 的 顶点缓冲对象 VBO，
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	/// 先绑定 VAO, 再绑定设置 VBO, and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/// 把之前定义的顶点数据复制到缓冲的内存中
	/// 第四个参数指定了我们希望显卡如何管理给定的数据
	glBufferData(GL_ARRAY_BUFFER, std::span(vertices).size_bytes(), vertices.data(), GL_STATIC_DRAW);

	// position attribute
	/// float 32 位，4 byte, 
	/// 注意调整 步长为8
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT,
		GL_FALSE,
		5 * float_size, (void*)0
	);
	/// glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据
	glEnableVertexAttribArray(0);


	/// texture coord attribute
	/// 传给 顶点着色器的 vec2 aTexCoord，然后再传给 片段着色器
	glVertexAttribPointer(
		1, 2,
		GL_FLOAT,
		GL_FALSE, 5 * float_size,
		(void*)(3 * float_size)
	);
	glEnableVertexAttribArray(1);


	/// 纹理加载生成
	/// =================================================

	auto container_jpg = std::format("{}/container.jpg", getExecutableDir().c_str());
	auto awesomeface_png = std::format("{}/awesomeface.png", getExecutableDir().c_str());



	stbi_set_flip_vertically_on_load(true);
	auto image = readImage(container_jpg.c_str());
	if (image.data == nullptr) 
	{
		std::cerr << "Failed to load image container\n";
	}

	// 生成纹理 纹理也是使用ID引用的
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/**
	 * 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
	 * 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	 * 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	 * 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
	 * 下个参数应该总是被设为0（历史遗留的问题）。
	 * 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
	 * 最后一个参数是真正的图像数据。
	 */
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		image.width, image.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image.data
	);

	/// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image.data);

	auto image_face = readImage(awesomeface_png.c_str());
	if (image_face.data == nullptr) 
	{
		std::cerr << "Failed to load face image\n";
	}

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/// 这里是 RGBA 的图像
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		image_face.width, image_face.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image_face.data
	);

	/// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image_face.data);


	// 请注意，这种操作是允许的。glVertexAttribPointer的调用会将VBO注册为该顶点属性所对应的顶点缓冲区对象。这样一来，我们之后就可以安全地解除该VBO的绑定。
    	// glBindBuffer(GL_ARRAY_BUFFER, 0); 

    	// 之后可以解除VAO的绑定，这样其他VAO的调用就不会意外地修改这个VAO了。不过这种情况很少发生。
	// 无论如何，要修改其他VAO的话，都需要先调用glBindVertexArray。因此，除非确实有必要，否则我们通常不会解除VAO或VBO的绑定。
    	// glBindVertexArray(0); 

	auto vs_path = std::format("{}/Shader.vs", getExecutableDir().c_str());
	auto fs_path = std::format("{}/Shader.fs", getExecutableDir().c_str());
	Shader shader(vs_path.c_str(), fs_path.c_str());
	

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

        /// 投影矩阵，因为很少需要变化，所以设置在循环外
        glm::mat4 projection = glm::mat4(1.0f);
	auto aspect = SCR_WIDTH / SCR_HEIGHT;
	// aspect = 4 / 3;
        projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

                
		shader.use();
		shader.setFloat("alpha", alpha);

                /// 模型矩阵
                glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(
			model, 
			(float)glfwGetTime() * glm::radians(50.0f), 
			glm::vec3(0.5f, 1.0f, 0.0f)
		);

                /// 观察矩阵
                glm::mat4 view = glm::mat4(1.0f);
                // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
                view = glm::translate(view, glm::vec3(-3.0f, 2.0f, -9.0f));


                shader.setMat4("model", model);
                shader.setMat4("view", view);
                
                
                
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		alpha += 0.001f;
		if (alpha >= 1.0f) alpha = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		alpha -= 0.001f;
		if (alpha <= 0.0f) alpha = 0.0f;
	}
}

Image readImage(const char* path) 
{
	Image ret;
	ret.data = stbi_load(path, &ret.width, &ret.height, &ret.nrChannels, 0);
	return ret;
}	

std::filesystem::path getExecutableDir()
{
	namespace fs = std::filesystem;

#ifdef _WIN32
	// Windows: GetModuleFileNameW，NULL 表示当前进程
	std::vector<wchar_t> buf(MAX_PATH);
	for (;;) {
		DWORD n = ::GetModuleFileNameW(nullptr, buf.data(),
					static_cast<DWORD>(buf.size()));
		if (n == 0) return fs::current_path();           // 失败兜底
		if (n < buf.size()) break;                        // 成功且没截断
		buf.resize(buf.size() * 2);                       // 被截断，扩容重试
	}
	return fs::path(std::wstring(buf.data())).parent_path();

#elif defined(__linux__)
	// Linux: /proc/self/exe 是当前进程可执行文件的符号链接
	char buf[PATH_MAX];
	ssize_t n = ::readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (n <= 0) return fs::current_path();                // 失败兜底
	buf[n] = '\0';
	return fs::path(buf).parent_path();

#elif defined(__APPLE__)
	// macOS: _NSGetExecutablePath 返回的可能不是绝对路径，用 realpath 规整
	uint32_t size = 0;
	_NSGetExecutablePath(nullptr, &size);                 // 先问需要多大
	std::vector<char> buf(size);
	if (_NSGetExecutablePath(buf.data(), &size) != 0)
		return fs::current_path();
	char resolved[PATH_MAX];
	if (!::realpath(buf.data(), resolved))
		return fs::current_path();
	return fs::path(resolved).parent_path();

#else
	return fs::current_path();
#endif
	}