#include "ImageOutlineFinder.h"



ImageOutlineFinder::ImageOutlineFinder()
{
}


ImageOutlineFinder::~ImageOutlineFinder()
{
}

void ImageOutlineFinder::findOutLines(const sf::Image image)
{
	image_ = image;
	const sf::Vector2i starting_point = findStartingPoint();
	if (starting_point.x > -1)
	{
		traverseBoundary(starting_point);
	}
}

sf::Vector2i ImageOutlineFinder::findStartingPoint() const
{

	for (unsigned int cx = 0; cx < image_.getSize().x; cx++) {
		for (unsigned int cy = 0; cy < image_.getSize().y; cy++) {
			if (isBoundary({ static_cast<int>(cx), static_cast<int>(cy) })) {
				return { static_cast<int>(cx), static_cast<int>(cy) };
			}
		}
	}
	return {-1, -1};
}

void ImageOutlineFinder::traverseBoundary(sf::Vector2i startingPoint)
{
	const sf::Vector2i start = sf::Vector2i(startingPoint.x, startingPoint.y);
	sf::Vector2i next = start;
	sf::Vector2i old = sf::Vector2i(-1, -1);


	sf::Vector2i next_nsp = { -1, 0 };

	vertices_.push_back(start);

	// Set arbitary limit to length of circumference
	for (unsigned int i = 0; i < static_cast<unsigned int>((image_.getSize().x * image_.getSize().y) / 2); i++) {
		// Save off the last position
		old = next;

		// Find the next clockwise boundary pixel
		next = findNextBoundaryPoint(next, next_nsp);

		// Set the search start point as the previous pixel
		// to avoid loops and continue traversing the shape
		// in the same direction i.e.

		// edge:  x  first point     second point  34x
		//       x            23x                  2x
		//      x             1x                   1

		// In this diagram the numbers represent the order
		// in which the neighboring points are tested. We
		// Always start with the previous point found.
		next_nsp = {old.x - next.x, old.y - next.y};

		// When we get back to the start break the loop 
		if (next.x == start.x && next.y == start.y)
			break;

		vertices_.push_back(next);
	}
}

sf::Vector2i ImageOutlineFinder::findNextBoundaryPoint(const sf::Vector2i point, const sf::Vector2i sp) const
{
	sf::Vector2i nsp = sp;
	for (int i = 0; i < 9; i++) {
		nsp = clockWise(nsp);
		const sf::Vector2i next = { point.x + nsp.x, point.y + nsp.y };
		if (isBoundary(next)) {
			return next;
		}
	}
	return {-1, -1};
}

sf::Vector2i ImageOutlineFinder::clockWise(const sf::Vector2i sp)
{
	if (sp.x == -1) {
		if (sp.y == -1)
			return { -1, 0 };
		if (sp.y == 0)
			return { -1, 1 };
		if (sp.y == 1)
			return { 0, 1 };
	}

	if (sp.x == 0) {
		if (sp.y == -1)
			return {-1, -1};
		if (sp.y == 1)
			return {1, 1};
	}

	if (sp.x == 1) {
		if (sp.y == -1)
			return {0, -1};
		if (sp.y == 0)
			return {1, -1};
		if (sp.y == 1)
			return {1, 0};
	}
	return {-10 , -10};

}

std::vector<sf::Vector2i> ImageOutlineFinder::getVertices() const
{
	return vertices_;
}


bool ImageOutlineFinder::isBoundary(const sf::Vector2i point) const
{

	if (!pointHasAlpha(point))
		return false;

	for (int i = -1; i<2; i++) {
		for (int j = -1; j<2; j++) {
			if (!pointHasAlpha({ point.x + i, point.y + j }))
				return true;
		}
	}

	// If this point has alpha and all of the ajoining points
	// have alpha then it's not a boundary
	return false;
}



bool ImageOutlineFinder::pointHasAlpha(const sf::Vector2i point) const
{
	if (point.x < 0 || point.x > (image_.getSize().x - 1) || point.y < 0 || point.y > (image_.getSize().y - 1)) 
		return true;
	
	if (image_.getPixel(point.x, point.y).a < 255)
		return true;

	return false;
}






