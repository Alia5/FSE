#pragma once

#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include <v8.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/System.hpp>

inline v8pp::module getSFMod(v8::Isolate* isolate)
{
	v8pp::module sfmod(isolate);

	v8pp::class_<sf::Time> sfTimeClass(isolate);
	sfTimeClass.auto_wrap_objects();
	sfTimeClass.ctor<>();
	sfTimeClass.function("asSeconds", &sf::Time::asSeconds);
	sfTimeClass.function("asMilliseconds", &sf::Time::asMilliseconds);
	sfTimeClass.function("asMicroseconds", &sf::Time::asMicroseconds);
	sfmod.class_("Time", sfTimeClass);

	v8pp::class_<sf::Clock> sfClockClass(isolate);
	sfClockClass.auto_wrap_objects();
	sfClockClass.ctor<>();
	sfClockClass.function("getElapsedTime", &sf::Clock::getElapsedTime);
	sfClockClass.function("restart", &sf::Clock::restart);
	sfmod.class_("Clock", sfClockClass);

	v8pp::class_<sf::Window> sfWindowClass(isolate);
	sfWindowClass.auto_wrap_objects();
	//TODO: ctor sfWindowClass.ctor<>();
	sfWindowClass.function("close", &sf::Window::close);
	sfWindowClass.function("isOpen", &sf::Window::isOpen);
	sfWindowClass.function("getPosition", &sf::Window::getPosition);
	sfWindowClass.function("setPosition", &sf::Window::setPosition);
	sfWindowClass.function("getSize", &sf::Window::getSize);
	sfWindowClass.function("setSize", &sf::Window::setSize);
	sfWindowClass.function("setTitle", &sf::Window::setTitle);
	// sfWindowClass.function("setIcon", &sf::Window::setIcon);
	sfWindowClass.function("setVisible", &sf::Window::setVisible);
	sfWindowClass.function("setVerticalSyncEnabled", &sf::Window::setVerticalSyncEnabled);
	sfWindowClass.function("setMouseCursorVisible", &sf::Window::setMouseCursorVisible);
	sfWindowClass.function("setMouseCursorGrabbed", &sf::Window::setMouseCursorGrabbed);
	sfWindowClass.function("setMouseCursor", &sf::Window::setMouseCursor);
	sfWindowClass.function("setKeyRepeatEnabled", &sf::Window::setKeyRepeatEnabled);
	sfWindowClass.function("setFramerateLimit", &sf::Window::setFramerateLimit);
	sfWindowClass.function("setJoystickThreshold", &sf::Window::setJoystickThreshold);
	sfWindowClass.function("setActive", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Window*>(isolate, args.This());
			if (args.Length() == 0)
				return This->setActive();
			return This->setActive(v8pp::from_v8<bool>(isolate, args[0]));
		});
	sfWindowClass.function("requestFocus", &sf::Window::requestFocus);
	sfWindowClass.function("hasFocus", &sf::Window::hasFocus);
	sfWindowClass.function("display", &sf::Window::display);
	sfmod.class_("Window", sfWindowClass);

	v8pp::class_<sf::RenderTarget> sfRenderTargetClass(isolate);
	sfRenderTargetClass.auto_wrap_objects();
	sfRenderTargetClass.function("clear", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTarget*>(isolate, args.This());
			if (args.Length() == 0)
				return This->clear();
			This->clear(v8pp::from_v8<sf::Color>(isolate, args[0]));
	});
	sfRenderTargetClass.function("setView", &sf::RenderTarget::setView);
	sfRenderTargetClass.function("getView", &sf::RenderTarget::getView);
	sfRenderTargetClass.function("getDefaultView", &sf::RenderTarget::getDefaultView);
	sfRenderTargetClass.function("getViewport", &sf::RenderTarget::getViewport);
	sfRenderTargetClass.function("mapPixelToCoords", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTarget*>(isolate, args.This());
			if (args.Length() == 1)
				return This->mapPixelToCoords(v8pp::from_v8<sf::Vector2i>(isolate, args[0]));
			return This->mapPixelToCoords(v8pp::from_v8<sf::Vector2i>(isolate, args[0]),
				v8pp::from_v8<sf::View>(isolate, args[1]));
		});
	sfRenderTargetClass.function("mapCoordsToPixel", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTarget*>(isolate, args.This());
			if (args.Length() == 1)
				return This->mapCoordsToPixel(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			return This->mapCoordsToPixel(v8pp::from_v8<sf::Vector2f>(isolate, args[0]),
				v8pp::from_v8<sf::View>(isolate, args[1]));
		});
	sfRenderTargetClass.function("draw", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTarget*>(isolate, args.This());
			if (args.Length() == 1)
			{
				try
				{
					This->draw(*v8pp::from_v8<sf::Drawable*>(isolate, args[0]));
				} catch ( std::invalid_argument e)
				{
					This->draw(*v8pp::from_v8<sf::VertexBuffer*>(isolate, args[0]));
				}
			}
			if (args.Length() == 2)
			{
				try
				{
					This->draw(*v8pp::from_v8<sf::Drawable*>(isolate, args[0]), *v8pp::from_v8<sf::RenderStates*>(isolate, args[1]));
				}
				catch (std::invalid_argument e)
				{
					This->draw(*v8pp::from_v8<sf::VertexBuffer*>(isolate, args[0]), *v8pp::from_v8<sf::RenderStates*>(isolate, args[1]));
				}
			}
	});
	sfRenderTargetClass.function("getSize", &sf::RenderTarget::getSize);
	sfRenderTargetClass.function("setActive", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTarget*>(isolate, args.This());
			if (args.Length() == 0)
				return This->setActive();
			return This->setActive(v8pp::from_v8<bool>(isolate, args[0]));
		});
	sfRenderTargetClass.function("pushGLStates", &sf::RenderTarget::pushGLStates);
	sfRenderTargetClass.function("popGLStates", &sf::RenderTarget::popGLStates);
	sfRenderTargetClass.function("resetGLStates", &sf::RenderTarget::resetGLStates);
	sfmod.class_("RenderTarget", sfRenderTargetClass);

	v8pp::class_<sf::RenderTexture> sfRenderTextureClass(isolate);
	sfRenderTextureClass.auto_wrap_objects();
	sfRenderTextureClass.inherit<sf::RenderTarget>();
	sfRenderTextureClass.ctor<>();
	sfRenderTextureClass.function("create", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTexture*>(isolate, args.This());
			if (args.Length() == 3)
			{
				if (args[2]->IsBoolean())
					return  This->create(v8pp::from_v8<int>(isolate, args[0]),
						v8pp::from_v8<int>(isolate, args[1]),
						v8pp::from_v8<int>(isolate, args[2]));
				// TODO: contextSettings
				return  This->create(v8pp::from_v8<int>(isolate, args[0]),
					v8pp::from_v8<int>(isolate, args[1]),
					v8pp::from_v8<sf::ContextSettings>(isolate, args[2]));
			}
			return  This->create(v8pp::from_v8<int>(isolate, args[0]),
				v8pp::from_v8<int>(isolate, args[1]));
	});
	sfRenderTextureClass.function("getMaximumAntialiasingLevel", &sf::RenderTexture::getMaximumAntialiasingLevel);
	sfRenderTextureClass.function("setSmooth", &sf::RenderTexture::setSmooth);
	sfRenderTextureClass.function("isSmooth", &sf::RenderTexture::isSmooth);
	sfRenderTextureClass.function("setRepeated", &sf::RenderTexture::setRepeated);
	sfRenderTextureClass.function("isRepeated", &sf::RenderTexture::isRepeated);
	sfRenderTextureClass.function("generateMipmap", &sf::RenderTexture::generateMipmap);
	sfRenderTextureClass.function("setActive", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::RenderTexture*>(isolate, args.This());
			if (args.Length() == 0)
				return This->setActive();
			return This->setActive(v8pp::from_v8<bool>(isolate, args[0]));
		});
	sfRenderTextureClass.function("display", &sf::RenderTexture::display);
	sfRenderTextureClass.function("getSize", &sf::RenderTexture::getSize);
	sfRenderTextureClass.function("getTexture", &sf::RenderTexture::getTexture);
	// TODO: 
	sfmod.class_("RenderTexture", sfRenderTextureClass);
	
	v8pp::class_<sf::RenderWindow> sfRenderWindowClass(isolate);
	sfRenderWindowClass.auto_wrap_objects();
	sfRenderWindowClass.inherit<sf::Window>();
	sfRenderWindowClass.inherit<sf::RenderTarget>();
	//TODO: ctor sfRenderWindowClass.ctor<>();
	sfmod.class_("RenderWindow", sfRenderWindowClass);

	v8pp::module sfClipBoardMod(isolate);
	sfClipBoardMod.function("getString", sf::Clipboard::getString);
	sfClipBoardMod.function("setString", sf::Clipboard::setString);
	sfmod.submodule("Clipboard", sfClipBoardMod);

	
	v8pp::module sfMouseModule(isolate);
	
	v8pp::module sfMouseButtonModule(isolate);
	sfMouseButtonModule.const_("Left", sf::Mouse::Button::Left);
	sfMouseButtonModule.const_("Right", sf::Mouse::Button::Right);
	sfMouseButtonModule.const_("Middle", sf::Mouse::Button::Middle);
	sfMouseButtonModule.const_("XButton1", sf::Mouse::Button::XButton1);
	sfMouseButtonModule.const_("XButton2", sf::Mouse::Button::XButton2);
	sfMouseButtonModule.const_("ButtonCount", sf::Mouse::Button::ButtonCount);
	sfMouseModule.submodule("Button", sfMouseButtonModule);

	v8pp::module sfMouseWheelModule(isolate);
	sfMouseWheelModule.const_("VerticalWheel", sf::Mouse::Wheel::VerticalWheel);
	sfMouseWheelModule.const_("HorizontalWheel", sf::Mouse::Wheel::HorizontalWheel);
	sfMouseModule.submodule("Wheel", sfMouseWheelModule);

	sfMouseModule.function("isButtonPressed", sf::Mouse::isButtonPressed);
	sfMouseModule.function("getPosition", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return sf::Mouse::getPosition();
			return sf::Mouse::getPosition(*v8pp::from_v8<sf::Window*>(isolate, args[0]));
		});
	sfMouseModule.function("setPosition", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 1)
				return sf::Mouse::setPosition(v8pp::from_v8<sf::Vector2i>(isolate, args[0]));
			sf::Mouse::setPosition(v8pp::from_v8<sf::Vector2i>(isolate, args[0]),
				*v8pp::from_v8<sf::Window*>(isolate, args[1]));
		});
	
	sfmod.submodule("Mouse", sfMouseModule);

	v8pp::class_<sf::IntRect> sfIntRectClass(isolate);
	sfIntRectClass.auto_wrap_objects();
	sfIntRectClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new sf::IntRect();
			if (args.Length() == 2)
				return new sf::IntRect(v8pp::from_v8<sf::Vector2i>(isolate, args[0]),
					v8pp::from_v8<sf::Vector2i>(isolate, args[1]));
			return new sf::IntRect(v8pp::from_v8<int>(isolate, args[0]),
				v8pp::from_v8<int>(isolate, args[1]),
				v8pp::from_v8<int>(isolate, args[2]),
				v8pp::from_v8<int>(isolate, args[3]));

		});
	sfIntRectClass.function("contains", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::IntRect*>(isolate, args.This());
			if (args.Length() == 1)
				return This->contains(v8pp::from_v8<sf::Vector2<int>>(isolate, args[0]));
			return This->contains(v8pp::from_v8<int>(isolate, args[0]), v8pp::from_v8<int>(isolate, args[1]));
		});
	sfIntRectClass.function("intersects", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::IntRect*>(isolate, args.This());
			if (args.Length() == 1)
				return This->intersects(v8pp::from_v8<sf::IntRect>(isolate, args[0]));
			return This->intersects(v8pp::from_v8<sf::IntRect>(isolate, args[0]),
				*v8pp::from_v8<sf::IntRect*>(isolate, args[1]));
		});
	sfIntRectClass.var("left", &sf::IntRect::left);
	sfIntRectClass.var("top", &sf::IntRect::top);
	sfIntRectClass.var("width", &sf::IntRect::width);
	sfIntRectClass.var("height", &sf::IntRect::height);
	sfmod.class_("IntRect", sfIntRectClass);

	v8pp::class_<sf::FloatRect> sfFloatRectClass(isolate);
	sfFloatRectClass.auto_wrap_objects();
	sfFloatRectClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new sf::FloatRect();
			if (args.Length() == 2)
				return new sf::FloatRect(v8pp::from_v8<sf::Vector2f>(isolate, args[0]),
					v8pp::from_v8<sf::Vector2f>(isolate, args[1]));
			return new sf::FloatRect(v8pp::from_v8<float>(isolate, args[0]),
				v8pp::from_v8<float>(isolate, args[1]),
				v8pp::from_v8<float>(isolate, args[2]),
				v8pp::from_v8<float>(isolate, args[3]));

		});
	sfFloatRectClass.function("contains", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::FloatRect*>(isolate, args.This());
			if (args.Length() == 1)
				return This->contains(v8pp::from_v8<sf::Vector2<float>>(isolate, args[0]));
			return This->contains(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfFloatRectClass.function("intersects", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::FloatRect*>(isolate, args.This());
			if (args.Length() == 1)
				return This->intersects(v8pp::from_v8<sf::FloatRect>(isolate, args[0]));
			return This->intersects(v8pp::from_v8<sf::FloatRect>(isolate, args[0]),
				*v8pp::from_v8<sf::FloatRect*>(isolate, args[1]));
		});
	sfFloatRectClass.var("left", &sf::FloatRect::left);
	sfFloatRectClass.var("top", &sf::FloatRect::top);
	sfFloatRectClass.var("width", &sf::FloatRect::width);
	sfFloatRectClass.var("height", &sf::FloatRect::height);
	sfmod.class_("FloatRect", sfFloatRectClass);

	v8pp::class_<sf::Color> sfColorClass(isolate);
	sfColorClass.auto_wrap_objects();
	sfColorClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new sf::Color();
			if (args.Length() == 1)
				return new sf::Color(v8pp::from_v8<sf::Uint32>(isolate, args[0]));
			if (args.Length() == 3)
				return new sf::Color(v8pp::from_v8<int>(isolate, args[0]),
					v8pp::from_v8<int>(isolate, args[1]),
					v8pp::from_v8<int>(isolate, args[2]));
			return new sf::Color(v8pp::from_v8<int>(isolate, args[0]),
				v8pp::from_v8<int>(isolate, args[1]),
				v8pp::from_v8<int>(isolate, args[2]),
				v8pp::from_v8<int>(isolate, args[3]));
		});
	sfColorClass.function("toInteger", &sf::Color::toInteger);
	sfColorClass.var("r", &sf::Color::r);
	sfColorClass.var("g", &sf::Color::g);
	sfColorClass.var("b", &sf::Color::b);
	sfColorClass.var("a", &sf::Color::a);
	sfmod.class_("Color", sfColorClass);

	sfColorClass.static_("Black", sf::Color::Black, true);
	sfColorClass.static_("White", sf::Color::White, true);
	sfColorClass.static_("Red", sf::Color::Red, true);
	sfColorClass.static_("Green", sf::Color::Green, true);
	sfColorClass.static_("Blue", sf::Color::Blue, true);
	sfColorClass.static_("Yellow", sf::Color::Yellow, true);
	sfColorClass.static_("Magenta", sf::Color::Magenta, true);
	sfColorClass.static_("Cyan", sf::Color::Cyan, true);
	sfColorClass.static_("Transparent", sf::Color::Transparent, true);

	v8pp::class_<sf::Transformable> sfTransformableClass(isolate);
	sfTransformableClass.auto_wrap_objects();
	sfTransformableClass.ctor<>();
	sfTransformableClass.function("setPosition", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Transformable*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setPosition(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->setPosition(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfTransformableClass.function("setRotation", &sf::Transformable::setRotation);
	sfTransformableClass.function("setScale", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Transformable*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setScale(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->setScale(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfTransformableClass.function("setOrigin", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Transformable*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setOrigin(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->setOrigin(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfTransformableClass.function("getPosition", &sf::Transformable::getPosition);
	sfTransformableClass.function("getRotation", &sf::Transformable::getRotation);
	sfTransformableClass.function("getScale", &sf::Transformable::getScale);
	sfTransformableClass.function("getOrigin", &sf::Transformable::getOrigin);
	sfTransformableClass.function("move", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Transformable*>(isolate, args.This());
			if (args.Length() == 1)
				return This->move(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->move(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfTransformableClass.function("rotate", &sf::Transformable::rotate);
	sfTransformableClass.function("scale", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Transformable*>(isolate, args.This());
			if (args.Length() == 1)
				return This->scale(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->scale(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfTransformableClass.function("getTransform", &sf::Transformable::getTransform);
	sfTransformableClass.function("getInverseTransform", &sf::Transformable::getInverseTransform);
	sfmod.class_("Transformable", sfTransformableClass);

	v8pp::class_<sf::View> sfViewClass(isolate);
	sfViewClass.auto_wrap_objects();
	sfViewClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		if (args.Length() == 0)
			return new sf::View();
		const auto isolate = args.GetIsolate();
		if (args.Length() == 1)
			return new sf::View(v8pp::from_v8<sf::FloatRect>(isolate, args[0]));
		return new sf::View(v8pp::from_v8<sf::Vector2f>(isolate, args[0]),
			v8pp::from_v8<sf::Vector2f>(isolate, args[1]));
	});
	sfViewClass.function("setCenter", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::View*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setCenter(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->setCenter(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfViewClass.function("setSize", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::View*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setSize(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->setSize(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfViewClass.function("setRotation", &sf::View::setRotation);
	sfViewClass.function("setViewport", &sf::View::setViewport);
	sfViewClass.function("reset", &sf::View::reset);
	sfViewClass.function("getCenter", &sf::View::getCenter);
	sfViewClass.function("getSize", &sf::View::getSize);
	sfViewClass.function("getRotation", &sf::View::getRotation);
	sfViewClass.function("getViewport", &sf::View::getViewport);
	sfViewClass.function("move", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::View*>(isolate, args.This());
			if (args.Length() == 1)
				return This->move(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
			This->move(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	sfViewClass.function("rotate", &sf::View::rotate);
	sfViewClass.function("zoom", &sf::View::zoom);
	sfViewClass.function("getTransform", &sf::View::getTransform);
	sfViewClass.function("getInverseTransform", &sf::View::getInverseTransform);
	sfmod.class_("View", sfViewClass);

	v8pp::class_<sf::Drawable> sfDrawableClass(isolate);
	sfDrawableClass.auto_wrap_objects();
	sfmod.class_("Drawable", sfDrawableClass);

	v8pp::class_<sf::Image> sfImageClass(isolate);
	sfImageClass.auto_wrap_objects();
	sfImageClass.ctor<>();
	sfImageClass.function("create", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Image*>(isolate, args.This());
			if (args.Length() == 2)
				return This->create(v8pp::from_v8<unsigned int>(isolate, args[0]), v8pp::from_v8<unsigned int>(isolate, args[1]));
			return This->create(v8pp::from_v8<unsigned int>(isolate, args[0]), 
				v8pp::from_v8<unsigned int>(isolate, args[1]),
				v8pp::from_v8<sf::Color>(isolate, args[2]));
	});
	sfImageClass.function("loadFromFile", &sf::Image::loadFromFile);
	// sfImageClass.function("loadFromMemory", &sf::Image::loadFromMemory);
	sfImageClass.function("loadFromStream", &sf::Image::loadFromStream);
	sfImageClass.function("saveToFile", &sf::Image::saveToFile);
	sfImageClass.function("getSize", &sf::Image::getSize);
	sfImageClass.function("createMaskFromColor", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Image*>(isolate, args.This());
			if (args.Length() == 1)
				return This->createMaskFromColor(v8pp::from_v8<sf::Color>(isolate, args[0]));
			return This->createMaskFromColor(v8pp::from_v8<sf::Color>(isolate, args[0]),
				v8pp::from_v8<int>(isolate, args[1]));
		});
	sfImageClass.function("copy", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Image*>(isolate, args.This());
			if (args.Length() == 3)
				return This->copy(*v8pp::from_v8<sf::Image*>(isolate, args[0]),
					v8pp::from_v8<unsigned int>(isolate, args[1]),
					v8pp::from_v8<unsigned int>(isolate, args[2]));
			return This->copy(*v8pp::from_v8<sf::Image*>(isolate, args[0]),
				v8pp::from_v8<unsigned int>(isolate, args[1]),
				v8pp::from_v8<unsigned int>(isolate, args[2]),
				v8pp::from_v8<sf::IntRect>(isolate, args[3]));

	});
	sfImageClass.function("setPixel", &sf::Image::setPixel);
	sfImageClass.function("getPixel", &sf::Image::getPixel);
	// sfImageClass.function("getPixelsPtr", &sf::Image::getPixelsPtr);
	sfImageClass.function("flipHorizontally", &sf::Image::flipHorizontally);
	sfImageClass.function("flipVertically", &sf::Image::flipVertically);
	sfmod.class_("Image", sfImageClass);

	v8pp::module sfPrimitiveTypeModule(isolate);
	sfPrimitiveTypeModule.const_("Points", sf::PrimitiveType::Points);
	sfPrimitiveTypeModule.const_("Lines", sf::PrimitiveType::Lines);
	sfPrimitiveTypeModule.const_("LineStrip", sf::PrimitiveType::LineStrip);
	sfPrimitiveTypeModule.const_("Triangles", sf::PrimitiveType::Triangles);
	sfPrimitiveTypeModule.const_("TriangleStrip", sf::PrimitiveType::TriangleStrip);
	sfPrimitiveTypeModule.const_("TriangleFan", sf::PrimitiveType::TriangleFan);
	sfPrimitiveTypeModule.const_("Quads", sf::PrimitiveType::Quads);
	sfmod.submodule("PrimitiveType", sfPrimitiveTypeModule);

	v8pp::class_<sf::Texture> sfTextureClass(isolate);
	sfTextureClass.auto_wrap_objects();
	v8pp::module sfTextureCoordTypeModule(isolate);
	sfTextureCoordTypeModule.const_("Normalized", sf::Texture::CoordinateType::Normalized);
	sfTextureCoordTypeModule.const_("Pixels", sf::Texture::CoordinateType::Pixels);
	sfTextureClass.static_("CoordinateType", sfTextureCoordTypeModule.new_instance(), true);
	sfTextureClass.ctor<>();
	sfTextureClass.function("create", &sf::Texture::create);
	sfTextureClass.function("loadFromFile",[](v8::FunctionCallbackInfo<v8::Value> const & args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Texture*>(isolate, args.This());
			if (args.Length() == 1)
				return This->loadFromFile(v8pp::from_v8<std::string>(isolate, args[0]));
			return This->loadFromFile(v8pp::from_v8<std::string>(isolate, args[0]),
				v8pp::from_v8<sf::IntRect>(isolate, args[1]));
	});
	// TODO: sfTextureClass.function("loadFromMemory", &sf::Texture::loadFromMemory);
	sfTextureClass.function("loadFromStream", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Texture*>(isolate, args.This());
			if (args.Length() == 1)
				return This->loadFromStream(v8pp::from_v8<sf::InputStream>(isolate, args[0]));
			return This->loadFromStream(v8pp::from_v8<sf::InputStream>(isolate, args[0]),
				v8pp::from_v8<sf::IntRect>(isolate, args[1]));
		});
	sfTextureClass.function("loadFromImage", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Texture*>(isolate, args.This());
			if (args.Length() == 1)
				return This->loadFromImage(v8pp::from_v8<sf::Image>(isolate, args[0]));
			return This->loadFromImage(v8pp::from_v8<sf::Image>(isolate, args[0]),
				v8pp::from_v8<sf::IntRect>(isolate, args[1]));
		});
	sfTextureClass.function("getSize", &sf::Texture::getSize);
	sfTextureClass.function("copyToImage", &sf::Texture::copyToImage);
	sfTextureClass.function("update", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
		// TODO: more overloads
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Texture*>(isolate, args.This());
			try
			{
				if (args.Length() == 1)
					return This->update(v8pp::from_v8<sf::Texture>(isolate, args[0]));
				if (args.Length() == 3)
					return This->update(v8pp::from_v8<sf::Texture>(isolate, args[0]),
						v8pp::from_v8<unsigned int>(isolate, args[1]),
						v8pp::from_v8<unsigned int>(isolate, args[2]));
			} catch (std::invalid_argument e)
			{
				try
				{
					if (args.Length() == 1)
						return This->update(v8pp::from_v8<sf::Image>(isolate, args[0]));
					if (args.Length() == 3)
						return This->update(v8pp::from_v8<sf::Image>(isolate, args[0]),
							v8pp::from_v8<unsigned int>(isolate, args[1]),
							v8pp::from_v8<unsigned int>(isolate, args[2]));
					}
				catch (std::invalid_argument e)
				{

					if (args.Length() == 1)
						return This->update(v8pp::from_v8<sf::Window>(isolate, args[0]));
					if (args.Length() == 3)
						return This->update(v8pp::from_v8<sf::Window>(isolate, args[0]),
							v8pp::from_v8<unsigned int>(isolate, args[1]),
							v8pp::from_v8<unsigned int>(isolate, args[2]));
				}
			}
		});
	sfTextureClass.function("setSmooth", &sf::Texture::setSmooth);
	sfTextureClass.function("isSmooth", &sf::Texture::isSmooth);
	sfTextureClass.function("setSrgb", &sf::Texture::setSrgb);
	sfTextureClass.function("isSrgb", &sf::Texture::isSrgb);
	sfTextureClass.function("setRepeated", &sf::Texture::setRepeated);
	sfTextureClass.function("isRepeated", &sf::Texture::isRepeated);
	sfTextureClass.function("generateMipmap", &sf::Texture::generateMipmap);
	sfTextureClass.function("swap", &sf::Texture::swap);
	// sfTextureClass.function("getNativeHandle", &sf::Texture::getNativeHandle);
	sfTextureClass.function("bind", &sf::Texture::bind);
	sfTextureClass.function("getMaximumSize", &sf::Texture::getMaximumSize);
	sfmod.class_("Texture", sfTextureClass);

	v8pp::class_<sf::Shape> sfShapeClass(isolate);
	sfShapeClass.auto_wrap_objects();
	sfShapeClass.inherit<sf::Drawable>();
	sfShapeClass.inherit<sf::Transformable>();
	sfShapeClass.function("setTexture", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Shape*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setTexture(v8pp::from_v8<sf::Texture*>(isolate, args[0]));
			return This->setTexture(v8pp::from_v8<sf::Texture*>(isolate, args[0]), 
				v8pp::from_v8<bool>(isolate, args[1]));
	});
	sfShapeClass.function("setTextureRect", &sf::Shape::setTextureRect);
	sfShapeClass.function("setFillColor", &sf::Shape::setFillColor);
	sfShapeClass.function("setOutlineColor", &sf::Shape::setOutlineColor);
	sfShapeClass.function("setOutlineThickness", &sf::Shape::setOutlineThickness);
	sfShapeClass.function("getTexture", &sf::Shape::getTexture);
	sfShapeClass.function("getTextureRect", &sf::Shape::getTextureRect);
	sfShapeClass.function("getFillColor", &sf::Shape::getFillColor);
	sfShapeClass.function("getOutlineColor", &sf::Shape::getOutlineColor);
	sfShapeClass.function("getOutlineThickness", &sf::Shape::getOutlineThickness);
	sfShapeClass.function("getPointCount", &sf::Shape::getPointCount);
	sfShapeClass.function("getPoint", &sf::Shape::getPoint);
	sfShapeClass.function("getLocalBounds", &sf::Shape::getLocalBounds);
	sfShapeClass.function("getGlobalBounds", &sf::Shape::getGlobalBounds);
	sfmod.class_("Shape", sfShapeClass);

	v8pp::class_<sf::CircleShape> sfCircleShapeClass(isolate);
	sfCircleShapeClass.auto_wrap_objects();
	sfCircleShapeClass.inherit<sf::Shape>();
	sfCircleShapeClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			if (args.Length() == 0)
				return new sf::CircleShape();
			const auto isolate = args.GetIsolate();
			if (args.Length() == 1)
				return new sf::CircleShape(v8pp::from_v8<float>(isolate, args[0]));
			return new sf::CircleShape(v8pp::from_v8<float>(isolate, args[0]),
				v8pp::from_v8<int>(isolate, args[1]));
	});
	sfCircleShapeClass.function("setRadius", &sf::CircleShape::setRadius);
	sfCircleShapeClass.function("getRadius", &sf::CircleShape::getRadius);
	sfCircleShapeClass.function("setPointCount", &sf::CircleShape::setPointCount);
	sfCircleShapeClass.function("getPointCount", &sf::CircleShape::getPointCount);
	sfCircleShapeClass.function("getPoint", &sf::CircleShape::getPoint);
	sfmod.class_("CircleShape", sfCircleShapeClass);


	v8pp::class_<sf::RectangleShape> sfRectangleShapeClass(isolate);
	sfRectangleShapeClass.auto_wrap_objects();
	sfRectangleShapeClass.inherit<sf::Shape>();
	sfRectangleShapeClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			if (args.Length() == 0)
				return new sf::RectangleShape();
			const auto isolate = args.GetIsolate();
			return new sf::RectangleShape(v8pp::from_v8<sf::Vector2f>(isolate, args[0]));
		});
	sfRectangleShapeClass.function("setSize", &sf::RectangleShape::setSize);
	sfRectangleShapeClass.function("getSize", &sf::RectangleShape::getSize);
	sfRectangleShapeClass.function("getPointCount", &sf::RectangleShape::getPointCount);
	sfRectangleShapeClass.function("getPoint", &sf::RectangleShape::getPoint);
	sfmod.class_("RectangleShape", sfRectangleShapeClass);


	v8pp::class_<sf::ConvexShape> sfConvexShapeClass(isolate);
	sfConvexShapeClass.auto_wrap_objects();
	sfConvexShapeClass.inherit<sf::Shape>();
	sfConvexShapeClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			if (args.Length() == 0)
				return new sf::ConvexShape();
			const auto isolate = args.GetIsolate();
			return new sf::ConvexShape(v8pp::from_v8<int>(isolate, args[0]));
		});
	sfConvexShapeClass.function("setPointCount", &sf::ConvexShape::setPointCount);
	sfConvexShapeClass.function("getPointCount", &sf::ConvexShape::getPointCount);
	sfConvexShapeClass.function("getPoint", &sf::ConvexShape::getPoint);
	sfmod.class_("ConvexShape", sfConvexShapeClass);


	v8pp::class_<sf::Sprite> sfSpriteClass(isolate);
	sfSpriteClass.auto_wrap_objects();
	sfSpriteClass.inherit<sf::Drawable>();
	sfSpriteClass.inherit<sf::Transformable>();
	sfSpriteClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			if (args.Length() == 0)
				return new sf::Sprite();
			const auto isolate = args.GetIsolate();
			if (args.Length() == 1)
				return new sf::Sprite(*v8pp::from_v8<sf::Texture*>(isolate, args[0]));
			return new sf::Sprite(*v8pp::from_v8<sf::Texture*>(isolate, args[0]),
				*v8pp::from_v8<sf::IntRect*>(isolate, args[1]));
	});
	sfSpriteClass.function("setTexture", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Sprite*>(isolate, args.This());
			if (args.Length() == 1)
				return This->setTexture(*v8pp::from_v8<sf::Texture*>(isolate, args[0]));
			return This->setTexture(*v8pp::from_v8<sf::Texture*>(isolate, args[0]),
				v8pp::from_v8<bool>(isolate, args[1]));
		});
	sfSpriteClass.function("setTextureRect", &sf::Sprite::setTextureRect);
	sfSpriteClass.function("setColor", &sf::Sprite::setColor);
	sfSpriteClass.function("getTexture", &sf::Sprite::getTexture);
	sfSpriteClass.function("getTextureRect", &sf::Sprite::getTextureRect);
	sfSpriteClass.function("getColor", &sf::Sprite::getColor);
	sfSpriteClass.function("getLocalBounds", &sf::Sprite::getLocalBounds);
	sfSpriteClass.function("getGlobalBounds", &sf::Sprite::getGlobalBounds);
	sfmod.class_("Sprite", sfSpriteClass);
	
	v8pp::class_<sf::Vertex> sfVertexClass(isolate);
	sfVertexClass.auto_wrap_objects();
	sfVertexClass.ctor<>();
	sfVertexClass.var("position", &sf::Vertex::position);
	sfVertexClass.var("color", &sf::Vertex::color);
	sfVertexClass.var("texCoords", &sf::Vertex::texCoords);
	sfmod.class_("Vertex", sfVertexClass);

	v8pp::class_<sf::VertexArray> sfVertexArrayClass(isolate);
	sfVertexArrayClass.auto_wrap_objects();
	sfVertexArrayClass.inherit<sf::Drawable>();
	sfVertexArrayClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			if (args.Length() == 0)
				return new sf::VertexArray();
			const auto isolate = args.GetIsolate();
			if (args.Length() == 1)
				return new sf::VertexArray(static_cast<sf::PrimitiveType>(v8pp::from_v8<int>(isolate, args[0])));
			return new sf::VertexArray(static_cast<sf::PrimitiveType>(v8pp::from_v8<int>(isolate, args[0])),
				v8pp::from_v8<int>(isolate, args[1]));
		});
	sfVertexArrayClass.function("getVertexCount", &sf::VertexArray::getVertexCount);
	sfVertexArrayClass.function("clear", &sf::VertexArray::clear);
	sfVertexArrayClass.function("resize", &sf::VertexArray::resize);
	sfVertexArrayClass.function("append", &sf::VertexArray::append);
	sfVertexArrayClass.function("setPrimitiveType", &sf::VertexArray::setPrimitiveType);
	sfVertexArrayClass.function("getPrimitiveType", &sf::VertexArray::getPrimitiveType);
	sfVertexArrayClass.function("getBounds", &sf::VertexArray::getBounds);
	sfVertexArrayClass.property("vertices", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			std::vector<sf::Vertex> result;
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::VertexArray*>(isolate, args.This());
			result.reserve(This->getVertexCount());
			for (auto i = 0; i < This->getVertexCount(); i++)
				result.push_back((*This)[i]);
			return result;
		
	}, [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		const auto isolate = args.GetIsolate();
		const auto This = v8pp::from_v8<sf::VertexArray*>(isolate, args.This());
		const auto vec = v8pp::from_v8<std::vector<sf::Vertex>>(isolate, args[0]);
		This->clear();
		for (const auto& vert : vec)
			This->append(vert);

	});
	sfmod.class_("VertexArray", sfVertexArrayClass);

	// TODO: this, right now, is useless without update methods
	v8pp::class_<sf::VertexBuffer> sfVertexBufferClass(isolate);
	sfVertexBufferClass.auto_wrap_objects();
	sfVertexBufferClass.inherit<sf::Drawable>();
	v8pp::module VertexBufferUserModule(isolate);
	VertexBufferUserModule.const_("Stream", sf::VertexBuffer::Usage::Stream);
	VertexBufferUserModule.const_("Dynamic", sf::VertexBuffer::Usage::Dynamic);
	VertexBufferUserModule.const_("Static", sf::VertexBuffer::Usage::Static);
	sfVertexBufferClass.static_("Usage", VertexBufferUserModule.new_instance(), true);
	sfVertexBufferClass.ctor<>(); // TODO: overloads
	sfVertexBufferClass.function("create", &sf::VertexBuffer::create);
	sfVertexBufferClass.function("getVertexCount", &sf::VertexBuffer::getVertexCount);
	// todo: array sfVertexBufferClass.function("update", &sf::VertexBuffer::update);
	sfVertexBufferClass.function("swap", &sf::VertexBuffer::swap);
	// sfVertexBufferClass.function("getNativeHandle", &sf::VertexBuffer::getNativeHandle);
	sfVertexBufferClass.function("setPrimitiveType", &sf::VertexBuffer::setPrimitiveType);
	sfVertexBufferClass.function("getPrimitiveType", &sf::VertexBuffer::getPrimitiveType);
	sfVertexBufferClass.function("setUsage", &sf::VertexBuffer::setUsage);
	sfVertexBufferClass.function("getUsage", &sf::VertexBuffer::getUsage);
	sfVertexBufferClass.function("bind", &sf::VertexBuffer::bind);
	sfVertexBufferClass.function("isAvailable", &sf::VertexBuffer::isAvailable);
	sfmod.class_("VertexBuffer", sfVertexBufferClass);


	v8pp::class_<sf::BlendMode> sfBlendModeClass(isolate);
	v8pp::module sfBlendModeFactorModule(isolate);
	sfBlendModeFactorModule.const_("Zero", sf::BlendMode::Factor::Zero);
	sfBlendModeFactorModule.const_("One", sf::BlendMode::Factor::One);
	sfBlendModeFactorModule.const_("SrcColor", sf::BlendMode::Factor::SrcColor);
	sfBlendModeFactorModule.const_("OneMinusSrcColor", sf::BlendMode::Factor::OneMinusSrcColor);
	sfBlendModeFactorModule.const_("DstColor", sf::BlendMode::Factor::DstColor);
	sfBlendModeFactorModule.const_("OneMinusDstColor", sf::BlendMode::Factor::OneMinusDstColor);
	sfBlendModeFactorModule.const_("SrcAlpha", sf::BlendMode::Factor::SrcAlpha);
	sfBlendModeFactorModule.const_("OneMinusSrcAlpha", sf::BlendMode::Factor::OneMinusSrcAlpha);
	sfBlendModeFactorModule.const_("DstAlpha", sf::BlendMode::Factor::DstAlpha);
	sfBlendModeFactorModule.const_("OneMinusDstAlpha", sf::BlendMode::Factor::OneMinusDstAlpha);
	sfBlendModeClass.static_("Factor", sfBlendModeFactorModule.new_instance(), true);
	v8pp::module sfBlendModeEquationModule(isolate);
	sfBlendModeEquationModule.const_("Add", sf::BlendMode::Equation::Add);
	sfBlendModeEquationModule.const_("Subtract", sf::BlendMode::Equation::Subtract);
	sfBlendModeEquationModule.const_("ReverseSubtract", sf::BlendMode::Equation::ReverseSubtract);
	sfBlendModeClass.static_("Equation", sfBlendModeEquationModule.new_instance(), true);
	sfBlendModeClass.auto_wrap_objects();
	sfBlendModeClass.ctor<>(); // Todo: overloads
	sfBlendModeClass.var("colorSrcFactor", &sf::BlendMode::colorSrcFactor);
	sfBlendModeClass.var("colorDstFactor", &sf::BlendMode::colorDstFactor);
	sfBlendModeClass.var("colorEquation", &sf::BlendMode::colorEquation);
	sfBlendModeClass.var("alphaSrcFactor", &sf::BlendMode::alphaSrcFactor);
	sfBlendModeClass.var("alphaDstFactor", &sf::BlendMode::alphaDstFactor);
	sfBlendModeClass.var("alphaEquation", &sf::BlendMode::alphaEquation);
	sfmod.class_("BlendMode", sfBlendModeClass);

	sfBlendModeClass.static_("BlendAlpha", sf::BlendAlpha, true);
	sfBlendModeClass.static_("BlendAdd", sf::BlendAdd, true);
	sfBlendModeClass.static_("BlendMultiply", sf::BlendMultiply, true);
	sfBlendModeClass.static_("BlendNone", sf::BlendNone, true);

	v8pp::class_<sf::Shader::CurrentTextureType> sfShaderCurrentTextureTypeClass(isolate);
	sfShaderCurrentTextureTypeClass.auto_wrap_objects();
	sfmod.class_("CurrentTextureType", sfShaderCurrentTextureTypeClass);
	
	v8pp::class_<sf::Shader> sfShaderClass(isolate);
	v8pp::module sfShaderTypeModule(isolate);
	sfShaderTypeModule.const_("Vertex", sf::Shader::Type::Vertex);
	sfShaderTypeModule.const_("Geometry", sf::Shader::Type::Geometry);
	sfShaderTypeModule.const_("Fragment", sf::Shader::Type::Fragment);
	sfShaderClass.static_("Type", sfShaderTypeModule.new_instance(), true);
	sfShaderClass.auto_wrap_objects();
	sfShaderClass.ctor<>();
	sfShaderClass.static_("CurrentTexture", sf::Shader::CurrentTexture, true);
	sfShaderClass.function("loadFromFile", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Shader*>(isolate, args.This());
			if (args.Length() == 2)
			{
				if (args[1]->IsString())
					return This->loadFromFile(v8pp::from_v8<std::string>(isolate, args[0]),
						v8pp::from_v8<std::string>(isolate, args[1]));
				return This->loadFromFile(v8pp::from_v8<std::string>(isolate, args[0]),
					static_cast<sf::Shader::Type>(v8pp::from_v8<int>(isolate, args[1])));
			}
			return This->loadFromFile(v8pp::from_v8<std::string>(isolate, args[0]),
				v8pp::from_v8<std::string>(isolate, args[1]),
				v8pp::from_v8<std::string>(isolate, args[2]));
	});
	sfShaderClass.function("loadFromMemory", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Shader*>(isolate, args.This());
			if (args.Length() == 2)
			{
				if (args[1]->IsString())
					return This->loadFromMemory(v8pp::from_v8<std::string>(isolate, args[0]),
						v8pp::from_v8<std::string>(isolate, args[1]));
				return This->loadFromMemory(v8pp::from_v8<std::string>(isolate, args[0]),
					static_cast<sf::Shader::Type>(v8pp::from_v8<int>(isolate, args[1])));
			}
			return This->loadFromMemory(v8pp::from_v8<std::string>(isolate, args[0]),
				v8pp::from_v8<std::string>(isolate, args[1]),
				v8pp::from_v8<std::string>(isolate, args[2]));
		});
	sfShaderClass.function("loadFromStream", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Shader*>(isolate, args.This());
			if (args.Length() == 2)
			{
				if (args[1]->IsString())
					return This->loadFromStream(v8pp::from_v8<sf::InputStream>(isolate, args[0]),
						v8pp::from_v8<sf::InputStream>(isolate, args[1]));
				return This->loadFromStream(v8pp::from_v8<sf::InputStream>(isolate, args[0]),
					static_cast<sf::Shader::Type>(v8pp::from_v8<int>(isolate, args[1])));
			}
			return This->loadFromStream(v8pp::from_v8<sf::InputStream>(isolate, args[0]),
				v8pp::from_v8<sf::InputStream>(isolate, args[1]),
				v8pp::from_v8<sf::InputStream>(isolate, args[2]));
		});
	sfShaderClass.function("setUniform", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<sf::Shader*>(isolate, args.This());
			const auto name = v8pp::from_v8<std::string>(isolate, args[0]);
			if (args[1]->IsNumber())
				return This->setUniform(name, v8pp::from_v8<float>(isolate, args[1]));
			// TODO: overloads vor glsVecs and matrices
			return This->setUniform(name, v8pp::from_v8<bool>(isolate, args[1]));
		});
	// TODO: sfShaderClass.function("setUniformArray", &sf::Shader::setUniformArray);
	sfShaderClass.function("bind", &sf::Shader::bind);
	sfShaderClass.function("isAvailable", &sf::Shader::isAvailable);
	sfShaderClass.function("isGeometryAvailable", &sf::Shader::isGeometryAvailable);
	sfmod.class_("Shader", sfShaderClass);

	v8pp::class_<sf::RenderStates> sfRenderStatesClass(isolate);
	sfRenderStatesClass.auto_wrap_objects();
	sfRenderStatesClass.ctor<>();
	sfRenderStatesClass.var("blendMode", &sf::RenderStates::blendMode);
	sfRenderStatesClass.var("transform", &sf::RenderStates::transform);
	sfRenderStatesClass.var("texture", &sf::RenderStates::texture);
	sfRenderStatesClass.var("shader", &sf::RenderStates::shader);
	sfmod.class_("RenderStates", sfRenderStatesClass);
	sfRenderStatesClass.static_("Default", sf::RenderStates::Default, true);

	v8pp::class_<sf::Glyph> sfGlyphClass(isolate);
	sfGlyphClass.auto_wrap_objects();
	sfGlyphClass.ctor<>();
	sfGlyphClass.var("advance", &sf::Glyph::advance);
	sfGlyphClass.var("bounds", &sf::Glyph::bounds);
	sfGlyphClass.var("textureRect", &sf::Glyph::textureRect);
	sfmod.class_("Glpyh", sfGlyphClass);

	v8pp::class_<sf::Font::Info> sfFontInfoClass(isolate);
	sfFontInfoClass.auto_wrap_objects();
	sfFontInfoClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new sf::Font::Info();
	});
	sfFontInfoClass.var("family", &sf::Font::Info::family);
	sfmod.class_("FontInfo", sfFontInfoClass);
	
	v8pp::class_<sf::Font> sfFontClass(isolate);
	sfFontClass.auto_wrap_objects();
	sfFontClass.ctor<>();
	sfFontClass.function("loadFromFile", &sf::Font::loadFromFile);
	//TODO: sfFontClass.function("loadFromMemory", &sf::Font::loadFromMemory);
	sfFontClass.function("loadFromStream", &sf::Font::loadFromStream);
	sfFontClass.function("getInfo", &sf::Font::getInfo);
	sfFontClass.function("getGlyph", &sf::Font::getGlyph); //TODO: default param
	sfFontClass.function("getKerning", &sf::Font::getKerning);
	sfFontClass.function("getLineSpacing", &sf::Font::getLineSpacing);
	sfFontClass.function("getUnderlinePosition", &sf::Font::getUnderlinePosition);
	sfFontClass.function("getUnderlineThickness", &sf::Font::getUnderlineThickness);
	sfFontClass.function("getTexture", &sf::Font::getTexture);
	sfmod.class_("Font", sfFontClass);

	v8pp::class_<sf::Text> sfTextClass(isolate);
	sfTextClass.auto_wrap_objects();
	sfTextClass.inherit<sf::Drawable>();
	sfTextClass.inherit<sf::Transformable>();
	v8pp::module sfTextTypeMod(isolate);
	sfTextTypeMod.const_("Regular", sf::Text::Style::Regular);
	sfTextTypeMod.const_("Bold", sf::Text::Style::Bold);
	sfTextTypeMod.const_("Italic", sf::Text::Style::Italic);
	sfTextTypeMod.const_("Underlined", sf::Text::Style::Underlined);
	sfTextTypeMod.const_("StrikeThrough", sf::Text::Style::StrikeThrough);
	sfTextClass.static_("Style", sfTextTypeMod.new_instance(), true);
	sfTextClass.ctor<>(); //TODO: overloads
	sfTextClass.function("setString", &sf::Text::setString);
	sfTextClass.function("setFont", &sf::Text::setFont);
	sfTextClass.function("setCharacterSize", &sf::Text::setCharacterSize);
	sfTextClass.function("setLineSpacing", &sf::Text::setLineSpacing);
	sfTextClass.function("setLetterSpacing", &sf::Text::setLetterSpacing);
	sfTextClass.function("setStyle", &sf::Text::setStyle);
	sfTextClass.function("setFillColor", &sf::Text::setFillColor);
	sfTextClass.function("setOutlineColor", &sf::Text::setOutlineColor);
	sfTextClass.function("setOutlineThickness", &sf::Text::setOutlineThickness);
	sfTextClass.function("getString", &sf::Text::getString);
	sfTextClass.function("getFont", &sf::Text::getFont);
	sfTextClass.function("getCharacterSize", &sf::Text::getCharacterSize);
	sfTextClass.function("getLetterSpacing", &sf::Text::getLetterSpacing);
	sfTextClass.function("getLineSpacing", &sf::Text::getLineSpacing);
	sfTextClass.function("getStyle", &sf::Text::getStyle);
	sfTextClass.function("getFillColor", &sf::Text::getFillColor);
	sfTextClass.function("getOutlineColor", &sf::Text::getOutlineColor);
	sfTextClass.function("getOutlineThickness", &sf::Text::getOutlineThickness);
	sfTextClass.function("findCharacterPos", &sf::Text::findCharacterPos);
	sfTextClass.function("getLocalBounds", &sf::Text::getLocalBounds);
	sfTextClass.function("getGlobalBounds", &sf::Text::getGlobalBounds);
	sfmod.class_("Text", sfTextClass);
	return sfmod;
}
