#include "LetThereBeLight.hpp"

namespace ltbl
{

namespace priv
{

sf::Vector2f rectCenter(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f);
}

bool rectContains(const sf::FloatRect& rect, const sf::FloatRect& other)
{
	if (other.left < rect.left)
		return false;
	if (other.top < rect.top)
		return false;
	if (other.left + other.width > rect.left + rect.width)
		return false;
	if (other.top + other.height > rect.top + rect.height)
		return false;
	return true;
}

sf::Vector2f rectHalfDims(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.width * 0.5f, rect.height * 0.5f);
}

sf::Vector2f rectDims(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.width, rect.height);
}

sf::Vector2f rectLowerBound(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left, rect.top);
}

sf::Vector2f rectUpperBound(const sf::FloatRect& rect)
{
	return sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
}

sf::FloatRect rectFromBounds(const sf::Vector2f& lowerBound, const sf::Vector2f& upperBound)
{
	return sf::FloatRect(lowerBound.x, lowerBound.y, upperBound.x - lowerBound.x, upperBound.y - lowerBound.y);
}

sf::FloatRect rectRecenter(const sf::FloatRect& rect, const sf::Vector2f& center)
{
	sf::Vector2f dims = rectDims(rect);
	return sf::FloatRect(center - dims * 0.5f, dims);
}

float vectorMagnitude(const sf::Vector2f& vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float vectorMagnitudeSquared(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

sf::Vector2f vectorNormalize(const sf::Vector2f& vector)
{
	float magnitude = vectorMagnitude(vector);
	if (magnitude == 0.0f)
		return sf::Vector2f(1.0f, 0.0f);
	float distInv = 1.0f / magnitude;
	return sf::Vector2f(vector.x * distInv, vector.y * distInv);
}

float vectorDot(const sf::Vector2f& left, const sf::Vector2f& right)
{
	return left.x * right.x + left.y * right.y;
}

float vectorProject(const sf::Vector2f& left, const sf::Vector2f& right)
{
	assert(vectorMagnitudeSquared(right) != 0.0f);
	return vectorDot(left, right) / vectorMagnitudeSquared(right);
}

bool shapeIntersection(const sf::ConvexShape& left, const sf::ConvexShape& right)
{
	std::vector<sf::Vector2f> transformedLeft(left.getPointCount());
	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		transformedLeft[i] = left.getTransform().transformPoint(left.getPoint(i));
	}

	std::vector<sf::Vector2f> transformedRight(right.getPointCount());
	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		transformedRight[i] = right.getTransform().transformPoint(right.getPoint(i));
	}

	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		sf::Vector2f point = transformedLeft[i];
		sf::Vector2f nextPoint = (i == left.getPointCount() - 1u) ? transformedLeft[0] : transformedLeft[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedRight[0].x * edgePerpendicular.x + transformedRight[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < right.getPointCount(); j++)
		{
			float proj = (transformedRight[j].x * edgePerpendicular.x + transformedRight[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}

	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		sf::Vector2f point = transformedRight[i];
		sf::Vector2f nextPoint = (i == right.getPointCount() - 1u) ? transformedRight[0] : transformedRight[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedLeft[0].x * edgePerpendicular.x + transformedLeft[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < left.getPointCount(); j++)
		{
			float proj = (transformedLeft[j].x * edgePerpendicular.x + transformedLeft[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}

	return true;
}

sf::ConvexShape shapeFromRect(const sf::FloatRect& rect)
{
	sf::ConvexShape shape(4);
	sf::Vector2f halfDims = sf::Vector2f(rect.width * 0.5f, rect.height * 0.5f);
	shape.setPoint(0, sf::Vector2f(-halfDims.x, -halfDims.y));
	shape.setPoint(1, sf::Vector2f(halfDims.x, -halfDims.y));
	shape.setPoint(2, sf::Vector2f(halfDims.x, halfDims.y));
	shape.setPoint(3, sf::Vector2f(-halfDims.x, halfDims.y));
	shape.setPosition(sf::Vector2f(rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f));
	return shape;
}

bool rayIntersect(const sf::Vector2f& as, const sf::Vector2f& ad, const sf::Vector2f& bs, const sf::Vector2f& bd, sf::Vector2f& intersection)
{
	float dx = bs.x - as.x;
	float dy = bs.y - as.y;
	float det = bd.x * ad.y - bd.y * ad.x;
	if (det == 0.0f)
		return false;
	float u = (dy * bd.x - dx * bd.y) / det;
	if (u < 0.0f)
		return false;
	float v = (dy * ad.x - dx * ad.y) / det;
	if (v < 0.0f)
		return false;
	intersection = as + ad * u;
	return true;
}

QuadtreeOccupant::QuadtreeOccupant()
	: mAwake(true)
	, mAABBChanged(false)
{
}

void QuadtreeOccupant::setAwake(bool awake)
{
	mAwake = awake;
}

bool QuadtreeOccupant::isAwake() const
{
	return mAwake;
}

void QuadtreeOccupant::toggleAwake()
{
	mAwake = !mAwake;
}

void QuadtreeOccupant::quadtreeAABBChanged()
{
	mAABBChanged = true;
}

Quadtree::Quadtree(const sf::FloatRect& region, unsigned int maxOccupants, unsigned int maxLevels, Quadtree* parent, unsigned int level, unsigned int type)
	: mRegion(region)
	, mMaxOccupants(maxOccupants)
	, mMaxLevels(maxLevels)
	, mParent(parent)
	, mLevel(level)
	, mType(type)
	, mOccupants()
	, mOutsideOccupants()
	, mChildren()
{
}

void Quadtree::create(const sf::FloatRect& region, unsigned int maxOccupants, unsigned int maxLevels, Quadtree* parent, unsigned int level, unsigned int type)
{
	clear();

	mRegion = region;
	mMaxOccupants = maxOccupants;
	mMaxLevels = maxLevels;
	mParent = parent;
	mLevel = level;
	mType = type;
}

void Quadtree::addOccupant(QuadtreeOccupant* oc)
{
	if (oc != nullptr)
	{
		if (mRegion.intersects(oc->getAABB()))
		{
			if (hasChildren())
			{
				bool handled = false;
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (!handled && mChildren[i]->mRegion.intersects(oc->getAABB()))
					{
						mChildren[i]->addOccupant(oc);
						handled = true;
					}
				}
				if (handled)
				{
					return;
				}
			}
			else
			{
				mOccupants.insert(oc);

				if (mOccupants.size() >= mMaxOccupants && mLevel < mMaxLevels)
				{
					split();
				}

				return;
			}
		}

		if (mParent != nullptr)
		{
			mParent->addOccupant(oc);
		}
		else
		{
			mOutsideOccupants.insert(oc);
		}
	}
}

bool Quadtree::removeOccupant(QuadtreeOccupant* oc)
{
	if (hasChildren())
	{
		bool removed = false;
		for (std::size_t i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i]->mRegion.intersects(oc->getAABB()))
			{
				if (mChildren[i]->removeOccupant(oc))
				{
					removed = true;
				}
			}
		}
		if (removed)
		{
			if (getNumOccupantsBelow() < mMaxOccupants)
			{
				unsplit();
			}
			return true;
		}
	}
	else
	{
		auto itr = mOccupants.find(oc);
		if (itr != mOccupants.end())
		{
			mOccupants.erase(itr);
			return true;
		}
	}

	if (mOutsideOccupants.size() > 0)
	{
		auto itr = mOutsideOccupants.find(oc);
		if (itr != mOutsideOccupants.end())
		{
			mOutsideOccupants.erase(itr);
			return true;
		}
	}

	return false;
}

bool Quadtree::update()
{
	bool moved = false;
	if (hasChildren())
	{
		for (std::size_t i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i]->update())
			{
				moved = true;
			}
		}

		if (mOutsideOccupants.size() > 0)
		{
			for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); )
			{
				bool erase = false;
				if ((*itr) != nullptr)
				{
					if ((*itr)->mAABBChanged && (*itr)->isAwake())
					{
						(*itr)->mAABBChanged = false;
						if (mRegion.intersects((*itr)->getAABB()))
						{
							addOccupant(*itr);
							erase = true;
						}
					}
				}
				else
				{
					erase = true;
				}
				if (erase)
				{
					itr = mOutsideOccupants.erase(itr);
				}
				else
				{
					++itr;
				}
			}
		}

		if (moved)
		{
			if (getNumOccupantsBelow() < mMaxOccupants)
			{
				unsplit();
			}
		}
	}
	else
	{
		for (auto itr = mOccupants.begin(); itr != mOccupants.end(); )
		{
			bool erase = false;
			if ((*itr) != nullptr)
			{
				if ((*itr)->mAABBChanged && (*itr)->isAwake())
				{
					(*itr)->mAABBChanged = false;
					if (!mRegion.intersects((*itr)->getAABB()))
					{
						if (mParent != nullptr)
						{
							mParent->addOccupant(*itr);
						}
						else
						{
							mOutsideOccupants.insert(*itr);
						}
						erase = true;
					}
				}
			}
			else
			{
				erase = true;
			}
			if (erase)
			{
				itr = mOccupants.erase(itr);
				moved = true;
			}
			else
			{
				++itr;
			}
		}
	}
	return moved;
}

