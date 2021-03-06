#include "ImageOutlineFinder.h"
#include "FMath.h"

#include <poly2tri.h>
#include <iostream>

#include "../liquidfun_contrib/ConvexDecomposition/b2Polygon.h"


ImageOutlineFinder::ImageOutlineFinder()
= default;

ImageOutlineFinder::~ImageOutlineFinder()
= default;

void ImageOutlineFinder::findOutLines(const sf::Image& image, const int alpha_threshold)
{
	alpha_threshold_ = alpha_threshold;
	image_ = image;
	const sf::Vector2i starting_point = findStartingPoint();
	if (starting_point.x > -1)
	{
		traverseBoundary({ starting_point.x + 1, starting_point.y + 1 });
	}
}

sf::Vector2i ImageOutlineFinder::findStartingPoint() const
{
	for (unsigned int cx = 1; cx < image_.getSize().x; cx++) {
		for (unsigned int cy = 1; cy < image_.getSize().y; cy++) {
			if (isBoundary({ static_cast<int>(cx), static_cast<int>(cy) })) {
				return { static_cast<int>(cx), static_cast<int>(cy) };
			}
		}
	}
	return {-1, -1};
}

void ImageOutlineFinder::traverseBoundary(const sf::Vector2i starting_point)
{
	const sf::Vector2i start = sf::Vector2i(starting_point.x, starting_point.y);
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

		if (next.x < 0 || next.y < 0)
		{
			sf::Vector2i nnext = next;

			if (nnext.x < 0)
				nnext.x = 0;

			if (nnext.y < 0)
				nnext.y = 0;

			vertices_.push_back(nnext);
		}
		else
		{
			vertices_.push_back(next);
		}
	}


	for (auto& vert : vertices_)
	{
		vert.x -= image_.getSize().x / 2.f;
		vert.y -= image_.getSize().y / 2.f;
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

std::vector<sf::Vector2f> ImageOutlineFinder::getSimplifiedVertices(const float limit, unsigned int average_before, unsigned int average_after)
{
	std::vector<sf::Vector2f> smoothed_line;
	std::vector<sf::Vector2f> simplified_line;

	if (vertices_.size() == 0)
		return std::vector<sf::Vector2f>();

	smoothed_line.emplace_back(vertices_[0].x, vertices_[0].y);
	std::vector<sf::Vector2f>  average_vertices;

	// Loop over the next [average] vertices and 
	// add the result to the array of smoothed points
	if (average_before > 0 && average_before < vertices_.size())
	{
		for (unsigned int i = 0; i < vertices_.size() - average_before; i++) {

			average_vertices.clear();
			for (int j = 0; j < average_before; j++) {
				average_vertices.emplace_back(vertices_[i + j].x, vertices_[i + j].y);
			}
			smoothed_line.push_back(ImageOutlineFinder::average(average_vertices));
		}
	} else
	{
		for (const auto& vert : vertices_)
		{
			smoothed_line.emplace_back(sf::Vector2f(vert.x, vert.y ));
		}
	}

	float curvature_total = 0.f;
	float last_curvature = 0.f;

	for (unsigned int i = 0; i < smoothed_line.size() - 3; i++) {
		// Calculate the curvature
		const float curvature = ImageOutlineFinder::curvature(smoothed_line[i], smoothed_line[i + 1], smoothed_line[i + 2]);

		// Use a curvature accumulator to prevent cases
		// where a line curves gradually - this would be 
		// be picked up if we just used the curvature because
		// each individual curvature may be less than our limit
		curvature_total += curvature;

		// If the total curvature is greater than our set
		// limit then add the point to our simplified line
		if (curvature_total > limit) {
			curvature_total = 0;
			if (simplified_line.empty()
				|| (simplified_line.back().x != smoothed_line[i].x && simplified_line.back().y != smoothed_line[i].y))
				simplified_line.push_back(smoothed_line[i]);
		}
	}

	smoothed_line.clear();
	smoothed_line.emplace_back(vertices_[0].x, vertices_[0].y);
	// Loop over the next [average] vertices and 
	// add the result to the array of smoothed points
	if (average_after > 0 && average_after < simplified_line.size())
	{
		for (unsigned int i = 0; i < simplified_line.size() - average_after; i++) {

			average_vertices.clear();
			for (int j = 0; j < average_after; j++) {
				average_vertices.emplace_back(simplified_line[i + j].x, simplified_line[i + j].y);
			}
			smoothed_line.push_back(ImageOutlineFinder::average(average_vertices));
		}
		return smoothed_line;
	}
	return simplified_line;
}


std::vector<std::vector<sf::Vector2f>> ImageOutlineFinder::getSimplifiedTriangles(const float limit, const unsigned int average,const unsigned int average_after, const float min_area)
{
	std::vector<std::vector<sf::Vector2f>> result;

	std::vector<sf::Vector2f> simplified_verts = getSimplifiedVertices(limit, average, average_after);

	if (simplified_verts.size() < 3)
		return result;

	std::vector<p2t::Point*> polyline;

	polyline.reserve(simplified_verts.size());
	for (auto& point : simplified_verts)
	{
		polyline.push_back(new p2t::Point(point.x, point.y));
	}


	p2t::CDT* cdt = new p2t::CDT(polyline);
	cdt->Triangulate();
	auto tris = cdt->GetTriangles();

	for (auto& tri : tris)
	{
		std::vector<sf::Vector2f> sf_tri;
		sf_tri.reserve(3);
		p2t::Point* p = tri->GetPoint(0);
		sf_tri.emplace_back( p->x, p->y );
		p = tri->PointCCW(*p);
		sf_tri.emplace_back( p->x, p->y );
		p = tri->PointCCW(*p);
		sf_tri.emplace_back( p->x, p->y );

		float area = (sf_tri[0].x*(sf_tri[1].y - sf_tri[2].x)
			+ sf_tri[1].x*(sf_tri[2].y - sf_tri[0].y)
			+ sf_tri[2].x*(sf_tri[0].y - sf_tri[1].y)) / 2.f;
		if (area < 0)
			area *= -1;

		if (area >= min_area)
			result.push_back(sf_tri);
	}

	for (auto & i : polyline) {
		delete i;
	}
	polyline.clear();
	delete cdt;
	return result;
}

std::vector<std::vector<sf::Vector2f>> ImageOutlineFinder::getSimplifiedPolys(float limit, unsigned average,
	unsigned average_after, unsigned int max_polys)
{
	
	auto simplverts = getSimplifiedVertices(limit, average, average_after);
	std::vector<b2Vec2> cloud;

	if (limit < 0)
	{
		cloud.reserve(simplverts.size());

		for (const auto & vert : simplverts)
		{
			cloud.emplace_back(vert.x, vert.y);
		}
	} 
	else
	{
		cloud.reserve(vertices_.size());

		for (const auto & vert : vertices_)
		{
			cloud.emplace_back(vert.x, vert.y);
		}		
	}

	if (cloud.size() < 3)
		return std::vector<std::vector<sf::Vector2f>>();

	b2Polygon polygon = ConvexHull(cloud.data(), cloud.size());

	std::vector<b2Polygon> polys;
	polys.resize(max_polys);
	const auto size = DecomposeConvex(&polygon, polys.data(), max_polys);

	std::vector<std::vector<sf::Vector2f>> result;

	if (size == -1)
		return result;

	result.reserve(size);
	for (auto i = 0; i < size; i++)
	{
		if (polys[i].IsUsable(true) && polys[i].nVertices > 3)
		{
			std::vector<sf::Vector2f> poly;
			poly.reserve(polys[i].nVertices);
			auto verts = polys[i].GetVertexVecs();
			for (auto j = 0; j < polys[i].nVertices; j++)
			{
				poly.emplace_back(verts->x, verts->y);
				verts++;
			}
			result.push_back(poly);
		}
	}

	return result;
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
	if (point.x < 0 || point.x > static_cast<int>(image_.getSize().x - 1) 
		|| point.y < 0 || point.y > static_cast<int>(image_.getSize().y - 1)) 
		return true;
	
	return image_.getPixel(point.x, point.y).a < alpha_threshold_;
}

sf::Vector2f ImageOutlineFinder::average(std::vector<sf::Vector2f> vertices)
{
	sf::Vector2f average = vertices[0];
	for (auto& v : vertices)
	{
		if (v.x == vertices[0].x && v.y == vertices[0].y)
			continue;
		average = { average.x + v.x, average.y + v.y };
	}
	average = { average.x / vertices.size(), average.y / vertices.size() };
	return average;
}

sf::Vector2f ImageOutlineFinder::midPoint(const sf::Vector2f v1, const sf::Vector2f v2)
{
	return { (v1.x + v2.x) / 2.f, (v1.y + v2.y) / 2.f };
}

float ImageOutlineFinder::curvature(const sf::Vector2f v1, const sf::Vector2f v2, const sf::Vector2f v3)
{
	const sf::Vector2f mid_point = midPoint(v1, v3);
	return sq(mid_point.x - v2.x) + sq(mid_point.y - v2.y);
}

float ImageOutlineFinder::sq(const float f)
{
	return f * f;
}






