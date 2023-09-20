#include "engine/Engine.h"

#include "engine/shared/AABB.h"
#include "engine/shared/Time.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Config.h"
#include "engine/shared/UVRect.h"
#include "engine/shared/GLCallbacks.h"

#include "engine/Renderer.h"
#include "engine/Texture.h"
#include "engine/Input.h"
#include "engine/Level.h"
#include "engine/TriggerBox.h"
#include "engine/Camera.h"

#include "engine/audio/AudioSystem.h"



void Engine::Init()
{
	if (!glfwInit())
	{
		std::cerr << "\nError: Failed GLFW init" << std::endl;
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);

	editorWindow = glfwCreateWindow(Config::winH, Config::winW, "Editor", nullptr, nullptr);
	if (!editorWindow)
	{
		std::cerr << "\nError: Failed create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_REFRESH_RATE, 80);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	glfwSetWindowSizeLimits(editorWindow, 200, 155, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(editorWindow);
	glbinding::initialize(glfwGetProcAddress);
	Custom::GetVendorInfo();

	Config::LoadConfig(editorWindow);
	AudioSystem::Initialize();
	CallbacksInput(editorWindow);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Engine::Render()
{
	Camera camera;

	Level level("Data/Levels/Level.xml", camera);
	auto levelData = level.getLoadedLevel();

	Shader spriteShader("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag");
	auto render = std::make_unique<Renderer>(spriteShader);

	Shader uiShader("Data/Shaders/UI.vert", "Data/Shaders/UI.frag");
	auto UIR = std::make_unique<Renderer>(uiShader);

	Shader aabbShader("Data/Shaders/AABB.vert", "Data/Shaders/AABB.frag");
	auto ABR = std::make_unique<Renderer>(aabbShader);

	Shader polylineShader("Data/Shaders/Polyline.vert", "Data/Shaders/Polyline.frag");
	auto PLR = std::make_unique<Renderer>(polylineShader);




	//AABB
	auto atlas = std::make_shared<Texture>("Data/Engine/TagIcons.png", "CC");
	glm::vec3 pos(5.0F, 0.0F, 3.5F);

	AABB aabb;
	aabb.setPosition(pos);

	TriggerBox trigger;
	trigger.setSize(aabb);
	trigger.setPosition(pos);

	IconData bounce;
	bounce.position = trigger.getPosition();
	bounce.type = IconType::TRIGGER;

	AABB aabb_BG;
	aabb_BG.setSize(glm::vec3(3.0F, 3.0F, 0.5F));
	aabb_BG.setPosition(glm::vec3(0.0F, 0.0F, -7.0F));


	//Test filled polygon
	auto fill = std::make_shared<Texture>("Data/Textures/Food Land/square_fill.png");

	std::vector<GLfloat> points;
	std::vector<glm::vec2> values
	{
		 glm::vec2(-3.0F,  -0.25F),
		 glm::vec2(3.25F, -0.25F),
		 glm::vec2(3.25F, -2.7F),
		 glm::vec2(5.75F, -2.7F),
		 glm::vec2(5.75F, -0.25F),
		 glm::vec2(7.25F, -0.25F),
		 glm::vec2(7.25F, -4.5F),
		 glm::vec2(-3.0F,  -4.5F),
	};

	

	GLfloat uvScale = 2.0F;
	for (const auto &val : values)
	{
		auto x = val.x;
		auto y = val.y;
		auto uvx = x / uvScale;
		auto uvy = y / uvScale;

		points.emplace_back(x);
		points.emplace_back(y);
		points.emplace_back(uvx);
		points.emplace_back(uvy);
	}

	std::cout << "\n\n";
	GLuint counter = 0;
	for (const auto &value : points)
	{
		std::cout << value << ", ";

		counter++;
		if (counter % 4 == 0) std::cout << std::endl;
	}
	//End



	//Mus
	ALid medievalBeta = AudioSystem::LoadSound("Data/Audio/Music/(Beta) The Gallery - Medieval.flac");

	//Main loop
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(editorWindow))
	{
		Time::Start();
		Engine::RawInput(editorWindow, camera);

		//Custom::glClearColorHex("193366");
		//Custom::glClearColorHex("101010");
		Custom::glClearColorHex("101010");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto aspect = static_cast<GLfloat>(Config::FBW) / static_cast<GLfloat>(Config::FBH);
		auto view = camera.getViewMatrix();
		auto projection = glm::infinitePerspective(glm::radians(45.0F), aspect, 0.1F);

		spriteShader.use();
		spriteShader.setMat4("view", view);
		spriteShader.setMat4("projection", projection);
		spriteShader.setFloat("time", static_cast<GLfloat>(Time::FixedTime()));

		uiShader.use();
		uiShader.setMat4("view", view);
		uiShader.setMat4("projection", projection);

		aabbShader.use();
		aabbShader.setMat4("view", view);
		aabbShader.setMat4("projection", projection);

		polylineShader.use();
		polylineShader.setMat4("view", view);
		polylineShader.setMat4("projection", projection);

		//Drawing
		//PLR->DrawStrip(glm::vec3(0.0F, 0.0F, -0.003F), *fill, points);
		//if (Custom::isLevelLoaded)
		//{
		render->DrawSprite(levelData.value(), camera);
		//}


		glDisable(GL_DEPTH_TEST);
		ABR->DrawAABB(aabb);
		UIR->DrawIcon(bounce, camera, *atlas);
		glEnable(GL_DEPTH_TEST);


		//Logic update
		if (trigger.OnEnter(camera.getPosition()))
		{
			bounce.type = IconType::TRIGGER_INACTIVE;
			AudioSystem::StopSound(1);
			AudioSystem::PlaySound2D(medievalBeta, 0.5F, true);
		}

		glfwSwapBuffers(editorWindow);
		glfwPollEvents();
		glFinish();
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


void Engine::RawInput(GLFWwindow *window, Camera &cam)
{
	Input::ProcessInput(window, cam, Time::DeltaTime());
}


void Engine::CallbacksInput(GLFWwindow *window)
{
	//GLFW
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
	glfwSetCursorPosCallback(window, Input::CursorPositionCallback);
	glfwSetKeyCallback(window, Input::KeyboardCallback);
	glfwSetFramebufferSizeCallback(window, Input::FramebufferSizeCallback);
	glfwSetWindowSizeCallback(window, Input::WindowSizeCallback);
	glfwSetWindowCloseCallback(window, Input::WindowCloseCallback);
	glfwSetWindowMaximizeCallback(window, Input::WindowMaximizeCallback);
	glfwSetDropCallback(window, Input::FileDropCallback);

	//glDebugMessageCallback(GLCallbacks::MessageCallback, nullptr);
}

