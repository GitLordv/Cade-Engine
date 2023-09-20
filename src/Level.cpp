#include "engine/Level.h"
#include "engine/audio/AudioSystem.h"


Level::Level(const std::string &path, Camera &cam) : cam(cam) 
{
	if(!path.empty()) loadedLevel = LoadLevel(path);

    missingTexture = std::make_shared<Texture>();
	missingTexture->Generate("Data/Engine/Missing.png");
}

auto Level::LoadLevel(const std::string &path) -> LevelData
{
    //Camera &camera = cam;
    LevelData levelData;
    std::unordered_map<std::string, std::shared_ptr<Texture>> texturePaths;


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
        //Get attribute values
        std::string position = cameraNode.attribute("position").as_string();

        auto cameraPos = SplitStringToVec<glm::vec3>(position);    
        cam.setPosition(cameraPos);
    }

    //Load textures
    XmlNode texturesNode = levelNode.child("Textures");
    for (XmlNode textureNode : texturesNode.children("Texture"))
    {
        std::string attrName = textureNode.attribute("name").as_string();
        std::string attrPath = textureNode.attribute("path").as_string();
        std::string attrWrap = textureNode.attribute("wrap").as_string();

        if (attrName.empty() || attrPath.empty())
        {
            continue; //Skip this texture
        }

        auto [it, inserted] = texturePaths.emplace(attrPath, std::make_shared<Texture>());
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
        float attrVolume = audioNode.attribute("volume").as_float();
        bool attrLoop = audioNode.attribute("loop").as_bool();

        AudioSystem::PlaySound2D(AudioSystem::LoadSound(attrPath), attrVolume, attrLoop);
    }

    //Load sprites
    for (XmlNode spriteNode : spritesNode.children("Sprite"))
    {
        SpriteData sprite;

        //Get attribute values
        std::string name      = spriteNode.attribute("name"    ).as_string();
        std::string texture   = spriteNode.attribute("texture" ).as_string();
        std::string uvRect    = spriteNode.attribute("uv"      ).as_string();
        std::string uvRectPx  = spriteNode.attribute("uvpx"    ).as_string();
        std::string position  = spriteNode.attribute("position").as_string();
        std::string scroll    = spriteNode.attribute("scSpeed" ).as_string();
        std::string rotation  = spriteNode.attribute("angle"   ).as_string();
        std::string size      = spriteNode.attribute("size"    ).as_string();
        std::string color     = spriteNode.attribute("color"   ).as_string();
        float worldUV         = spriteNode.attribute("WuvScale").as_float();
        bool isLight          = spriteNode.attribute("light"   ).as_bool();
        bool useFog           = spriteNode.attribute("useFog"  ).as_bool();

        if (!uvRectPx.empty())
        {
            sprite.isUvAsPixels = true;
            spriteNode.remove_attribute("uv");
        }

        if (spriteNode.attribute("WuvScale"))
        {
            sprite.isWorldUV = true;
            sprite.isUvAsPixels = false;
            spriteNode.remove_attribute("uv");
            spriteNode.remove_attribute("uvpx");
            sprite.uvScale = worldUV;
        }

        if (uvRect.empty())
        {
            uvRect = "0,0,1,1";
        }

        if (size.empty())
        {
            size = "1,1";
        }

        if (color.empty())
        {
            color = "1,1,1,1";
        }

        if (!spriteNode.attribute("light"))
        {
            isLight = false;
        }

        if (!spriteNode.attribute("useFog"))
        {
            useFog = true;
        }

        auto iterator = textureMap.find(texture);
        if (iterator != textureMap.end())
        {
            sprite.texture = iterator->second;
        }
        else
        {
            sprite.texture = missingTexture;
        }

        sprite.uvRect = sprite.isUvAsPixels ? SplitStringToVec<UVRect>(uvRectPx) : SplitStringToVec<UVRect>(uvRect);
        sprite.position = SplitStringToVec<glm::vec3>(position);
        sprite.rotation = SplitStringToVec<glm::vec3>(rotation);
        sprite.scSpeed  = SplitStringToVec<glm::vec2>(scroll);
        sprite.size = SplitStringToVec<glm::vec2>(size);
        sprite.tint = SplitStringToVec<glm::vec4>(color);
        sprite.isLight = isLight;
        sprite.useFog  = useFog;

        //Parse sprite animation attributes
        XmlNode animationNode = spriteNode.child("Animation");
        if (!animationNode.empty())
        {
            sprite.isAnim = true;
            sprite.animation.type = animationNode.attribute("type").as_string();
            sprite.animation.speed = animationNode.attribute("speed").as_float();
        }
        else
        {
            sprite.isAnim = false;
        }

        //Parse sprite additional attributes
        XmlNode addinNode = spriteNode.child("Additional");
        if (!addinNode.empty())
        {
            sprite.addin.isAdditional = true;

            XmlNode fadeNode = addinNode.child("OpacityFade");
            if (!fadeNode.empty())
            {
                sprite.addin.isFade = true;
                sprite.addin.fadeEnd = fadeNode.attribute("fadeEnd").as_float();
            }
            else
            {
                sprite.addin.isFade = false;
            }
        }
        else
        {
            sprite.addin.isAdditional = false;
        }

        levelData.sprites.emplace_back(sprite);
    }

    return levelData;
}

auto Level::SaveLevel(std::string_view path) -> void
{

}

auto Level::getLoadedLevel() -> std::optional<LevelData>
{
	if (loadedLevel.sprites.size() > 0)
	{
		return loadedLevel;
	}
	else
	{
		std::cout << "\nLEVEL: Level is empty!\n";
		return std::nullopt;
	}
	return std::nullopt;
}


//Utils
template <typename T>
auto Level::SplitStringToVec(const std::string &str) -> T
{
	std::vector<GLfloat> floatValues;
	std::stringstream ss(str);
	std::string substring;

	while (getline(ss, substring, ','))
	{
		if (substring.empty())
		{
			floatValues.emplace_back(0.0);
		}
		else
		{
			try
			{
				floatValues.emplace_back(std::stof(substring));
			}
			catch (const std::exception &e)
			{
				std::cerr << "Error: Unable to convert substring: " << e.what() << std::endl;
				floatValues.emplace_back(0.0);
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
