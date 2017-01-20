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

		sf::Font font_;
		sf::Text fps_text_;
		sf::RectangleShape background_;

	private:
		sf::View counter_view_;

		float current_time_;
		sf::Clock measureclock_;
		float fps_ = 0;
		float smoothing_ = 0.98f;
		sf::Clock updclock_;

		Signal<>::Connection on_resize_connection_;

	};
}