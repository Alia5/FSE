#include "FPSCounter.h"
#include "../Application.h"
#include "../FSE-ImGui/imgui-plotvar.h"
#include <rttr/registration>

namespace fse
{
	FPSCounter::FPSCounter() : FPSCounter(sf::Vector2f(0, 0))
	{
	}

	FPSCounter::FPSCounter(const sf::Vector2f& spawnPos) : FSEObject(spawnPos)
	{
		font_.loadFromFile("data/fonts/bitstream-vera/VeraMoBd.ttf");


		fps_text_.setFont(font_);
		fps_text_.setFillColor(sf::Color::Green);
		fps_text_.setString(L"FPS: 0");
		fps_text_.setCharacterSize(16);

		sf::FloatRect rekt = fps_text_.getLocalBounds();
		background_.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
		background_.setPosition(rekt.left - 10, rekt.top - 10);
		background_.setFillColor(sf::Color(0, 0, 0, 128));

		setZOrder(257);
	}

	FPSCounter::~FPSCounter()
	{

	}

	void FPSCounter::update(float deltaTime)
	{
	}

	void FPSCounter::draw(sf::RenderTarget& target)
	{
		current_time_ = measureclock_.restart().asSeconds();
		fps_ = (fps_ * smoothing_) + ((1.f / current_time_) * (1.f - smoothing_));
		if (!detailed_view_)
		{
			if (updclock_.getElapsedTime().asMilliseconds() > 250)
			{
				fps_text_.setString(std::wstring(L"FPS: " + std::to_wstring(static_cast<int>((fps_ + last_fps_) / 2.f))));
				last_fps_ = fps_;
				sf::FloatRect rekt = fps_text_.getLocalBounds();
				background_.setSize(sf::Vector2f(rekt.width + 10, rekt.height + 10));
				background_.setPosition(rekt.left - 5, rekt.top - 5);
				updclock_.restart();
			}

			sf::View origView = target.getView();
			target.setView(counter_view_);
			target.draw(background_);
			target.draw(fps_text_);
			target.setView(origView);
		} 
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.f, 0.f, 0.f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.0f));
			ImGui::Begin("##FPSDetailed", nullptr,
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
			ImGui::SetWindowPos(ImVec2(0, 0));
			if (updclock_.getElapsedTime().asSeconds() > 1)
			{
				ImGui::PlotVarFlushOldEntries();
				updclock_.restart();
			}
			ImGui::PlotVar("FPS", fps_, 0, 250, 250);
			ImGui::PlotVar("Frametime", current_time_, 0, 0.1f, 250);
			ImGui::End();
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
		}
	}

	void FPSCounter::spawned()
	{
		counter_view_ = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(scene_->getApplication()->getWindow()->getSize().x),
			static_cast<float>(scene_->getApplication()->getWindow()->getSize().y)));
		on_resize_connection_ = scene_->getApplication()->on_window_resized_.connect([this]()
		{
			counter_view_ = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(scene_->getApplication()->getWindow()->getSize().x),
				static_cast<float>(scene_->getApplication()->getWindow()->getSize().y)));
		});
	}

	void FPSCounter::onDespawn()
	{
		scene_->getApplication()->on_window_resized_.disconnect(on_resize_connection_);
	}

	void FPSCounter::setShowDetailed(bool detailed)
	{
#ifdef ANDROID
		detailed_view_ = false;
#else
		detailed_view_ = detailed;
#endif
	}

	bool FPSCounter::isDetailed() const
	{
		return detailed_view_;
	}

	FSE_CHAI_REGISTER(FPSCounter)
	{
		RegisterChaiUserTypeFromRTTR<FPSCounter>(chai);
		chai.add(chaiscript::base_class<fse::FSEObject, FPSCounter>());
		chai.add(chaiscript::fun((&FPSCounter::detailed_view_)), "detailed");
	}

}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<FPSCounter>("fse::FPSCounter")
	.constructor<>()
	(
		policy::ctor::as_std_shared_ptr
	)
	.property("detailed_view_", &FPSCounter::detailed_view_)
	;
}