void Quadtree::clear()
{
	if (hasChildren())
	{
		for (std::size_t i = 0; i < mChildren.size(); i++)
		{
			mChildren[i]->clear();
			delete mChildren[i];
			mChildren[i] = nullptr;
		}
		mChildren.clear();
	}

	mOccupants.clear();
	mOutsideOccupants.clear();

	mParent = nullptr;
}

void Quadtree::query(const sf::FloatRect& area, std::vector<QuadtreeOccupant*>& occupants)
{
	for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
	{
		if ((*itr) != nullptr && (*itr)->isAwake() && area.intersects((*itr)->getAABB()))
		{
			occupants.push_back(*itr);
		}
	}

	std::list<Quadtree*> open;
	open.push_back(this);
	while (!open.empty())
	{
		Quadtree* current = open.back();
		open.pop_back();
		if (area.intersects(current->mRegion))
		{
			if (current->hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (current->mChildren[i]->getNumOccupantsBelow() > 0)
					{
						open.push_back(current->mChildren[i]);
					}
				}
			}
			else
			{
				for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
				{
					if ((*itr) != nullptr && (*itr)->isAwake() && area.intersects((*itr)->getAABB()))
					{
						occupants.push_back(*itr);
					}
				}
			}
		}
	}
}

void Quadtree::query(const sf::Vector2f& point, std::vector<QuadtreeOccupant*>& occupants)
{
	for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
	{
		if ((*itr) != nullptr && (*itr)->isAwake() && (*itr)->getAABB().contains(point))
		{
			occupants.push_back(*itr);
		}
	}

	std::list<Quadtree*> open;
	open.push_back(this);
	while (!open.empty())
	{
		Quadtree* current = open.back();
		open.pop_back();
		if (current->mRegion.contains(point))
		{
			if (current->hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (current->mChildren[i]->getNumOccupantsBelow() > 0)
					{
						open.push_back(current->mChildren[i]);
					}
				}
			}
			else
			{
				for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
				{
					if ((*itr) != nullptr && (*itr)->isAwake() && (*itr)->getAABB().contains(point))
					{
						occupants.push_back(*itr);
					}
				}
			}
		}
	}
}

void Quadtree::query(const sf::ConvexShape& shape, std::vector<QuadtreeOccupant*>& occupants)
{
	for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
	{
		if ((*itr) != nullptr && (*itr)->isAwake() && shapeIntersection(shape, shapeFromRect((*itr)->getAABB())))
		{
			occupants.push_back(*itr);
		}
	}

	std::list<Quadtree*> open;
	open.push_back(this);
	while (!open.empty())
	{
		Quadtree* current = open.back();
		open.pop_back();
		if (shapeIntersection(shape, shapeFromRect(current->mRegion)))
		{
			if (current->hasChildren())
			{
				for (std::size_t i = 0; i < mChildren.size(); i++)
				{
					if (current->mChildren[i]->getNumOccupantsBelow() > 0)
					{
						open.push_back(current->mChildren[i]);
					}
				}
			}
			else
			{
				for (auto itr = current->mOccupants.begin(); itr != current->mOccupants.end(); itr++)
				{
					if ((*itr) != nullptr && (*itr)->isAwake() && shapeIntersection(shape, shapeFromRect((*itr)->getAABB())))
					{
						occupants.push_back(*itr);
					}
				}
			}
		}
	}
}

void Quadtree::split()
{
	mChildren.clear();

	sf::Vector2f lower = { mRegion.left, mRegion.top };
	sf::Vector2f size = { mRegion.width * 0.5f, mRegion.height * 0.5f };

	for (std::size_t i = 0; i < 4; i++)
	{
		sf::FloatRect rect(lower.x, lower.y, size.x, size.y);
		switch (i)
		{
		case 1: rect.left += size.x; break;
		case 3: rect.left += size.x;
		case 2: rect.top += size.y; break;
		default: break;
		}
		mChildren.push_back(new Quadtree(rect, mMaxOccupants, mMaxLevels, this, mLevel + 1, i + 1));
	}

	for (auto itr = mOccupants.begin(); itr != mOccupants.end(); itr++)
	{
		if ((*itr) != nullptr)
		{
			addOccupant(*itr);
		}
	}

	mOccupants.clear();
}

void Quadtree::unsplit()
{
	for (std::size_t i = 0; i < mChildren.size(); i++)
	{
		if (mChildren[i] != nullptr)
		{
			for (auto itr = mChildren[i]->mOccupants.begin(); itr != mChildren[i]->mOccupants.end(); itr++)
			{
				if ((*itr) != nullptr)
				{
					mOccupants.insert(*itr);
				}
			}
			mChildren[i]->clear();

			delete mChildren[i];
			mChildren[i] = nullptr;
		}
	}
	mChildren.clear();
}

unsigned int Quadtree::getNumOccupantsBelow() const
{
	if (hasChildren())
	{
		unsigned int sum = 0;
		for (std::size_t i = 0; i < mChildren.size(); i++)
		{
			sum += mChildren[i]->getNumOccupantsBelow();
		}
		return sum;
	}
	else
	{
		return mOccupants.size();
	}
}

bool Quadtree::hasChildren() const
{
	return mChildren.size() > 0;
}

void Quadtree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (hasChildren())
	{
		for (std::size_t i = 0; i < mChildren.size(); i++)
		{
			target.draw(*mChildren[i], states);
		}
	}
	else
	{
		sf::Color color;
		switch (mType)
		{
		case 1: color = sf::Color::Red; break;
		case 2: color = sf::Color::Green; break;
		case 3: color = sf::Color::Blue; break;
		case 4: color = sf::Color::Yellow; break;
		default: color = sf::Color::Magenta; break;
		}
		for (auto itr = mOccupants.begin(); itr != mOccupants.end(); itr++)
		{
			if ((*itr) != nullptr)
			{
				sf::FloatRect box = (*itr)->getAABB();
				sf::RectangleShape oc({ box.width, box.height });
				oc.setPosition({ box.left, box.top });
				oc.setFillColor(color);
				target.draw(oc, states);
			}
		}

		sf::RectangleShape shape({ mRegion.width, mRegion.height });
		shape.setPosition({ mRegion.left, mRegion.top });
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(1.f);
		target.draw(shape, states);
	}

	for (auto itr = mOutsideOccupants.begin(); itr != mOutsideOccupants.end(); itr++)
	{
		if ((*itr) != nullptr)
		{
			sf::FloatRect box = (*itr)->getAABB();
			sf::RectangleShape oc({ box.width, box.height });
			oc.setPosition({ box.left, box.top });
			oc.setFillColor(sf::Color::Cyan);
			target.draw(oc, states);
		}
	}
}

