#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>

class ImageOutlineFinder
{
public:
	ImageOutlineFinder();
	~ImageOutlineFinder();

	void findOutLines(const sf::Image image);

	std::vector<sf::Vector2i> getVertices() const;


private:

	sf::Vector2i findStartingPoint() const;

	void traverseBoundary(sf::Vector2i startingPoint);

	sf::Vector2i findNextBoundaryPoint(sf::Vector2i point, sf::Vector2i sp) const;
	static sf::Vector2i clockWise(sf::Vector2i sp);

	bool isBoundary(sf::Vector2i point) const;

	bool pointHasAlpha(sf::Vector2i point) const;

	sf::Image image_;
	std::vector<sf::Vector2i> vertices_;

};

