#pragma once
#include <random>

namespace fse
{
	class Random
	{
	public:

		static int random(int from, int to)
		{
            std::uniform_int_distribution<int> dist(from, to);
            return dist(rng);
		}

		static float random(float from, float to)
		{
			std::uniform_real_distribution<float> dist(from, to);
			return dist(rng);
		}

		static double random(double from, double to)
		{
			std::uniform_real_distribution<double> dist(from, to);
			return dist(rng);
		}

		static void InitRandom()
		{
			rng.seed(std::random_device()());
		}

	private:

		static std::mt19937 rng;
	};
}