sf::ConvexShape Quadtree::shapeFromRect(const sf::FloatRect& rect)
{
	sf::ConvexShape shape(4);
	shape.setPoint(0, { 0.f , 0.f });
	shape.setPoint(1, { rect.width, 0.f });
	shape.setPoint(2, { rect.width, rect.height });
	shape.setPoint(3, { 0.f, rect.height });
	shape.setPosition(rect.left, rect.top);
	return shape;
}

bool Quadtree::shapeIntersection(const sf::ConvexShape& left, const sf::ConvexShape& right)
{
	std::vector<sf::Vector2f> transformedLeft(left.getPointCount());
	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		transformedLeft[i] = left.getTransform().transformPoint(left.getPoint(i));
	}

	std::vector<sf::Vector2f> transformedRight(right.getPointCount());
	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		transformedRight[i] = right.getTransform().transformPoint(right.getPoint(i));
	}

	for (unsigned i = 0; i < left.getPointCount(); i++)
	{
		sf::Vector2f point = transformedLeft[i];
		sf::Vector2f nextPoint = (i == left.getPointCount() - 1u) ? transformedLeft[0] : transformedLeft[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedRight[0].x * edgePerpendicular.x + transformedRight[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < right.getPointCount(); j++)
		{
			float proj = (transformedRight[j].x * edgePerpendicular.x + transformedRight[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}

	for (unsigned i = 0; i < right.getPointCount(); i++)
	{
		sf::Vector2f point = transformedRight[i];
		sf::Vector2f nextPoint = (i == right.getPointCount() - 1u) ? transformedRight[0] : transformedRight[i + 1];
		sf::Vector2f edge = nextPoint - point;
		sf::Vector2f edgePerpendicular = sf::Vector2f(edge.y, -edge.x);
		float magnitude = edgePerpendicular.x * edgePerpendicular.x + edgePerpendicular.y * edgePerpendicular.y;
		float pointProj = (point.x * edgePerpendicular.x + point.y * edgePerpendicular.y) / magnitude;
		float minRightProj = (transformedLeft[0].x * edgePerpendicular.x + transformedLeft[0].y * edgePerpendicular.y) / magnitude;

		for (unsigned j = 1; j < left.getPointCount(); j++)
		{
			float proj = (transformedLeft[j].x * edgePerpendicular.x + transformedLeft[j].y * edgePerpendicular.y) / magnitude;
			minRightProj = std::min(minRightProj, proj);
		}

		if (minRightProj > pointProj)
		{
			return false;
		}
	}
	return true;
}

BaseLight::BaseLight()
	: mTurnedOn(true)
{
}

void BaseLight::setTurnedOn(bool turnedOn)
{
	mTurnedOn = turnedOn;
}

bool BaseLight::isTurnedOn() const
{
	return mTurnedOn;
}

void BaseLight::toggleTurnedOn()
{
	mTurnedOn = !mTurnedOn;
}

void BaseLight::unmaskWithPenumbras(sf::RenderTexture& renderTexture, sf::BlendMode blendMode, sf::Shader& unshadowShader, const std::vector<Penumbra>& penumbras, float shadowExtension)
{
	sf::VertexArray vertexArray;
	vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);
	vertexArray.resize(3);

	sf::RenderStates states;
	states.blendMode = blendMode;
	states.shader = &unshadowShader;

	unsigned int penumbrasCount = penumbras.size();
	for (unsigned int i = 0; i < penumbrasCount; i++)
	{
		unshadowShader.setUniform("lightBrightness", penumbras[i]._lightBrightness);
		unshadowShader.setUniform("darkBrightness", penumbras[i]._darkBrightness);
		vertexArray[0].position = penumbras[i]._source;
		vertexArray[1].position = penumbras[i]._source + priv::vectorNormalize(penumbras[i]._lightEdge) * shadowExtension;
		vertexArray[2].position = penumbras[i]._source + priv::vectorNormalize(penumbras[i]._darkEdge) * shadowExtension;
		vertexArray[0].texCoords = sf::Vector2f(0.0f, 1.0f);
		vertexArray[1].texCoords = sf::Vector2f(1.0f, 0.0f);
		vertexArray[2].texCoords = sf::Vector2f(0.0f, 0.0f);
		renderTexture.draw(vertexArray, states);
	}
}

} // namespace priv

LightShape::LightShape()
	: priv::QuadtreeOccupant()
	, priv::BaseLight()
	, sf::Drawable()
	, mShape()
	, mRenderLightOver(true)
{
}

void LightShape::setPointCount(unsigned int pointCount)
{
	mShape.setPointCount(pointCount);
	quadtreeAABBChanged();
}

unsigned int LightShape::getPointCount() const
{
	return mShape.getPointCount();
}

void LightShape::setPoint(unsigned int index, const sf::Vector2f& point)
{
	mShape.setPoint(index, point);
	quadtreeAABBChanged();
}

sf::Vector2f LightShape::getPoint(unsigned int index) const
{
	return mShape.getPoint(index);
}

void LightShape::setColor(const sf::Color& color)
{
	mShape.setFillColor(color);
}

const sf::Color& LightShape::getColor() const
{
	return mShape.getFillColor();
}

const sf::Transform& LightShape::getTransform() const
{
	return mShape.getTransform();
}

void LightShape::setPosition(const sf::Vector2f& position)
{
	mShape.setPosition(position);
	quadtreeAABBChanged();
}

void LightShape::setPosition(float x, float y)
{
	mShape.setPosition(x, y);
	quadtreeAABBChanged();
}

void LightShape::move(const sf::Vector2f& movement)
{
	mShape.move(movement);
	quadtreeAABBChanged();
}

void LightShape::move(float x, float y)
{
	mShape.move(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightShape::getPosition() const
{
	return mShape.getPosition();
}

void LightShape::setRotation(float angle)
{
	mShape.setRotation(angle);
	quadtreeAABBChanged();
}

void LightShape::rotate(float angle)
{
	mShape.rotate(angle);
	quadtreeAABBChanged();
}

float LightShape::getRotation() const
{
	return mShape.getRotation();
}

void LightShape::setScale(const sf::Vector2f& scale)
{
	mShape.setScale(scale);
	quadtreeAABBChanged();
}

void LightShape::setScale(float x, float y)
{
	mShape.setScale(x, y);
	quadtreeAABBChanged();
}

void LightShape::scale(const sf::Vector2f& scale)
{
	mShape.scale(scale);
	quadtreeAABBChanged();
}

void LightShape::scale(float x, float y)
{
	mShape.scale(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightShape::getScale() const
{
	return mShape.getScale();
}

void LightShape::setOrigin(const sf::Vector2f& origin)
{
	mShape.setOrigin(origin);
	quadtreeAABBChanged();
}

void LightShape::setOrigin(float x, float y)
{
	mShape.setOrigin(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightShape::getOrigin() const
{
	return mShape.getOrigin();
}

void LightShape::setRenderLightOver(bool renderLightOver)
{
	mRenderLightOver = renderLightOver;
}

bool LightShape::renderLightOver() const
{
	return mRenderLightOver;
}

sf::FloatRect LightShape::getAABB() const
{
	return mShape.getGlobalBounds();
}

void LightShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}

LightDirectionEmission::LightDirectionEmission()
	: BaseLight()
	, mShape()
	, mCastDirection(0.f, 1.f)
	, mCastAngle(90.f)
	, mSourceRadius(5.0f)
	, mSourceDistance(100.0f)
{
}

void LightDirectionEmission::setColor(const sf::Color& color)
{
	mShape.setFillColor(color);
}

const sf::Color& LightDirectionEmission::getColor() const
{
	return mShape.getFillColor();
}

void LightDirectionEmission::render(const sf::View& view, sf::RenderTexture& lightTempTexture, sf::RenderTexture& antumbraTempTexture, sf::Shader& unshadowShader, const std::vector<priv::QuadtreeOccupant*>& shapes, float shadowExtension)
{
	lightTempTexture.setView(view);
	lightTempTexture.clear(sf::Color::White);

	// Mask off light shape (over-masking - mask too much, reveal penumbra/antumbra afterwards)
	unsigned int shapesCount = shapes.size();
	for (unsigned int i = 0; i < shapesCount; ++i)
	{
		LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
		if (pLightShape != nullptr && pLightShape->isTurnedOn())
		{
			// Get boundaries
			std::vector<priv::Penumbra> penumbras;
			std::vector<int> innerBoundaryIndices;
			std::vector<int> outerBoundaryIndices;
			std::vector<sf::Vector2f> innerBoundaryVectors;
			std::vector<sf::Vector2f> outerBoundaryVectors;

			getPenumbrasDirection(penumbras, innerBoundaryIndices, innerBoundaryVectors, outerBoundaryIndices, outerBoundaryVectors, *pLightShape);

			if (innerBoundaryIndices.size() != 2 || outerBoundaryIndices.size() != 2)
			{
				continue;
			}

			antumbraTempTexture.clear(sf::Color::White);
			antumbraTempTexture.setView(view);

			float maxDist = 0.0f;
			for (unsigned j = 0; j < pLightShape->getPointCount(); j++)
			{
				maxDist = std::max(maxDist, priv::vectorMagnitude(view.getCenter() - pLightShape->getTransform().transformPoint(pLightShape->getPoint(j))));
			}
			float totalShadowExtension = shadowExtension + maxDist;

			sf::ConvexShape maskShape;
			maskShape.setPointCount(4);
			maskShape.setPoint(0, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0])));
			maskShape.setPoint(1, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1])));
			maskShape.setPoint(2, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1])) + priv::vectorNormalize(innerBoundaryVectors[1]) * totalShadowExtension);
			maskShape.setPoint(3, pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0])) + priv::vectorNormalize(innerBoundaryVectors[0]) * totalShadowExtension);
			maskShape.setFillColor(sf::Color::Black);
			antumbraTempTexture.draw(maskShape);

			unmaskWithPenumbras(antumbraTempTexture, sf::BlendAdd, unshadowShader, penumbras, totalShadowExtension);

			antumbraTempTexture.display();

			lightTempTexture.setView(lightTempTexture.getDefaultView());
			lightTempTexture.draw(sf::Sprite(antumbraTempTexture.getTexture()), sf::BlendMultiply);
			lightTempTexture.setView(view);
		}
	}

	for (unsigned int i = 0; i < shapesCount; i++)
	{
		LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
		if (pLightShape->renderLightOver() && pLightShape->isTurnedOn())
		{
			pLightShape->setColor(sf::Color::White);

			lightTempTexture.draw(*pLightShape);
		}
	}

	lightTempTexture.setView(lightTempTexture.getDefaultView());
	mShape.setSize(lightTempTexture.getView().getSize());
	lightTempTexture.draw(mShape, sf::BlendMultiply);

	lightTempTexture.display();
}

