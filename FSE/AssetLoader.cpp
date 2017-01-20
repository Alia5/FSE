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
		if (texture_map_.count(path)) //Texture is already loaded
		{
			texture_counter_[path]++;
		}
		else
		{
			loadTexture(path);
		}
		return texture_map_[path];
	}

	AssetLoader::Texture AssetLoader::getTexture(std::string path)
	{
		return Texture(this, path, &getSFTexture(path));
	}

	void AssetLoader::releaseSFTexture(std::string path)
	{
		if (texture_counter_.size() == 0)
			return;
		if (!texture_counter_.count(path)) //Texture is not in memory
			return;
		if (texture_counter_[path] == 1)
		{
			texture_counter_.erase(path);
			texture_map_.erase(path);

			std::cout << "erasing texture: " << path << "\n";

			return;
		}

		texture_counter_[path]--;
	}

	void AssetLoader::loadTexture(std::string path)
	{
		std::string realPath = "data/" + path;
		texture_map_[path].loadFromFile(realPath.c_str());
		texture_counter_[path] = 1;
	}


	AssetLoader::Texture::Texture() : loader_(nullptr), tex_(nullptr)
	{
	}

	AssetLoader::Texture::Texture(const Texture& texture) : loader_(texture.loader_), path_(texture.path_), tex_(texture.tex_)
	{
		if (loader_ != nullptr)
			loader_->texture_counter_[path_]++;
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
