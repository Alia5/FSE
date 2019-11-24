#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <map>

namespace fse
{
	/*!
	* \brief Handles string based texture and asset loading
	*/
	class AssetLoader {
	public:

		/*!
		 * Wraps sfml textures neatly
		 */
		class Texture
		{
		public:
			Texture();
			Texture(const Texture &texture);
			Texture(AssetLoader* loader, const std::string& path, sf::Texture* tex);
			~Texture();
			[[nodiscard]] sf::Texture& get() const;

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

		sf::Texture& getSFTexture(const std::string& path);
		AssetLoader::Texture getTexture(const std::string& path);
		void releaseSFTexture(const std::string& path);

	private:

		void loadTexture(const std::string& path);

		std::map<std::string, sf::Texture> texture_map_;
		std::map<std::string, unsigned int> texture_counter_;

	};

	using Texture = AssetLoader::Texture;

}