void LightDirectionEmission::setCastDirection(const sf::Vector2f& castDirection)
{
	mCastDirection = priv::vectorNormalize(castDirection);
	mCastAngle = priv::_radToDeg * std::atan2(mCastDirection.y, mCastDirection.x);
}

const sf::Vector2f& LightDirectionEmission::getCastDirection() const
{
	return mCastDirection;
}

void LightDirectionEmission::setCastAngle(float angle)
{
	mCastAngle = angle;
	float radAngle = angle * priv::_degToRad;
	mCastDirection.x = std::cos(radAngle);
	mCastDirection.y = std::sin(radAngle);
}

float LightDirectionEmission::getCastAngle() const
{
	return mCastAngle;
}

void LightDirectionEmission::setSourceRadius(float radius)
{
	mSourceRadius = radius;
}

float LightDirectionEmission::getSourceRadius() const
{
	return mSourceRadius;
}

void LightDirectionEmission::setSourceDistance(float distance)
{
	mSourceDistance = distance;
}

float LightDirectionEmission::getSourceDistance() const
{
	return mSourceDistance;
}

void LightDirectionEmission::getPenumbrasDirection(std::vector<priv::Penumbra>& penumbras, std::vector<int>& innerBoundaryIndices, std::vector<sf::Vector2f>& innerBoundaryVectors, std::vector<int>& outerBoundaryIndices, std::vector<sf::Vector2f>& outerBoundaryVectors, const LightShape& shape)
{
	const int numPoints = shape.getPointCount();

	innerBoundaryIndices.reserve(2);
	innerBoundaryVectors.reserve(2);
	penumbras.reserve(2);

	std::vector<bool> bothEdgesBoundaryWindings;
	bothEdgesBoundaryWindings.reserve(2);

	// Calculate front and back facing sides
	std::vector<bool> facingFrontBothEdges;
	facingFrontBothEdges.reserve(numPoints);

	std::vector<bool> facingFrontOneEdge;
	facingFrontOneEdge.reserve(numPoints);

	for (int i = 0; i < numPoints; i++)
	{
		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));
		sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint((i < numPoints - 1) ? i + 1 : 0));
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
		sf::Vector2f firstNextEdgeRay = nextPoint - (point - mCastDirection * mSourceDistance - perpendicularOffset);
		sf::Vector2f secondNextEdgeRay = nextPoint - (point - mCastDirection * mSourceDistance + perpendicularOffset);
		sf::Vector2f pointToNextPoint = nextPoint - point;
		sf::Vector2f normal = priv::vectorNormalize(sf::Vector2f(-pointToNextPoint.y, pointToNextPoint.x));

		// Front facing, mark it
		facingFrontBothEdges.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f && priv::vectorDot(secondEdgeRay, normal) > 0.0f) || (priv::vectorDot(firstNextEdgeRay, normal) > 0.0f && priv::vectorDot(secondNextEdgeRay, normal) > 0.0f));
		facingFrontOneEdge.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f || priv::vectorDot(secondEdgeRay, normal) > 0.0f) || priv::vectorDot(firstNextEdgeRay, normal) > 0.0f || priv::vectorDot(secondNextEdgeRay, normal) > 0.0f);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontBothEdges[i] != facingFrontBothEdges[i - 1])
		{
			innerBoundaryIndices.push_back(i);
			bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[i]);
		}
	}

	// Check looping indices separately
	if (facingFrontBothEdges[0] != facingFrontBothEdges[numPoints - 1])
	{
		innerBoundaryIndices.push_back(0);
		bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[0]);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontOneEdge[i] != facingFrontOneEdge[i - 1])
		{
			outerBoundaryIndices.push_back(i);
		}
	}

	// Check looping indices separately
	if (facingFrontOneEdge[0] != facingFrontOneEdge[numPoints - 1])
	{
		outerBoundaryIndices.push_back(0);
	}

	for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++)
	{
		int penumbraIndex = innerBoundaryIndices[bi];
		bool winding = bothEdgesBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);

		// Add boundary vector
		innerBoundaryVectors.push_back(winding ? secondEdgeRay : firstEdgeRay);
		sf::Vector2f outerBoundaryVector = winding ? firstEdgeRay : secondEdgeRay;
		outerBoundaryVectors.push_back(outerBoundaryVector);

		// Add penumbras
		bool hasPrevPenumbra = false;

		sf::Vector2f prevPenumbraLightEdgeVector;

		float prevBrightness = 1.0f;

		while (penumbraIndex != -1)
		{
			sf::Vector2f nextPoint;
			int nextPointIndex;

			if (penumbraIndex < numPoints - 1)
			{
				nextPointIndex = penumbraIndex + 1;
				nextPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex + 1));
			}
			else
			{
				nextPointIndex = 0;
				nextPoint = shape.getTransform().transformPoint(shape.getPoint(0));
			}

			sf::Vector2f pointToNextPoint = nextPoint - point;

			sf::Vector2f prevPoint;
			int prevPointIndex;

			if (penumbraIndex > 0)
			{
				prevPointIndex = penumbraIndex - 1;
				prevPoint = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex - 1));
			}
			else
			{
				prevPointIndex = numPoints - 1;
				prevPoint = shape.getTransform().transformPoint(shape.getPoint(numPoints - 1));
			}

			sf::Vector2f pointToPrevPoint = prevPoint - point;

			priv::Penumbra penumbra;
			penumbra._source = point;

			if (!winding)
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToNextPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle)
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToNextPoint;
					penumbraIndex = nextPointIndex;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
					firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
					secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
					outerBoundaryVector = secondEdgeRay;
				}
				else
				{
					penumbra._darkBrightness = 0.0f;
					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}
					hasPrevPenumbra = false;
					penumbraIndex = -1;
				}
			}
			else // Winding = true
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToPrevPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle)
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToPrevPoint;
					penumbraIndex = prevPointIndex;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					perpendicularOffset = priv::vectorNormalize({ -mCastDirection.y, mCastDirection.x }) * mSourceRadius;
					firstEdgeRay = point - (point - mCastDirection * mSourceDistance + perpendicularOffset);
					secondEdgeRay = point - (point - mCastDirection * mSourceDistance - perpendicularOffset);
					outerBoundaryVector = firstEdgeRay;
				}
				else
				{
					penumbra._darkBrightness = 0.0f;
					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}
					hasPrevPenumbra = false;
					penumbraIndex = -1;
				}
			}

			penumbras.push_back(penumbra);
		}
	}
}

