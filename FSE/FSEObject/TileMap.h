#pragma once
#include "FSEObject.h"
#include "../AssetLoader.h"
#include "../LTBL2/src/Sprite.hpp"
#include "../../LTBL2/src/LightShape.hpp"

namespace fse
{
	class TileMap : public FSEObject
	{
	public:
		TileMap();
		explicit TileMap(const sf::Vector2f& spawnPos);
		~TileMap() override;

		void spawned() override;
		void onDespawn() override;

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void drawNormals(sf::RenderTarget& target) override;
		void drawSpecular(sf::RenderTarget& target) override;
		void setPosition(const sf::Vector2f position) override;
		sf::FloatRect GetAABBs() const override;


		std::string getTexturePath() const;
		void setTexturePath(const std::string& texture_path);
		sf::Vector2i getTileSize() const;
		void setTileSize(const sf::Vector2i& tile_size);
		sf::Vector2i getMapSize() const;
		void setMapSize(const sf::Vector2i& map_size);
		std::vector<std::vector<int>> getTileIndices() const;
		void setTileIndices(const std::vector<std::vector<int>>& tile_indices);
		void setTileIndices(const std::string& csv);

		std::vector<int> getTileIndices1D() const;
		void setTileIndices1D(std::vector<int> indices);


		bool hasCollision() const;
		void setHasCollision(bool has_collision);
		bool blocksLight() const;
		void setBlocksLight(bool blocks_light);
	private:
		std::string texture_path_;
		sf::Vector2i tile_size_;
		sf::Vector2i map_size_;

		std::vector<std::vector<int>> tile_indices_;

		fse::Texture ground_sheet_texture_;
		sf::VertexArray vertices_;

		ltbl::Sprite sprite_;

		std::vector<b2Body*> tile_bodies_;
		std::vector<ltbl::LightShape*> light_shapes_;

		bool has_collision_ = true;
		bool blocks_light_ = true;

		FSE_CHAI_ENABLE(TileMap);
		FSE_CHAI_REGISTRATION_FRIEND

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND

	};

}
