#include "TileMap.h"
#include "../Application.h"
#include "../Lights/FSELightWorld.h"
#include "../ImageOutlineFinder.h"
#include "../FMath.h"

namespace fse
{
	TileMap::TileMap() : TileMap({0,0})
	{
	}

	TileMap::TileMap(const sf::Vector2f& spawnPos) : FSEObject(spawnPos)
	{

	}

	TileMap::~TileMap()
	{
	}

	void TileMap::spawned()
	{
		setZOrder(32);

		ground_sheet_texture_ = scene_->getApplication()->getAssetLoader().getTexture(texture_path_);
		ground_sheet_texture_->setSmooth(true);

		sprite_ = ltbl::Sprite(scene_->getLightWorld()->getLightSystem());

		sprite_.setTextureRect(sf::IntRect(0, 0,
			static_cast<int>(map_size_.x * tile_size_.x),
			static_cast<int>(map_size_.y * tile_size_.y)));

		sprite_.setOrigin({ map_size_.x * tile_size_.x / 2.f,
			 map_size_.y * tile_size_.y / 2.f,
		});

		sprite_.setPosition(position_ * scene_->getPixelsPerMeter());

		generate();
		has_spawned_ = true;

	}

	void TileMap::onDespawn()
	{
		for (const auto& body : tile_bodies_)
			scene_->getPhysWorld()->DestroyBody(body);
		tile_bodies_.clear();


		for (const auto& shape : light_shapes_)
			scene_->getLightWorld()->getLightSystem()->removeShape(shape);
		light_shapes_.clear();
	}

	

	void TileMap::update(float deltaTime)
	{
	}

	void TileMap::draw(sf::RenderTarget& target)
	{
		sf::RenderStates states = sf::RenderStates::Default;
		states.transform *= sprite_.getTransform();
		states.texture = &*(ground_sheet_texture_);

		target.draw(vertices_, states);
	}

	void TileMap::drawNormals(sf::RenderTarget& target)
	{
	}

	void TileMap::drawSpecular(sf::RenderTarget& target)
	{
	}

	void TileMap::setPosition(const sf::Vector2f position)
	{
		sf::Vector2f oldPos = position_;
		position_ = position;
		sprite_.setPosition(position_ * scene_->getPixelsPerMeter());




		for (const auto & body : tile_bodies_)
		{
			body->SetTransform(fse::FMath::sfVec2fTob2Vec2(
				(position - oldPos + fse::FMath::b2Vec2ToSfVec2f(body->GetTransform().p))),
				body->GetAngle());
		}

		for (const auto & lShape : light_shapes_)
		{
			lShape->setPosition((position - oldPos + lShape->getPosition()* scene_->getMetersPerPixel()) * scene_->getPixelsPerMeter());
		}

	}

	sf::FloatRect TileMap::GetAABBs() const
	{
		return sprite_.getGlobalBounds();
	}

