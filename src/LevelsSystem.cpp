#include "engine/LevelsSystem.h"
#include "engine/audio/AudioSystem.h"


LevelsSystem::LevelsSystem(Camera &cam) : cam(cam) {}

auto LevelsSystem::LoadLevel(std::string_view path) -> Level
{
	Camera &camera = cam;
	Level levelData;
	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePathMap;

	XmlDocument doc;
	if (!doc.load_file(path.data()))
	{
		std::cerr << "\nFailed to open level file: " << path.data() << std::endl;
		return levelData;
	}

	XmlNode levelNode = doc.child("Level");
	XmlNode spritesNode = levelNode.child("Sprites");

	//Load camera
	XmlNode cameraNode = levelNode.child("Camera");
	if (!cameraNode.empty())
	{
		std::string attrCameraPos = cameraNode.attribute("position").as_string();

		auto cameraPos = SplitStringToVec<glm::vec3>(attrCameraPos);
		camera.setPosition(cameraPos);
	}


	//Load textures
	XmlNode texturesNode = levelNode.child("Textures");
	for (XmlNode textureNode : texturesNode.children("Texture"))
	{
		// Parse texture attributes
		std::string attrName = textureNode.attribute("name").as_string();
		std::string attrPath = textureNode.attribute("path").as_string();
		std::string attrWrap = textureNode.attribute("wrapMode").as_string();

		if (attrName.empty() || attrPath.empty())
		{
			continue; //Skip this texture
		}

		auto [it, inserted] = texturePathMap.emplace(attrPath, std::make_shared<Texture>());
		if (inserted)
		{
			it->second->Generate(attrPath, attrWrap);
		}
		textureMap[attrName] = it->second;
	}

	//Load audio
	for (XmlNode audioNode : levelNode.children("Audio2D"))
	{
		std::string attrPath = audioNode.attribute("path").as_string();
		ALfloat attrVolume = audioNode.attribute("volume").as_float();
		ALboolean attrLoop = audioNode.attribute("loop").as_bool();

		auto track = AudioSystem::LoadSound(attrPath);
		AudioSystem::PlaySound2D(track, attrVolume, attrLoop);
		//AudioSystem::SetSoundStartTime(track, 50.0F);
	}

	//Load audio
	for (XmlNode audioNode : levelNode.children("Audio3D"))
	{
		std::string attrPath = audioNode.attribute("path").as_string();
		std::string attrPos  = audioNode.attribute("position").as_string();
		ALfloat attrVolume = audioNode.attribute("volume").as_float();
		ALboolean attrLoop = audioNode.attribute("loop").as_bool();

		auto pos = SplitStringToVec<glm::vec3>(attrPos);

		auto track = AudioSystem::LoadSound(attrPath);
		AudioSystem::PlaySound3D(track, pos, attrVolume, attrLoop);
	}

	//Load sprites
	for (XmlNode spriteNode : spritesNode.children("Sprite"))
	{
		Level::Sprite sprite;

		//Get attributes
		std::string attrName	 = spriteNode.attribute("name"	   ).as_string();
		std::string attrTexture  = spriteNode.attribute("texture"  ).as_string();
		std::string attrUVRect   = spriteNode.attribute("uv"	   ).as_string();
		std::string attrPosition = spriteNode.attribute("position" ).as_string();
		std::string attrScroll   = spriteNode.attribute("scSpeed"  ).as_string();
		std::string attrRotation = spriteNode.attribute("rotAngle" ).as_string();
		std::string attrSize	 = spriteNode.attribute("size"	   ).as_string();
		std::string attrColor	 = spriteNode.attribute("color"    ).as_string();
		bool attrIsLight		 = spriteNode.attribute("isLight"  ).as_bool();
		bool attrUseFog			 = spriteNode.attribute("useFog"   ).as_bool();



		if (attrUVRect.empty()) { attrUVRect = static_cast<std::string>("0,0,1,1"); }
		if (attrSize.empty())   { attrSize   = static_cast<std::string>("1,1");     }
		if (attrColor.empty())  { attrColor  = static_cast<std::string>("1,1,1,1"); }
		if (!spriteNode.attribute("isLight")) { attrIsLight = false; }
		if (!spriteNode.attribute("useFog"))  { attrUseFog  = true; }


		//Other
		auto textureIterator = textureMap.find(attrTexture);

		sprite.name     = attrName;
		sprite.texture  = (textureIterator != textureMap.end()) ? textureIterator->second : missingTexture;
		sprite.uvRect   = SplitStringToVec<UVRect>(attrUVRect);
		sprite.position = SplitStringToVec<glm::vec3>(attrPosition);
		sprite.scSpeed  = SplitStringToVec<glm::vec2>(attrScroll);
		sprite.rotation = SplitStringToVec<glm::vec3>(attrRotation);
		sprite.size		= SplitStringToVec<glm::vec2>(attrSize);
		sprite.tint		= SplitStringToVec<glm::vec4>(attrColor);
		sprite.isLight  = attrIsLight;
		sprite.useFog   = attrUseFog;

		// Parse sprite animation attributes
		if (spriteNode.child("Animation"))
		{
			XmlNode animationNode = spriteNode.child("Animation");
			if (!animationNode.empty())
			{
				sprite.isAnim = true;
				sprite.animation.type = animationNode.attribute("type").as_string();
				sprite.animation.speed = animationNode.attribute("speed").as_float();
			}
		}
		else { sprite.isAnim = false; }

		// Parse sprite àdditional attributes
		if (spriteNode.child("Additional"))
		{
			sprite.addin.isAdditional = true;

			XmlNode addinNode = spriteNode.child("Additional");
			if (!addinNode.empty())
			{
				XmlNode fadeNode = addinNode.child("OpacityFade");
				if (!fadeNode.empty())
				{
					sprite.addin.isFade = true;
					sprite.addin.fadeEnd = fadeNode.attribute("fadeEnd").as_float();
					std::cout << "Fade value: " << fadeNode.attribute("fadeEnd").as_float() << std::endl;
				}
				else { sprite.addin.isFade = false; }
			}
		}
		else { sprite.addin.isAdditional = false; }

		levelData.sprites.emplace_back(sprite);
	}

	return levelData;
}