LightPointEmission::LightPointEmission()
	: BaseLight()
	, mSprite()
	, mLocalCastCenter()
	, mSourceRadius(8.0f)
	, mShadowOverExtendMultiplier(1.4f)
{
}

void LightPointEmission::setTexture(sf::Texture& texture)
{
	mSprite.setTexture(texture);
	quadtreeAABBChanged();
}

const sf::Texture* LightPointEmission::getTexture() const
{
	return mSprite.getTexture();
}

void LightPointEmission::setTextureRect(const sf::IntRect& rect)
{
	mSprite.setTextureRect(rect);
	quadtreeAABBChanged();
}

const sf::IntRect& LightPointEmission::getTextureRect() const
{
	return mSprite.getTextureRect();
}

void LightPointEmission::setColor(const sf::Color& color)
{
	mSprite.setColor(color);
}

const sf::Color& LightPointEmission::getColor() const
{
	return mSprite.getColor();
}

const sf::Transform& LightPointEmission::getTransform() const
{
	return mSprite.getTransform();
}

void LightPointEmission::setPosition(const sf::Vector2f& position)
{
	mSprite.setPosition(position);
	quadtreeAABBChanged();
}

void LightPointEmission::setPosition(float x, float y)
{
	mSprite.setPosition(x, y);
	quadtreeAABBChanged();
}

void LightPointEmission::move(const sf::Vector2f& movement)
{
	mSprite.move(movement);
	quadtreeAABBChanged();
}

