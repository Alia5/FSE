#pragma once

#include "FSEObject.h"

namespace fse
{
	class FPSCounter : public FSEObject
	{
	public:
		FPSCounter(Scene* scene);
		FPSCounter(Scene* scene, const sf::Vector2f& spawnPos);
		~FPSCounter();

		void update(float deltaTime) override;
		void draw(sf::RenderTarget& target) override;
		void spawned() override;

	protected:

		sf::Font m_font;
		sf::Text m_fpsText;
		sf::RectangleShape m_background;

	private:
		sf::View counterView;

		float currentTime;
		sf::Clock measureclock;
		float fps = 0;
		float smoothing = 0.98f;
		sf::Clock updclock;

		Signal<>::Connection onResizeConnection;

	};
}