	void TileMap::generate()
	{
		 // TODO: Clean up/split up, regenerate only what is neccessary in 3 or more funs
		vertices_.setPrimitiveType(sf::Quads);
		vertices_.resize(static_cast<size_t>(map_size_.x * map_size_.y * 4));

		for (const auto& body : tile_bodies_)
			scene_->getPhysWorld()->DestroyBody(body);
		tile_bodies_.clear();


		for (const auto& shape : light_shapes_)
			scene_->getLightWorld()->getLightSystem()->removeShape(shape);
		light_shapes_.clear();

		sf::RenderTexture temp_texture;
		temp_texture.create(tile_size_.x + 2, tile_size_.y + 2);
		sf::Sprite temp_sprite;

		for (int x = 0; x < map_size_.x; x++)
		{
			for (int y = 0; y < map_size_.y; y++)
			{
				sf::Vertex* quad = &vertices_[static_cast<size_t>((x + y * map_size_.x) * 4)];
				quad[0].position = sf::Vector2f(x * tile_size_.x, y * tile_size_.y);
				quad[1].position = sf::Vector2f((x + 1) * tile_size_.x, y * tile_size_.y);
				quad[2].position = sf::Vector2f((x + 1) * tile_size_.x, (y + 1) * tile_size_.y);
				quad[3].position = sf::Vector2f(x * tile_size_.x, (y + 1) * tile_size_.y);
				const int tiles_on_x = ground_sheet_texture_->getSize().x / tile_size_.x;
				const int tile_index = tile_indices_[y][x];
				if (tile_index == 0)
				{
					quad[0].texCoords = sf::Vector2f(ground_sheet_texture_->getSize().x, ground_sheet_texture_->getSize().y);
					quad[1].texCoords = sf::Vector2f(ground_sheet_texture_->getSize().x, ground_sheet_texture_->getSize().y);
					quad[2].texCoords = sf::Vector2f(ground_sheet_texture_->getSize().x, ground_sheet_texture_->getSize().y);
					quad[3].texCoords = sf::Vector2f(ground_sheet_texture_->getSize().x, ground_sheet_texture_->getSize().y);
				}
				else
				{
					int tu = (tile_index % tiles_on_x) - 1;
					int tv = std::floor(tile_index / tiles_on_x);

					quad[0].texCoords = sf::Vector2f(tu * tile_size_.x, tv * tile_size_.y);
					quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, tv * tile_size_.y);
					quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size_.x, (tv + 1) * tile_size_.y);
					quad[3].texCoords = sf::Vector2f(tu * tile_size_.x, (tv + 1) * tile_size_.y);


					temp_texture.clear(sf::Color::Transparent);
					temp_sprite.setTexture(*ground_sheet_texture_);
					temp_sprite.setTextureRect(sf::IntRect(tu * tile_size_.x, tv * tile_size_.y, tile_size_.x, tile_size_.y));
					temp_sprite.setPosition({ 1,1 });
					temp_texture.draw(temp_sprite);
					temp_texture.display();

					if (has_collision_ || blocks_light_)
					{
						ImageOutlineFinder finder;
						finder.findOutLines(temp_texture.getTexture().copyToImage(), 18);
						auto polys = finder.getSimplifiedPolys(0.0018f, 1);
						if (polys.empty())
						{
							polys = finder.getSimplifiedPolys(-1.f, 1);
						}

						b2Body* body = nullptr;
						if (has_collision_)
						{
							b2BodyDef testbdef;

							testbdef.type = b2_staticBody;
							testbdef.position.Set(position_.x, position_.y);
							testbdef.userData = this;

							body = scene_->getPhysWorld()->CreateBody(&testbdef);
							body->SetTransform(b2Vec2(
								x * tile_size_.x + tile_size_.x / 2.f - (map_size_.x * tile_size_.x / 2.f),
								y * tile_size_.y + tile_size_.y / 2.f - (map_size_.y * tile_size_.y / 2.f)
							) * scene_->getMetersPerPixel() + fse::FMath::sfVec2fTob2Vec2(position_), body->GetAngle());
						}

						ltbl::LightShape * light_shape = nullptr;
						for (auto& poly : polys)
						{
							b2FixtureDef fdef;
							fdef.friction = 0.5f;
							fdef.restitution = 0.001f;
							std::vector<b2Vec2> verts;

							for (auto& point : poly)
							{
								verts.emplace_back(point.x * scene_->getMetersPerPixel(), point.y * scene_->getMetersPerPixel());
							}
							if (has_collision_)
							{
								b2PolygonShape poly_shape;
								poly_shape.Set(verts.data(), verts.size());
								fdef.shape = &poly_shape;
								auto fixture = body->CreateFixture(&fdef);
							}

							if (blocks_light_)
							{
								sf::ConvexShape shape;
								shape.setPointCount(poly.size());
								int j = 0;
								for (auto& point : poly)
								{
									shape.setPoint(j, point);
									j++;
								}
								light_shape = getScene()->getLightWorld()->getLightSystem()->createLightShape(shape);
								light_shape->setPosition(fse::FMath::b2Vec2ToSfVec2f(b2Vec2(
									x* tile_size_.x + tile_size_.x / 2.f - (map_size_.x * tile_size_.x / 2.f),
									y * tile_size_.y + tile_size_.y / 2.f - (map_size_.y * tile_size_.y / 2.f)
								) * scene_->getMetersPerPixel() + fse::FMath::sfVec2fTob2Vec2(position_)) * scene_->getPixelsPerMeter());
								light_shape->setRenderLightOver(true);
								light_shape->setReceiveShadow(false);
								light_shapes_.push_back(light_shape);
							}

						}
						if (has_collision_)
						{
							tile_bodies_.push_back(body);
						}
					}
				}
			}
		}
	}

	std::string TileMap::getTexturePath() const
	{
		return texture_path_;
	}

	void TileMap::setTexturePath(const std::string& texture_path)
	{
		texture_path_ = texture_path;
	}

	sf::Vector2i TileMap::getTileSize() const
	{
		return tile_size_;
	}

	void TileMap::setTileSize(const sf::Vector2i& tile_size)
	{
		tile_size_ = tile_size;
	}

	sf::Vector2i TileMap::getMapSize() const
	{
		return map_size_;
	}

	void TileMap::setMapSize(const sf::Vector2i& map_size)
	{
		map_size_ = map_size;
	}

	std::vector<std::vector<int>> TileMap::getTileIndices() const
	{
		return tile_indices_;
	}

	void TileMap::setTileIndices(const std::vector<std::vector<int>>& tile_indices)
	{
		tile_indices_ = tile_indices;
	}

	void TileMap::setTileIndices(const std::string& csv)
	{
		tile_indices_.clear();
		tile_indices_.reserve(map_size_.y);

		std::string::const_iterator searchStart(csv.cbegin());
		std::smatch match;

		for (auto i = 0; i < map_size_.y; i++)
		{
			std::vector<int> x_vector;
			x_vector.reserve(map_size_.x);
			for (auto j = 0; j < map_size_.x; j++)
			{
				std::regex_search(searchStart, csv.cend(), match, std::regex(R"([0-9]*)"));
				x_vector.push_back(std::stol(match.str().c_str()));
				if (!match.suffix().str().empty())
					searchStart = match.suffix().first + 1;
				else
				{
					break;
				}
			}
			tile_indices_.push_back(x_vector);
		}
	}

	std::vector<int> TileMap::getTileIndices1D() const
	{
		std::vector<int> result;
		result.reserve(map_size_.x * map_size_.y);
		for (auto & x_vec : tile_indices_)
		{
			for (auto & val : x_vec)
			{
				result.push_back(val);
			}
		}
		return result;
	}

	void TileMap::setTileIndices1D(std::vector<int> indices)
	{
		tile_indices_.clear();
		for (int k = 0, i = 0; i < map_size_.y; i++)
		{
			std::vector<int> x_vector;
			x_vector.reserve(map_size_.x);
			for (int j = 0; j < map_size_.x; j++)
			{
				x_vector.push_back(indices[k]);
				k++;
			}
			tile_indices_.push_back(x_vector);
		}
		if (has_spawned_)
			generate();
	}

	bool TileMap::hasCollision() const
	{
		return has_collision_;
	}

	void TileMap::setHasCollision(bool has_collision)
	{
		has_collision_ = has_collision;
		if (has_spawned_)
			generate();
	}

	bool TileMap::blocksLight() const
	{
		return blocks_light_;
	}

	void TileMap::setBlocksLight(bool blocks_light)
	{
		blocks_light_ = blocks_light;
		if (has_spawned_)
			generate();
	}


	FSE_CHAI_REGISTER(TileMap)
	{
		RegisterChaiUserTypeFromRTTR<TileMap>(chai);
		chai.add(chaiscript::base_class<fse::FSEObject, TileMap>());
		chai.add(chaiscript::constructor<TileMap()>(), "TileMap");
		chai.add(chaiscript::fun(&TileMap::getTexturePath), "getTexturePath");
		chai.add(chaiscript::fun(&TileMap::setTexturePath), "setTexturePath");
		chai.add(chaiscript::fun(&TileMap::getTileSize), "getTileSize");
		chai.add(chaiscript::fun(&TileMap::setTileSize), "setTileSize");
		chai.add(chaiscript::fun(&TileMap::getMapSize), "getMapSize");
		chai.add(chaiscript::fun(&TileMap::setMapSize), "setMapSize");
		chai.add(chaiscript::fun(&TileMap::getTileIndices), "getTileIndices");
		chai.add(chaiscript::fun(static_cast<void(TileMap::*)(const std::vector<std::vector<int>>&)>(&TileMap::setTileIndices)), "setTileIndices");
		chai.add(chaiscript::fun(static_cast<void(TileMap::*)(const std::string&)>(&TileMap::setTileIndices)), "setTileIndices");
		chai.add(chaiscript::fun(&TileMap::getTileIndices1D), "getTileIndices1D");
		chai.add(chaiscript::fun(&TileMap::setTileIndices1D), "setTileIndices1D");
		chai.add(chaiscript::fun(&TileMap::hasCollision), "hasCollision");
		chai.add(chaiscript::fun(&TileMap::setHasCollision), "setHasCollision");
		chai.add(chaiscript::fun(&TileMap::blocksLight), "blocksLight");
		chai.add(chaiscript::fun(&TileMap::setBlocksLight), "setBlocksLight");
		chai.add(chaiscript::fun(&TileMap::generate), "generate");
	}

}

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;
	registration::class_<TileMap>("fse::TileMap")
		.constructor<>()
		(
			policy::ctor::as_std_shared_ptr
		)
		.constructor<const sf::Vector2f&>()
		(
			policy::ctor::as_std_shared_ptr,
			parameter_names("spawn position")
		)
		.property("texture_path_", &TileMap::texture_path_)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("tile_size_", &TileMap::tile_size_)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("map_size_", &TileMap::map_size_)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("tile_indices_", &TileMap::getTileIndices1D, &TileMap::setTileIndices1D)
		(
			metadata("BEFORE_SPAWN", true)
		)
		/*.property("has_collision_", &TileMap::has_collision_)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("blocks_light_", &TileMap::blocks_light_)
		(
			metadata("BEFORE_SPAWN", true)
		)*/
		.property("has_collision_", &TileMap::hasCollision, &TileMap::setHasCollision)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("blocks_light_", &TileMap::blocksLight, &TileMap::setBlocksLight)
		(
			metadata("BEFORE_SPAWN", true)
		)
		.property("tile_bodies_", &TileMap::tile_bodies_)
		(
			metadata("NO_SERIALIZE", true)
		)
		.property("light_shapes_", &TileMap::light_shapes_)
		(
			metadata("NO_SERIALIZE", true)
		).method("generate", &TileMap::generate)

	//TODO:
	;
}