void LightPointEmission::move(float x, float y)
{
	mSprite.move(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getPosition() const
{
	return mSprite.getPosition();
}

void LightPointEmission::setRotation(float angle)
{
	mSprite.setRotation(angle);
	quadtreeAABBChanged();
}

void LightPointEmission::rotate(float angle)
{
	mSprite.rotate(angle);
	quadtreeAABBChanged();
}

float LightPointEmission::getRotation() const
{
	return mSprite.getRotation();
}

void LightPointEmission::setScale(const sf::Vector2f& scale)
{
	mSprite.setScale(scale);
	quadtreeAABBChanged();
}

void LightPointEmission::setScale(float x, float y)
{
	mSprite.setScale(x, y);
	quadtreeAABBChanged();
}

void LightPointEmission::scale(const sf::Vector2f& scale)
{
	mSprite.scale(scale);
	quadtreeAABBChanged();
}

void LightPointEmission::scale(float x, float y)
{
	mSprite.scale(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getScale() const
{
	return mSprite.getScale();
}

void LightPointEmission::setOrigin(const sf::Vector2f& origin)
{
	mSprite.setOrigin(origin);
	quadtreeAABBChanged();
}

void LightPointEmission::setOrigin(float x, float y)
{
	mSprite.setOrigin(x, y);
	quadtreeAABBChanged();
}

const sf::Vector2f& LightPointEmission::getOrigin() const
{
	return mSprite.getOrigin();
}

void LightPointEmission::render(const sf::View& view, sf::RenderTexture& lightTempTexture, sf::RenderTexture& antumbraTempTexture, sf::Shader& unshadowShader, sf::Shader& lightOverShapeShader, const std::vector<priv::QuadtreeOccupant*>& shapes)
{
	float shadowExtension = mShadowOverExtendMultiplier * (getAABB().width + getAABB().height);

	struct OuterEdges
	{
		std::vector<int> _outerBoundaryIndices;
		std::vector<sf::Vector2f> _outerBoundaryVectors;
	};

	std::vector<OuterEdges> outerEdges(shapes.size());

	std::vector<int> innerBoundaryIndices;
	std::vector<sf::Vector2f> innerBoundaryVectors;
	std::vector<priv::Penumbra> penumbras;

	//----- Emission

	lightTempTexture.clear();
	lightTempTexture.setView(view);
	lightTempTexture.draw(*this);

	//----- Shapes

	// Mask off light shape (over-masking - mask too much, reveal penumbra/antumbra afterwards)
	unsigned int shapesCount = shapes.size();
	for (unsigned int i = 0; i < shapesCount; ++i)
	{
		LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);
		if (pLightShape->isAwake() && pLightShape->isTurnedOn())
		{
			// Get boundaries
			innerBoundaryIndices.clear();
			innerBoundaryVectors.clear();
			penumbras.clear();
			getPenumbrasPoint(penumbras, innerBoundaryIndices, innerBoundaryVectors, outerEdges[i]._outerBoundaryIndices, outerEdges[i]._outerBoundaryVectors, *pLightShape);

			if (innerBoundaryIndices.size() != 2 || outerEdges[i]._outerBoundaryIndices.size() != 2)
			{
				continue;
			}

			// Render shape
			if (!pLightShape->renderLightOver())
			{
				pLightShape->setColor(sf::Color::Black);
				lightTempTexture.draw(*pLightShape);
			}

			sf::Vector2f as = pLightShape->getTransform().transformPoint(pLightShape->getPoint(outerEdges[i]._outerBoundaryIndices[0]));
			sf::Vector2f bs = pLightShape->getTransform().transformPoint(pLightShape->getPoint(outerEdges[i]._outerBoundaryIndices[1]));
			sf::Vector2f ad = outerEdges[i]._outerBoundaryVectors[0];
			sf::Vector2f bd = outerEdges[i]._outerBoundaryVectors[1];

			sf::Vector2f intersectionOuter;

			// Handle antumbras as a seperate case
			if (priv::rayIntersect(as, ad, bs, bd, intersectionOuter))
			{
				sf::Vector2f asi = pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[0]));
				sf::Vector2f bsi = pLightShape->getTransform().transformPoint(pLightShape->getPoint(innerBoundaryIndices[1]));
				sf::Vector2f adi = innerBoundaryVectors[0];
				sf::Vector2f bdi = innerBoundaryVectors[1];

				antumbraTempTexture.clear(sf::Color::White);
				antumbraTempTexture.setView(view);

				sf::Vector2f intersectionInner;

				if (priv::rayIntersect(asi, adi, bsi, bdi, intersectionInner))
				{
					sf::ConvexShape maskShape;
					maskShape.setPointCount(3);
					maskShape.setPoint(0, asi);
					maskShape.setPoint(1, bsi);
					maskShape.setPoint(2, intersectionInner);
					maskShape.setFillColor(sf::Color::Black);
					antumbraTempTexture.draw(maskShape);
				}
				else
				{
					sf::ConvexShape maskShape;
					maskShape.setPointCount(4);
					maskShape.setPoint(0, asi);
					maskShape.setPoint(1, bsi);
					maskShape.setPoint(2, bsi + priv::vectorNormalize(bdi) * shadowExtension);
					maskShape.setPoint(3, asi + priv::vectorNormalize(adi) * shadowExtension);
					maskShape.setFillColor(sf::Color::Black);
					antumbraTempTexture.draw(maskShape);
				}

				unmaskWithPenumbras(antumbraTempTexture, sf::BlendAdd, unshadowShader, penumbras, shadowExtension);

				antumbraTempTexture.display();

				lightTempTexture.setView(lightTempTexture.getDefaultView());
				lightTempTexture.draw(sf::Sprite(antumbraTempTexture.getTexture()), sf::BlendMultiply);
				lightTempTexture.setView(view);
			}
			else
			{
				sf::ConvexShape maskShape;
				maskShape.setPointCount(4);
				maskShape.setPoint(0, as);
				maskShape.setPoint(1, bs);
				maskShape.setPoint(2, bs + priv::vectorNormalize(bd) * shadowExtension);
				maskShape.setPoint(3, as + priv::vectorNormalize(ad) * shadowExtension);
				maskShape.setFillColor(sf::Color::Black);
				lightTempTexture.draw(maskShape);

				unmaskWithPenumbras(lightTempTexture, sf::BlendMultiply, unshadowShader, penumbras, shadowExtension);
			}
		}
	}

	for (unsigned i = 0; i < shapesCount; i++)
	{
		LightShape* pLightShape = static_cast<LightShape*>(shapes[i]);

		if (pLightShape->renderLightOver())
		{
			pLightShape->setColor(sf::Color::White);
			lightTempTexture.draw(*pLightShape, &lightOverShapeShader);
		}
		else
		{
			pLightShape->setColor(sf::Color::Black);
			lightTempTexture.draw(*pLightShape);
		}
	}

	lightTempTexture.display();
}

void LightPointEmission::setLocalCastCenter(sf::Vector2f const & localCenter)
{
	mLocalCastCenter = localCenter;
}

sf::Vector2f LightPointEmission::getLocalCastCenter() const
{
	return mLocalCastCenter;
}

void LightPointEmission::setSourceRadius(float radius)
{
	mSourceRadius = radius;
}

float LightPointEmission::getSourceRadius() const
{
	return mSourceRadius;
}

void LightPointEmission::setShadowOverExtendMultiplier(float multiplier)
{
	mShadowOverExtendMultiplier = multiplier;
}

float LightPointEmission::getShadowOverExtendMultiplier() const
{
	return mShadowOverExtendMultiplier;
}

sf::FloatRect LightPointEmission::getAABB() const
{
	return mSprite.getGlobalBounds();
}

sf::Vector2f LightPointEmission::getCastCenter() const
{
	sf::Transform t = mSprite.getTransform();
	t.translate(mSprite.getOrigin());
	return t.transformPoint(mLocalCastCenter);
}

void LightPointEmission::getPenumbrasPoint(std::vector<priv::Penumbra>& penumbras, std::vector<int>& innerBoundaryIndices, std::vector<sf::Vector2f>& innerBoundaryVectors, std::vector<int>& outerBoundaryIndices, std::vector<sf::Vector2f>& outerBoundaryVectors, const LightShape& shape)
{
	sf::Vector2f sourceCenter = getCastCenter();

	const int numPoints = shape.getPointCount();

	std::vector<bool> bothEdgesBoundaryWindings;
	bothEdgesBoundaryWindings.reserve(2);

	std::vector<bool> oneEdgeBoundaryWindings;
	oneEdgeBoundaryWindings.reserve(2);

	// Calculate front and back facing sides
	std::vector<bool> facingFrontBothEdges;
	facingFrontBothEdges.reserve(numPoints);

	std::vector<bool> facingFrontOneEdge;
	facingFrontOneEdge.reserve(numPoints);

	for (int i = 0; i < numPoints; i++)
	{
		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(i));
		sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint((i < numPoints - 1) ? i + 1 : 0));

		sf::Vector2f firstEdgeRay;
		sf::Vector2f secondEdgeRay;
		sf::Vector2f firstNextEdgeRay;
		sf::Vector2f secondNextEdgeRay;

		{
			sf::Vector2f sourceToPoint = point - sourceCenter;
			sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
			firstEdgeRay = point - (sourceCenter - perpendicularOffset);
			secondEdgeRay = point - (sourceCenter + perpendicularOffset);
		}
		{
			sf::Vector2f sourceToPoint = nextPoint - sourceCenter;
			sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
			firstNextEdgeRay = nextPoint - (sourceCenter - perpendicularOffset);
			secondNextEdgeRay = nextPoint - (sourceCenter + perpendicularOffset);
		}

		sf::Vector2f pointToNextPoint = nextPoint - point;
		sf::Vector2f normal = priv::vectorNormalize(sf::Vector2f(-pointToNextPoint.y, pointToNextPoint.x));

		// Front facing, mark it
		facingFrontBothEdges.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f && priv::vectorDot(secondEdgeRay, normal) > 0.0f) || (priv::vectorDot(firstNextEdgeRay, normal) > 0.0f && priv::vectorDot(secondNextEdgeRay, normal) > 0.0f));
		facingFrontOneEdge.push_back((priv::vectorDot(firstEdgeRay, normal) > 0.0f || priv::vectorDot(secondEdgeRay, normal) > 0.0f) || priv::vectorDot(firstNextEdgeRay, normal) > 0.0f || priv::vectorDot(secondNextEdgeRay, normal) > 0.0f);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontBothEdges[i] != facingFrontBothEdges[i - 1])
		{
			innerBoundaryIndices.push_back(i);
			bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[i]);
		}
	}

	// Check looping indices separately
	if (facingFrontBothEdges[0] != facingFrontBothEdges[numPoints - 1])
	{
		innerBoundaryIndices.push_back(0);
		bothEdgesBoundaryWindings.push_back(facingFrontBothEdges[0]);
	}

	// Go through front/back facing list. Where the facing direction switches, there is a boundary
	for (int i = 1; i < numPoints; i++)
	{
		if (facingFrontOneEdge[i] != facingFrontOneEdge[i - 1])
		{
			outerBoundaryIndices.push_back(i);
			oneEdgeBoundaryWindings.push_back(facingFrontOneEdge[i]);
		}
	}

	// Check looping indices separately
	if (facingFrontOneEdge[0] != facingFrontOneEdge[numPoints - 1])
	{
		outerBoundaryIndices.push_back(0);
		oneEdgeBoundaryWindings.push_back(facingFrontOneEdge[0]);
	}

	// Compute outer boundary vectors
	for (unsigned bi = 0; bi < outerBoundaryIndices.size(); bi++)
	{
		int penumbraIndex = outerBoundaryIndices[bi];
		bool winding = oneEdgeBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f sourceToPoint = point - sourceCenter;
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (sourceCenter + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (sourceCenter - perpendicularOffset);

		// Add boundary vector
		outerBoundaryVectors.push_back(winding ? firstEdgeRay : secondEdgeRay);
	}

	for (unsigned bi = 0; bi < innerBoundaryIndices.size(); bi++)
	{
		int penumbraIndex = innerBoundaryIndices[bi];
		bool winding = bothEdgesBoundaryWindings[bi];

		sf::Vector2f point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
		sf::Vector2f sourceToPoint = point - sourceCenter;
		sf::Vector2f perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
		sf::Vector2f firstEdgeRay = point - (sourceCenter + perpendicularOffset);
		sf::Vector2f secondEdgeRay = point - (sourceCenter - perpendicularOffset);

		// Add boundary vector
		innerBoundaryVectors.push_back(winding ? secondEdgeRay : firstEdgeRay);
		sf::Vector2f outerBoundaryVector = winding ? firstEdgeRay : secondEdgeRay;

		if (innerBoundaryIndices.size() == 1)
		{
			innerBoundaryVectors.push_back(outerBoundaryVector);
		}

		// Add penumbras
		bool hasPrevPenumbra = false;

		sf::Vector2f prevPenumbraLightEdgeVector;

		float prevBrightness = 1.0f;

		while (penumbraIndex != -1)
		{
			int nextPointIndex = (penumbraIndex < numPoints - 1) ? penumbraIndex + 1 : 0;
			sf::Vector2f nextPoint = shape.getTransform().transformPoint(shape.getPoint(nextPointIndex));
			sf::Vector2f pointToNextPoint = nextPoint - point;

			int prevPointIndex = (penumbraIndex > 0) ? penumbraIndex - 1 : numPoints - 1;
			sf::Vector2f prevPoint = shape.getTransform().transformPoint(shape.getPoint(prevPointIndex));
			sf::Vector2f pointToPrevPoint = prevPoint - point;

			priv::Penumbra penumbra;
			penumbra._source = point;

			if (!winding)
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToNextPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle)
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToNextPoint;
					penumbraIndex = nextPointIndex;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					sourceToPoint = point - sourceCenter;
					perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
					firstEdgeRay = point - (sourceCenter + perpendicularOffset);
					secondEdgeRay = point - (sourceCenter - perpendicularOffset);
					outerBoundaryVector = secondEdgeRay;

					if (!outerBoundaryVectors.empty())
					{
						outerBoundaryVectors[0] = penumbra._darkEdge;
						outerBoundaryIndices[0] = penumbraIndex;
					}
				}
				else
				{
					penumbra._darkBrightness = 0.0f;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = false;

					if (!outerBoundaryVectors.empty())
					{
						outerBoundaryVectors[0] = penumbra._darkEdge;
						outerBoundaryIndices[0] = penumbraIndex;
					}

					penumbraIndex = -1;
				}
			}
			else // Winding = true
			{
				penumbra._lightEdge = (hasPrevPenumbra) ? prevPenumbraLightEdgeVector : innerBoundaryVectors.back();
				penumbra._darkEdge = outerBoundaryVector;
				penumbra._lightBrightness = prevBrightness;

				// Next point, check for intersection
				float intersectionAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(pointToPrevPoint)));
				float penumbraAngle = std::acos(priv::vectorDot(priv::vectorNormalize(penumbra._lightEdge), priv::vectorNormalize(penumbra._darkEdge)));

				if (intersectionAngle < penumbraAngle)
				{
					prevBrightness = penumbra._darkBrightness = intersectionAngle / penumbraAngle;

					assert(prevBrightness >= 0.0f && prevBrightness <= 1.0f);

					penumbra._darkEdge = pointToPrevPoint;
					penumbraIndex = prevPointIndex;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = true;
					prevPenumbraLightEdgeVector = penumbra._darkEdge;
					point = shape.getTransform().transformPoint(shape.getPoint(penumbraIndex));
					sourceToPoint = point - sourceCenter;
					perpendicularOffset = priv::vectorNormalize({ -sourceToPoint.y, sourceToPoint.x }) * mSourceRadius;
					firstEdgeRay = point - (sourceCenter + perpendicularOffset);
					secondEdgeRay = point - (sourceCenter - perpendicularOffset);
					outerBoundaryVector = firstEdgeRay;

					if (!outerBoundaryVectors.empty())
					{
						outerBoundaryVectors[1] = penumbra._darkEdge;
						outerBoundaryIndices[1] = penumbraIndex;
					}
				}
				else
				{
					penumbra._darkBrightness = 0.0f;

					if (hasPrevPenumbra)
					{
						std::swap(penumbra._darkBrightness, penumbras.back()._darkBrightness);
						std::swap(penumbra._lightBrightness, penumbras.back()._lightBrightness);
					}

					hasPrevPenumbra = false;

					if (!outerBoundaryVectors.empty())
					{
						outerBoundaryVectors[1] = penumbra._darkEdge;
						outerBoundaryIndices[1] = penumbraIndex;
					}

					penumbraIndex = -1;
				}
			}

			penumbras.push_back(penumbra);
		}
	}
}

