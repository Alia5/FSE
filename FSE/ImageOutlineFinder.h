#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>

class ImageOutlineFinder
{
public:
	ImageOutlineFinder();
	~ImageOutlineFinder();

	void findOutLines(const sf::Image& image, int alpha_threshold);

	std::vector<sf::Vector2i> getVertices() const;
	std::vector<sf::Vector2f> getSimplifiedVertices(const float limit, unsigned int average_before, unsigned int average_after = 0);

	std::vector<std::vector<sf::Vector2f>> getSimplifiedTriangles(float limit, unsigned int average, unsigned int average_after = 0, float min_area = 50);
	std::vector<std::vector<sf::Vector2f>> getSimplifiedPolys(float limit, unsigned int average, unsigned int average_after = 0, unsigned int max_polys = 512);

private:

	sf::Vector2i findStartingPoint() const;

	void traverseBoundary(sf::Vector2i starting_point);

	sf::Vector2i findNextBoundaryPoint(sf::Vector2i point, sf::Vector2i sp) const;
	static sf::Vector2i clockWise(sf::Vector2i sp);

	bool isBoundary(sf::Vector2i point) const;

	bool pointHasAlpha(sf::Vector2i point) const;


	static sf::Vector2f average(std::vector<sf::Vector2f> vertices);
	static sf::Vector2f midPoint(sf::Vector2f v1, sf::Vector2f v2);
	static float curvature(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f v3);
	static float sq(float f);

	int alpha_threshold_ = 200;
	sf::Image image_;
	std::vector<sf::Vector2i> vertices_;
};

