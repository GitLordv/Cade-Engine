#include "engine/Level.h"


std::vector<SpriteData> Level::LoadLevel(std::string_view filePath)
{
	std::vector<SpriteData> spriteData;
	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePathMap;

	auto splitStringToFloats = [](const std::string &str, const char delimiter = ',') -> std::vector<float>
	{
		std::vector<float> floatValues;
		std::stringstream ss(str);
		std::string substring;
		while (getline(ss, substring, delimiter)) {
			floatValues.push_back(std::stof(substring));
		}
		return floatValues;
	};
	auto splitStringToVec2 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec2
	{
		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
		if (floatValues.size() >= 2) {
			return glm::vec2(floatValues[0], floatValues[1]);
		}
		return glm::vec2(0.0f);
	};
	auto splitStringToVec3 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec3
	{
		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
		if (floatValues.size() >= 3) {
			return glm::vec3(floatValues[0], floatValues[1], floatValues[2]);
		}
		return glm::vec3(0.0f);
	};
	auto splitStringToVec4 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec4
	{
		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
		if (floatValues.size() >= 4) {
			return glm::vec4(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
		}
		return glm::vec4(0.0f);
	};
	auto splitStringToUVRegion = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> UVRegion
	{
		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
		if (floatValues.size() >= 4) {
			return UVRegion(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
		}
		return UVRegion(0.0f, 0.0f, 1.0f, 1.0f);
	};

	pugi::xml_document doc;
	if (!doc.load_file(filePath.data()))
	{
		std::clog << "\nFailed open level file: " << filePath << std::endl;
	}

	pugi::xml_node texturesNode = doc.child("Level").child("Textures");
	for (pugi::xml_node textureNode : texturesNode.children("Texture"))
	{
		if (!textureNode.attribute("name") ||
			!textureNode.attribute("path") ||

			textureNode.attribute("name").empty() ||
			textureNode.attribute("path").empty())
		{
			std::clog << "Missing or empty attributes in Texture node. Skipping." << std::endl;
			continue;
		}

		// Parse texture attributes
		std::string name = textureNode.attribute("name").as_string();
		std::string path = textureNode.attribute("path").as_string();

		// Check if texture path already exists
		auto textureIterator = texturePathMap.find(path);
		if (textureIterator != texturePathMap.end())
		{
			// Texture with the same path already exists, reuse it
			textureMap[name] = textureIterator->second;
		}
		else
		{
			// Texture is not loaded, create and load it
			auto texture = std::make_shared<Texture>();
			texture->Generate(path);
			textureMap[name] = texture;
			texturePathMap[path] = texture;
		}
	}

	pugi::xml_node spritesNode = doc.child("Level").child("Sprites");
	for (pugi::xml_node spriteNode : spritesNode.children("Sprite"))
	{
		if (!spriteNode.attribute("name")	  || 
			!spriteNode.attribute("texture")  || 
			!spriteNode.attribute("uv")		  ||
			!spriteNode.attribute("position") ||
			!spriteNode.attribute("scSpeed")  || 
			!spriteNode.attribute("rotAngle") ||
			!spriteNode.attribute("size")	  ||
			!spriteNode.attribute("color")    ||

			spriteNode.attribute("name").empty()	  ||
			spriteNode.attribute("texture").empty()   ||
			spriteNode.attribute("uv").empty()		  ||
			spriteNode.attribute("position").empty()  ||
			spriteNode.attribute("scSpeed").empty()   ||
			spriteNode.attribute("rotAngle").empty()  ||
			spriteNode.attribute("size").empty()	  ||
			spriteNode.attribute("color").empty())
		{
			std::clog << "Missing or empty attributes in Sprite node. Skipping." << std::endl;
			continue;
		}

		SpriteData sprite;
		sprite.name = spriteNode.attribute("name").as_string();
		sprite.texture = textureMap[spriteNode.attribute("texture").as_string()];
		sprite.uvRegion = UVRegion(splitStringToUVRegion(spriteNode.attribute("uv").as_string()));
		sprite.position = glm::vec3(splitStringToVec3(spriteNode.attribute("position").as_string()));
		sprite.scSpeed = glm::vec2(splitStringToVec2(spriteNode.attribute("scSpeed").as_string()));
		sprite.rotation = glm::vec3(splitStringToVec3(spriteNode.attribute("rotAngle").as_string()));
		sprite.size = glm::vec2(splitStringToVec2(spriteNode.attribute("size").as_string()));
		sprite.tint = glm::vec4(splitStringToVec4(spriteNode.attribute("color").as_string()));

		spriteData.push_back(sprite);
	}
	return spriteData;
}

//std::vector<SpriteData> Level::LoadLevel(const std::string &filePath)
//{
//	std::vector<SpriteData> spriteData;
//	std::unordered_map<std::string, std::shared_ptr<Texture>> texturePathMap;
//
//	auto splitStringToFloats = [](const std::string &str, const char delimiter = ',') -> std::vector<float>
//	{
//		std::vector<float> floatValues;
//		std::stringstream ss(str);
//		std::string substring;
//		while (getline(ss, substring, delimiter)) {
//			floatValues.push_back(std::stof(substring));
//		}
//		return floatValues;
//	};
//	auto splitStringToVec2 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec2
//	{
//		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
//		if (floatValues.size() >= 2) {
//			return glm::vec2(floatValues[0], floatValues[1]);
//		}
//		return glm::vec2(0.0f);
//	};
//	auto splitStringToVec3 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec3
//	{
//		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
//		if (floatValues.size() >= 3) {
//			return glm::vec3(floatValues[0], floatValues[1], floatValues[2]);
//		}
//		return glm::vec3(0.0f);
//	};
//	auto splitStringToVec4 = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> glm::vec4
//	{
//		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
//		if (floatValues.size() >= 4) {
//			return glm::vec4(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
//		}
//		return glm::vec4(0.0f);
//	};
//	auto splitStringToUVRegion = [splitStringToFloats](const std::string &str, const char delimiter = ',') -> UVRegion
//	{
//		std::vector<float> floatValues = splitStringToFloats(str, delimiter);
//		if (floatValues.size() >= 4) {
//			return UVRegion(floatValues[0], floatValues[1], floatValues[2], floatValues[3]);
//		}
//		return UVRegion(0.0f, 0.0f, 1.0f, 1.0f);
//	};
//
//	pugi::xml_document doc;
//	if (!doc.load_file(filePath.c_str()))
//	{
//		std::clog << "\nFailed open level file: " << filePath << std::endl;
//	}
//
//	pugi::xml_node texturesNode = doc.child("Level").child("Textures");
//	for (pugi::xml_node textureNode : texturesNode.children("Texture"))
//	{
//		// Parse texture attributes
//		std::string name = textureNode.attribute("name").as_string();
//		std::string path = textureNode.attribute("path").as_string();
//
//		// Check if texture path already exists
//		auto textureIterator = texturePathMap.find(path);
//		if (textureIterator != texturePathMap.end())
//		{
//			// Texture with the same path already exists, reuse it
//			textureMap[name] = textureIterator->second;
//		}
//		else
//		{
//			// Texture is not loaded, create and load it
//			auto texture = std::make_shared<Texture>();
//			texture->Generate(path);
//			textureMap[name] = texture;
//			texturePathMap[path] = texture;
//		}
//	}
//
//	pugi::xml_node spritesNode = doc.child("Level").child("Sprites");
//	for (pugi::xml_node spriteNode : spritesNode.children("Sprite"))
//	{
//		SpriteData sprite;
//		sprite.name = spriteNode.attribute("name").as_string();
//		sprite.texture = textureMap[spriteNode.attribute("texture").as_string()];
//		sprite.uvRegion = UVRegion(splitStringToUVRegion(spriteNode.attribute("uv").as_string()));
//		sprite.position = glm::vec3(splitStringToVec3(spriteNode.attribute("position").as_string()));
//		sprite.scSpeed = glm::vec2(splitStringToVec2(spriteNode.attribute("scSpeed").as_string()));
//		sprite.rotation = glm::vec3(splitStringToVec3(spriteNode.attribute("rotAngle").as_string()));
//		sprite.size = glm::vec2(splitStringToVec2(spriteNode.attribute("size").as_string()));
//		sprite.tint = glm::vec4(splitStringToVec4(spriteNode.attribute("color").as_string()));
//
//		spriteData.push_back(sprite);
//	}
//	return spriteData;
//}

void Level::SaveLevel(const std::string &filePath)
{

}
