#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <map>

namespace fse
{
	class AssetLoader {
	public:

		class Texture
		{
		public:
			Texture();
			Texture(const Texture &texture);
			Texture(AssetLoader* loader, std::string path, sf::Texture* tex);
			~Texture();
			sf::Texture& get() const;

			Texture& operator=(const Texture& texture);
			sf::Texture* operator->() const;
			sf::Texture& operator*() const
			{
				return *tex_;
			}
		private:
			AssetLoader* loader_;
			std::string path_;
			sf::Texture* tex_;
		};

		AssetLoader();
		~AssetLoader();

		sf::Texture& getSFTexture(std::string path);
		AssetLoader::Texture getTexture(std::string path);
		void releaseSFTexture(std::string path);

	private:

		void loadTexture(std::string path);

		std::map<std::string, sf::Texture> texture_map_;
		std::map<std::string, unsigned int> texture_counter_;

	};

	using Texture = AssetLoader::Texture;

}
