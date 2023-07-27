#include "engine/shared/Utils.h"
#include "engine/shared/Types.h"
#include "engine/shared/Time.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Config.h"
#include "engine/shared/UVRegion.h"

#include "engine/SpriteRenderer.h"
#include "engine/Texture.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "engine/Level.h"
#include "engine/Draw.h"

#include "engine/audio/AudioSystem.h"


void Engine::Init()
{
	if (!glfwInit())
	{
		std::cerr << "\nError: Failed GLFW init\n";
		glfwTerminate();
	}

	window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "Editor", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Error: Failed create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwSetWindowSizeLimits(window, 200, 151, GLFW_DONT_CARE, GLFW_DONT_CARE);

	Config::LoadConfig(window);

	glfwMakeContextCurrent(window);
	glbinding::initialize(glfwGetProcAddress);
	Custom::GetVendorInfo();

	AudioSystem::Initialize();
	CallbacksInput(window);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Engine::Render()
{
	//Make camera params
	CameraConfig camConfig;
	camConfig.position = glm::vec3(0.0f, 0.0f, 3.0f);
	camConfig.speed = 0.95f;

	Camera camera(camConfig);

	Shader spriteShader("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag");

	//Make sprite render
	auto sr = std::make_unique<SpriteRenderer>(spriteShader);

	//Load level
	auto levelSprites = Level::LoadLevel("Data/Levels/intro.xml");

	//ZDepth sort sprites in level
	std::sort(levelSprites.begin(), levelSprites.end(), 
		[](const SpriteData &a, const SpriteData &b) { return a.position.z >= b.position.z; });

	//Print loaded textures
	/*for (const auto &pair : Level::textureMap)
	{
		const auto &name = pair.first;
		const auto &texture = pair.second;

		std::cout << "Loaded texture: [name: " << name << " id: " << texture->getId() << "]" << std::endl;
	}*/

	glfwSwapInterval(0);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		Time::Start();
		Engine::RawInput(window, camera);

		Custom::glClearColorHex("193366");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto aspect = static_cast<float>(Config::fbW) / static_cast<float>(Config::fbH);
		auto view = camera.getViewMatrix();
		auto projection = glm::perspective(glm::radians(45.0f), aspect, 0.05f, 1000.0f);

		spriteShader.use();
		spriteShader.setMat4("view", view);
		spriteShader.setMat4("projection", projection);
		spriteShader.setFloat("time", static_cast<GLfloat>(Time::GetMainTime()));


		for (auto it = levelSprites.rbegin(); it != levelSprites.rend(); ++it)
		{
			sr->DrawSpriteInstanced(*it);
		}

		glfwSwapBuffers(window);
		glFinish();
		glfwPollEvents();
	}

	Destroy();
}


void Engine::Startup()
{
	Init();
	Render();
}


void Engine::Destroy()
{
	AudioSystem::Shutdown();
	glfwTerminate();
}


void Engine::RawInput(GLFWwindow *window, Camera& cam)
{
	Input::ProcessInput(window, cam, Time::GetDeltaTime());
}


void Engine::CallbacksInput(GLFWwindow *window)
{
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
	glfwSetCursorPosCallback(window, Input::CursorPositionCallback);
	glfwSetKeyCallback(window, Input::KeyboardCallback);
	glfwSetFramebufferSizeCallback(window, Input::FramebufferSizeCallback);
	glfwSetWindowSizeCallback(window, Input::WindowSizeCallback);
	glfwSetWindowCloseCallback(window, Input::WindowCloseCallback);
	glfwSetWindowMaximizeCallback(window, Input::WindowMaximizeCallback);
}