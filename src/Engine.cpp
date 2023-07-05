#include "engine/shared/Config.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Utils.h"
#include "engine/shared/Time.h"
#include "engine/shared/UVRegion.h"
#include "engine/SpriteRenderer.h"
#include "engine/Texture.h"
#include "engine/Engine.h"
#include "engine/Input.h"
#include "engine/Level.h"
#include "engine/Draw.h"


void Engine::Init()
{
	if (!glfwInit())
	{
		std::cerr << "\nError: Failed GLFW init\n";
		glfwTerminate();
	}

	window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "Engine", nullptr, nullptr);
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
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetWindowSizeLimits(window, 400, 300, GLFW_DONT_CARE, GLFW_DONT_CARE);

	Config::LoadConfig(window);

	glfwMakeContextCurrent(window);
	glbinding::initialize(glfwGetProcAddress);
	Custom::GetVendorInfo();

	CallbacksInput(window);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
}


void Engine::Render()
{
	Shader spriteShader("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag");


	//Experemental
	std::vector<std::string> texturePaths =
	{
		"Data/Textures/canopysky.png",
		"Data/Textures/bg_mountain_chateau.png",
		"Data/Textures/djembetente.png",
		"Data/Textures/bg_mountain_back_new-1.png"
	};

	std::vector<std::unique_ptr<Texture>> textures;
	for (size_t i = 0; i < texturePaths.size(); ++i)
	{
		auto texture = std::make_unique<Texture>();
		texture->Generate(texturePaths[i]);
		texture->getId();

		textures.push_back(std::move(texture));
	}

	std::vector<glm::vec3> positions =
	{
		glm::vec3(1.0f, 1.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(0.5f, -0.5f, 0.0f)
	};

	UVRegion uv;

	//Level level1;
	//level1.load("Levels/intro.lvl");

	auto sr = std::make_unique<SpriteRenderer>(spriteShader);

	//Set params for camera
	CameraConfig camConfig;
	camConfig.position = glm::vec3(0.0f, 0.0f, 3.0f);
	Camera camera(camConfig);

	// Time initialization
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		Time::Start();

		Input::ProcessInput(window, camera, Time::GetDeltaTime());

		Custom::glClearColorHex("9197A7");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		auto aspect = static_cast<float>(Config::fbW) / static_cast<float>(Config::fbH);
		auto view = camera.GetViewMatrix();
		auto projection = glm::perspective(glm::radians(45.0f), aspect, 0.05f, 100.0f);

		spriteShader.use();
		spriteShader.setMat4("view", view);
		spriteShader.setMat4("projection", projection);
		spriteShader.setFloat("time", static_cast<GLfloat>(Time::GetMainTime()));

		for (size_t i = 0; i < textures.size(); ++i)
		{
			sr->DrawSprite(*textures[i], uv, positions[i], glm::vec2(0.0f), 14.0f, glm::vec2(1.2f), glm::vec4(1.0f));
		}

		glfwSwapBuffers(window);
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
	glfwTerminate();
}


void Engine::RawInput(GLFWwindow *window)
{
	//Input::ProcessInput(window, cam, Time::GetDeltaTime());
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
}