auto LevelsSystem::SaveLevel(std::string_view path) -> void
{

}

auto LevelsSystem::PreloadEngineResources(std::string_view path) -> void
{
	missingTexture = std::make_shared<Texture>();
	missingTexture->Generate(path.data());
}

//Utils
template <typename T>
auto LevelsSystem::SplitStringToVec(const std::string &str) -> T
{
	std::vector<GLfloat> floatValues;
	std::stringstream ss(str);
	std::string substring;

	while (getline(ss, substring, ','))
	{
		if (substring.empty())
		{
			floatValues.push_back(0.0);
		}
		else
		{
			try
			{
				floatValues.push_back(std::stof(substring));
			}
			catch (const std::exception &e)
			{
				std::cerr << "Error: Unable to convert substring: " << e.what() << std::endl;
				floatValues.push_back(0.0);
			}
		}
	}

	T result{};
	size_t numFloats = std::min(floatValues.size(), sizeof(T) / sizeof(float));
	for (size_t i = 0; i < numFloats; ++i)
	{
		reinterpret_cast<float*>(&result)[i] = floatValues[i];
	}

	return result;
}

auto LevelsSystem::SplitStringToVec2(const std::string &str) -> std::vector<glm::vec2>
{
	std::vector<glm::vec2> uvCoordinates;
	std::stringstream ss(str);
	std::string substring;

	while (getline(ss, substring, ','))
	{
		try
		{
			GLfloat x = std::stof(substring);
			if (getline(ss, substring, ','))
			{
				GLfloat y = std::stof(substring);
				uvCoordinates.emplace_back(x, y);
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: Unable to convert substring: " << e.what() << std::endl;
			uvCoordinates.emplace_back(0.0F, 0.0F);
		}
	}

	return uvCoordinates;
}

auto LevelsSystem::GetDebugInfo() -> void
{
	//Print loaded textures
	for (const auto &pair : textureMap)
	{
		const auto &name = pair.first;
		const auto &texture = pair.second;

		std::cout << "Loaded texture: [name: " << name << " id: " << texture->getId() << "]" << std::endl;
	}
}