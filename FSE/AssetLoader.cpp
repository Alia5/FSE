#include "AssetLoader.h"

namespace fse
{
	AssetLoader::AssetLoader() {

	}

	AssetLoader::~AssetLoader()
	{
	}

	sf::Texture& AssetLoader::getTexture(std::string path)
	{
		if (textureMap.count(path)) //Texture is already loaded
		{
			textureCounter[path]++;
		} 
		else
		{
			loadTexture(path);
		}
		return textureMap[path];
	}

	void AssetLoader::releaseTexture(std::string path)
	{
		if (!textureCounter.count(path)) //Texture is not in memory
			return;
		if (textureCounter[path] == 1)
		{
			textureCounter.erase(path);
			textureMap.erase(path);
			return;
		}

		textureCounter[path]--;
	}

	void AssetLoader::loadTexture(std::string path)
	{
		sf::Texture texture;
		std::string realPath = "data/" + path;
		texture.loadFromFile(realPath.c_str());

		textureMap[path] = texture;
		textureCounter[path] = 0;
	}
}
