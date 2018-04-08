#pragma once
#include <random>
#include <chaiscript/chaiscript.hpp>

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

		static chaiscript::ModulePtr bootstrap(
			chaiscript::ModulePtr m = std::make_shared<chaiscript::Module>())
		{
			m->add(chaiscript::fun([](int from, int to)
			{
				return random(from, to);
			}), "random");
			m->add(chaiscript::fun([](float from, float to)
			{
				return random(from, to);
			}), "random");
			m->add(chaiscript::fun([](double from, double to)
			{
				return random(from, to);
			}), "random");

			return m;
		}

	private:

		static std::mt19937 rng;
	};
}