void LightPointEmission::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

LightSystem::LightSystem()
	: mLightShapeQuadtree(sf::FloatRect())
	, mLightPointEmissionQuadtree(sf::FloatRect())
	, mDirectionEmissionRange(1000.0f)
	, mDirectionEmissionRadiusMultiplier(1.1f)
	, mAmbientColor(sf::Color(16, 16, 16))
{
}

void LightSystem::create(const sf::FloatRect& rootRegion, const sf::Vector2u& imageSize)
{
	mLightShapeQuadtree.create(rootRegion, 6, 6);
	mLightPointEmissionQuadtree.create(rootRegion, 6, 6);
	// Load Texture
	mPenumbraTexture.loadFromMemory(priv::penumbraTexture, (sizeof(priv::penumbraTexture) / sizeof(*priv::penumbraTexture)));
	mPenumbraTexture.setSmooth(true);

	// Load Shaders
	mUnshadowShader.loadFromMemory(priv::unshadowFragment, sf::Shader::Fragment);
	mLightOverShapeShader.loadFromMemory(priv::lightOverShapeFragment, sf::Shader::Fragment);

	update(imageSize);
}

void LightSystem::render(sf::RenderTarget& target)
{
	sf::View view = target.getView();

	if (target.getSize() != mLightTempTexture.getSize())
	{
		update(target.getSize());
	}

	mLightShapeQuadtree.update();
	mLightPointEmissionQuadtree.update();

	mCompositionTexture.clear(mAmbientColor);
	mCompositionTexture.setView(mCompositionTexture.getDefaultView());

	mLightTempTexture.setView(view);

	sf::FloatRect viewBounds = sf::FloatRect(view.getCenter() - view.getSize() * 0.5f, view.getSize());

	// --- Point lights

	std::vector<priv::QuadtreeOccupant*> lightShapes;

	sf::Sprite lightTempSprite(mLightTempTexture.getTexture());

	// Query lights
	std::vector<priv::QuadtreeOccupant*> viewPointEmissionLights;
	mLightPointEmissionQuadtree.query(viewBounds, viewPointEmissionLights);

	for (const auto& occupant : viewPointEmissionLights)
	{
		LightPointEmission* light = static_cast<LightPointEmission*>(occupant);
		if (light != nullptr && light->isTurnedOn())
		{
			// Query shapes
			lightShapes.clear();
			mLightShapeQuadtree.query(light->getAABB(), lightShapes);

			// Render on Emission Texture : used by lightOverShapeShader
			mEmissionTempTexture.clear();
			mEmissionTempTexture.setView(view);
			mEmissionTempTexture.draw(*light);
			mEmissionTempTexture.display();

			// Render light
			light->render(view, mLightTempTexture, mAntumbraTempTexture, mUnshadowShader, mLightOverShapeShader, lightShapes);
			mCompositionTexture.draw(lightTempSprite, sf::BlendAdd);
		}
	}

	//----- Direction lights

	float shadowExtension = priv::vectorMagnitude(view.getSize() * -0.5f) * mDirectionEmissionRadiusMultiplier;
	sf::Vector2f extendedBounds = sf::Vector2f(1.f, 1.f) * std::max(viewBounds.width, viewBounds.height) * mDirectionEmissionRadiusMultiplier;
	sf::FloatRect extendedViewBounds = priv::rectFromBounds(-extendedBounds, extendedBounds + sf::Vector2f(mDirectionEmissionRange, 0.0f));

	std::vector<priv::QuadtreeOccupant*> viewLightShapes;

	for (const auto& light : mDirectionEmissionLights)
	{
		// Create light shape
		sf::ConvexShape directionShape = priv::shapeFromRect(extendedViewBounds);
		directionShape.setPosition(view.getCenter());
		directionShape.setRotation(light->getCastAngle());

		// Query shapes
		viewLightShapes.clear();
		mLightShapeQuadtree.query(directionShape, viewLightShapes);

		// Render light
		light->render(view, mLightTempTexture, mAntumbraTempTexture, mUnshadowShader, viewLightShapes, shadowExtension);
		mCompositionTexture.draw(sf::Sprite(mLightTempTexture.getTexture()), sf::BlendAdd);
	}

	mCompositionTexture.display();

	target.setView(target.getDefaultView());
	target.draw(sf::Sprite(mCompositionTexture.getTexture()), sf::BlendMultiply);
	target.setView(view);
}

