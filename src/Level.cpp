#include "engine/Level.h"

Level::Level() {}

Level::Level(const std::string& filePath)
{
	load(filePath);
}


void Level::load(const std::string filePath)
{
	try
	{
		level = YAML::LoadFile(dataFolder + "/" + filePath);

		//READ LEVEL
		const auto textures = level["textures"];
		std::clog << "\nTEXTURES:";
		for (const auto &texture : textures)
		{
			auto name = texture["name"].as<std::string>();
			auto path = texture["path"].as<std::string>();

			std::clog
				<< "\nTexture name: " << name << "\n"
				<< "TexturePath: " << path << std::endl;

			texturesList.push_back({name, path});
		}

		const auto sprites = level["sprites"];
		std::clog << "\nSPIRTES:";
		for (const auto &sprite : sprites)
		{
			SpriteData spriteData;
			
			//Load sprite data
			auto name = sprite["name"].as<std::string>();
			auto texture = sprite["texture"].as<std::string>();
			auto posX = sprite["position"]["x"].as<double>();
			auto posY = sprite["position"]["y"].as<double>();
			auto posZ = sprite["position"]["z"].as<double>();
			auto speedX = sprite["scrollSpeed"]["x"].as<double>();
			auto speedY = sprite["scrollSpeed"]["y"].as<double>();
			auto rotation = sprite["rotation"].as<float>();
			auto sizeX = sprite["size"]["x"].as<double>();
			auto sizeY = sprite["size"]["y"].as<double>();
			auto tintR = sprite["tint"]["r"].as<double>();
			auto tintG = sprite["tint"]["g"].as<double>();
			auto tintB = sprite["tint"]["b"].as<double>();
			auto tintA = sprite["tint"]["a"].as<double>();

			//Set sprite data
			spriteData.name = name;
			
			spriteData.texture = texture;
			
			spriteData.position.x = posX;
			spriteData.position.y = posY;
			spriteData.position.z = posZ;
			
			spriteData.scrollSpeed.x = speedX;
			spriteData.scrollSpeed.y = speedY;
			
			spriteData.angle = rotation;
		
			spriteData.size.x = sizeX;
			spriteData.size.y = sizeY;
		
			spriteData.tint.r = tintR;
			spriteData.tint.r = tintG;
			spriteData.tint.r = tintB;
			spriteData.tint.r = tintA;

			spritesDataList.push_back(spriteData);

			//Load level log
			std::clog
				<< "\nSprite name: " << name << "\n"
				<< "Texture name: " << texture << "\n"
				<< "Position: (" << posX << ", " << posY << ", " << posZ << ")\n"
				<< "scrollSpeed: (" << speedX << ", " << speedY << ")\n"
				<< "Rotation: " << rotation << "\n"
				<< "Size: (" << sizeX << ", " << sizeY << ")\n"
				<< "Tint: (" << tintR << ", " << tintG << ", " << tintB << ", " << tintA << ")\n\n";
		}

		for (const auto &data : spritesDataList)
		{
			std::cout << "\n[SpiteData]\n"
				<< "Name: " << data.name << std::endl
				<< "Texture: " << data.texture << std::endl
				<< "Position: " << "(" << data.position.x << ", " << data.position.y << ", " << data.position.z << ")" << std::endl
				<< "ScrollSpeed: " << "x[" << data.scrollSpeed.x << "] y[" << data.scrollSpeed.y << "]\n"
				<< "Rotation Angle: " << data.angle << std::endl
				<< "Size: " << "x[" << data.size.x << "] y[" << data.size.y << "]\n"
				<< "Tint: " << "r[" << data.tint.r << "] g[" << data.tint.g << "] b[" << data.tint.b << "] a[" << data.tint.a << "]\n";
		};

	}
	catch (...)
	{
		std::clog << "Log: Level not loaded: " << dataFolder + "/" + filePath << std::endl;
	}
}


void Level::save()
{

}
