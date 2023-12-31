#include "engine/Engine.h"


void Engine::Init()
{
	if (!glfwInit())
	{
		std::cerr << "\nError: Failed GLFW init" << std::endl;
		glfwTerminate();
		return;
	}

	editorWindow = glfwCreateWindow(Config::winH, Config::winW, "Editor", nullptr, nullptr);
	if (!editorWindow)
	{
		std::cerr << "\nError: Failed create GLFW window" << std::endl;
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
	glfwSetWindowSizeLimits(editorWindow, 200, 151, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(editorWindow);
	glbinding::initialize(glfwGetProcAddress);
	Custom::GetVendorInfo();

	Config::LoadConfig(editorWindow);
	AudioSystem::Initialize();
	CallbacksInput(editorWindow);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}


void Engine::Render()
{
	Camera camera;
	LevelsSystem lvlSys(camera);

	Shader spriteShader("Data/Shaders/Sprite.vert", "Data/Shaders/Sprite.frag");
	auto SR = std::make_unique<Renderer>(spriteShader);

	Shader uiShader("Data/Shaders/UI.vert", "Data/Shaders/UI.frag");
	auto UIR = std::make_unique<Renderer>(uiShader);

	Shader aabbShader("Data/Shaders/AABB.vert", "Data/Shaders/AABB.frag");
	auto ABR = std::make_unique<Renderer>(aabbShader);

	lvlSys.PreloadEngineResources("Data/Engine/MissingTex.png");
	Level level = lvlSys.LoadLevel("Data/Levels/Level.xml");

	//Z-Depth sort sprites in level
	std::sort(level.sprites.begin(), level.sprites.end(),
		[](const Level::Sprite &a, const Level::Sprite &b) { return a.position.z >= b.position.z; });

	//Test
	glm::vec3 min(0.0F, 0.0F, 0.0F);
	glm::vec3 max(1.0F, 1.0F, 1.0F);
	AABB aabb(min, max);
	
	glm::vec3 pos(5.0F, 0.0F, 3.5F);
	aabb.setPosition(pos);

	TriggerBox trigger;
	trigger.setSize(aabb);
	trigger.setPosition(pos);
	trigger.setMode(TriggerMode::MULTIPLE);

	std::shared_ptr<Texture> tagIcons = std::make_shared<Texture>("Data/Engine/TagIcons.png", "CC");
	Icon bounce;
	bounce.position = trigger.getPosition();
	bounce.angle = 0.0F;
	bounce.size = glm::vec2(1.0F, 1.0F);
	bounce.type = IconType::TRIGGER;



	//Main loop
	glfwSwapInterval(0);
	while (!glfwWindowShouldClose(editorWindow))
	{
		Time::Start();
		Engine::RawInput(editorWindow, camera);

		Custom::glClearColorHex("193366");
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

		//Drawing sprites
		for (auto it = level.sprites.rbegin(); it != level.sprites.rend(); ++it)
		{
			SR->DrawSprite(*it, camera);
		}

		//Drawing icons test
		glDisable(GL_DEPTH_TEST);
		ABR->DrawAABB(aabb);
		UIR->DrawIcon(bounce, camera, *tagIcons);
		glEnable(GL_DEPTH_TEST);

		//Logic update
		if (trigger.OnEnter(camera.getPosition()))
		{
			bounce.type = IconType::TRIGGER_INACTIVE;
			AudioSystem::SetSoundStartTime(1, 70.0F);
		}

		glFinish();
		glfwSwapBuffers(editorWindow);
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

	//GL
	//glDebugMessageCallback(GLCallbacks::MessageCallback, nullptr);
}