LightShape* LightSystem::createLightShape()
{
	LightShape* shape = new LightShape();
	mLightShapeQuadtree.addOccupant(shape);
	mLightShapes.insert(shape);
	return shape;
}


LightShape* LightSystem::createLightShape(const sf::RectangleShape& shape)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { shape.getSize().x, 0.f });
	lightShape->setPoint(2u, shape.getSize());
	lightShape->setPoint(3u, { 0.f, shape.getSize().y });
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::ConvexShape& shape)
{
	LightShape* lightShape = createLightShape();
	unsigned int pointCount = shape.getPointCount();
	lightShape->setPointCount(pointCount);
	for (unsigned int i = 0; i < pointCount; i++)
	{
		lightShape->setPoint(i, shape.getPoint(i));
	}
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::CircleShape& shape)
{
	LightShape* lightShape = createLightShape();
	unsigned int pointCount = shape.getPointCount();
	lightShape->setPointCount(pointCount);
	for (unsigned int i = 0; i < pointCount; i++)
	{
		lightShape->setPoint(i, shape.getPoint(i));
	}
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::FloatRect& rect)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { rect.width, 0.f });
	lightShape->setPoint(2u, { rect.width, rect.height });
	lightShape->setPoint(3u, { 0.f, rect.height });
	lightShape->setPosition(rect.left, rect.top);
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::Sprite& sprite)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { sprite.getTextureRect().width * 1.f, 0.f });
	lightShape->setPoint(2u, { sprite.getTextureRect().width * 1.f, sprite.getTextureRect().height * 1.f});
	lightShape->setPoint(3u, { 0.f, sprite.getTextureRect().height * 1.f});
	lightShape->setPosition(sprite.getPosition());
	lightShape->setOrigin(sprite.getOrigin());
	lightShape->setRotation(sprite.getRotation());
	lightShape->setScale(sprite.getScale());
	return lightShape;
}

void LightSystem::removeShape(LightShape* shape)
{
	auto itr = mLightShapes.find(shape);
	if (itr != mLightShapes.end())
	{
		mLightShapeQuadtree.removeOccupant(*itr);
		mLightShapes.erase(itr);
		delete shape;
	}
}

LightPointEmission* LightSystem::createLightPointEmission()
{
	LightPointEmission* light = new LightPointEmission();
	mLightPointEmissionQuadtree.addOccupant(light);
	mPointEmissionLights.insert(light);
	return light;
}

void LightSystem::removeLight(LightPointEmission* light)
{
	auto itr = mPointEmissionLights.find(light);
	if (itr != mPointEmissionLights.end())
	{
		mLightPointEmissionQuadtree.removeOccupant(*itr);
		mPointEmissionLights.erase(itr);
		delete light;
	}
}

LightDirectionEmission* LightSystem::createLightDirectionEmission()
{
	LightDirectionEmission* light = new LightDirectionEmission();
	mDirectionEmissionLights.insert(light);
	return light;
}

void LightSystem::removeLight(LightDirectionEmission* light)
{
	auto itr = mDirectionEmissionLights.find(light);
	if (itr != mDirectionEmissionLights.end())
	{
		mDirectionEmissionLights.erase(itr);
		delete light;
	}
}

void LightSystem::setDirectionEmissionRange(float range)
{
	mDirectionEmissionRange = range;
}

float LightSystem::getDirectionEmissionRange() const
{
	return mDirectionEmissionRange;
}

void LightSystem::setDirectionEmissionRadiusMultiplier(float multiplier)
{
	mDirectionEmissionRadiusMultiplier = multiplier;
}

float LightSystem::getDirectionEmissionRadiusMultiplier() const
{
	return mDirectionEmissionRadiusMultiplier;
}

void LightSystem::setAmbientColor(const sf::Color& color)
{
	mAmbientColor = color;
}

const sf::Color& LightSystem::getAmbientColor() const
{
	return mAmbientColor;
}

void LightSystem::update(sf::Vector2u const& size)
{
	mUnshadowShader.setUniform("penumbraTexture", mPenumbraTexture);

	if (size.x != 0 && size.y != 0)
	{
		mLightTempTexture.create(size.x, size.y);
		mEmissionTempTexture.create(size.x, size.y);
		mAntumbraTempTexture.create(size.x, size.y);
		mCompositionTexture.create(size.x, size.y);

		mLightOverShapeShader.setUniform("emissionTexture", mEmissionTempTexture.getTexture());
		mLightOverShapeShader.setUniform("targetSizeInv", sf::Vector2f(1.0f / size.x, 1.0f / size.y));
	}
}

sf::Texture& LightSystem::getPenumbraTexture()
{
	return mPenumbraTexture;
}

sf::Shader& LightSystem::getUnshadowShader()
{
	return mUnshadowShader;
}

sf::Shader& LightSystem::getLightOverShapeShader()
{
	return mLightOverShapeShader;
}

} // namespace ltbl