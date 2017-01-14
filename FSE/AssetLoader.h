#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <map>

namespace fse
{
	class AssetLoader {
	public:


		AssetLoader();
		~AssetLoader();

		sf::Texture& getTexture(std::string path);
		void releaseTexture(std::string path);

	private:

		void loadTexture(std::string path);

		std::map<std::string, sf::Texture> textureMap;
		std::map<std::string, unsigned int> textureCounter;

	};
}
