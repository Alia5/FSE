#include "AssetLoader.h"
#include <iostream>

namespace fse
{
	AssetLoader::AssetLoader() {

	}

	AssetLoader::~AssetLoader()
	{
	}

	sf::Texture& AssetLoader::getSFTexture(std::string path)
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

	AssetLoader::Texture AssetLoader::getTexture(std::string path)
	{
		return Texture(this, path, &getSFTexture(path));
	}

	void AssetLoader::releaseSFTexture(std::string path)
	{
		if (textureCounter.size() == 0)
			return;
		if (!textureCounter.count(path)) //Texture is not in memory
			return;
		if (textureCounter[path] == 1)
		{
			textureCounter.erase(path);
			textureMap.erase(path);

			std::cout << "erasing texture: " << path << "\n";

			return;
		}

		textureCounter[path]--;
	}

	void AssetLoader::loadTexture(std::string path)
	{
		std::string realPath = "data/" + path;
		textureMap[path].loadFromFile(realPath.c_str());
		textureCounter[path] = 1;
	}


	AssetLoader::Texture::Texture() : loader_(nullptr), tex_(nullptr)
	{
	}

	AssetLoader::Texture::Texture(const Texture& texture) : loader_(texture.loader_), path_(texture.path_), tex_(texture.tex_)
	{
		loader_->textureCounter[path_]++;
	}

	AssetLoader::Texture::Texture(AssetLoader* loader, std::string path, sf::Texture* tex) : loader_(loader), path_(path), tex_(tex)
	{
	}

	AssetLoader::Texture::~Texture()
	{
		if (path_ != "")
			loader_->releaseSFTexture(path_);
	}

	sf::Texture& AssetLoader::Texture::get() const
	{
		return *tex_;
	}

	AssetLoader::Texture& AssetLoader::Texture::operator=(const Texture& texture)
	{
		if (&texture == this)
			return *this;

		Texture tmp(texture);
		std::swap(loader_, tmp.loader_);
		std::swap(path_, tmp.path_);
		std::swap(tex_, tmp.tex_);

		return *this;
	}

	sf::Texture* AssetLoader::Texture::operator->() const
	{
		return tex_;
	}
}
