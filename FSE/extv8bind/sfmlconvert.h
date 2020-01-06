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


// Explicit convertor template specialization
template<typename T>
struct v8pp::convert<sf::Vector2<T>>
{
	using from_type = sf::Vector2<T>;
	using to_type = v8::Local<v8::Object>;

	static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		const auto ctx = isolate->GetCurrentContext();
		return !value.IsEmpty() && (value->IsArray() && value.As<v8::Array>()->Length() == 2)
			|| (value->IsObject() && value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "x")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "y")).FromMaybe(false));
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		v8::HandleScope scope(isolate);
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected [x, y] array or {x, y} object");
		}
		from_type result;
		const auto ctx = isolate->GetCurrentContext();
		if (value->IsArray())
		{
			v8::Local<v8::Array> arr = value.As<v8::Array>();
			result.x = v8pp::from_v8<T>(isolate, arr->Get(ctx, 0).ToLocalChecked());
			result.y = v8pp::from_v8<T>(isolate, arr->Get(ctx, 1).ToLocalChecked());
		} else {
			v8::Local<v8::Object> obj = value.As<v8::Object>();
			result.x = v8pp::from_v8<T>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "x")).ToLocalChecked());
			result.y = v8pp::from_v8<T>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "y")).ToLocalChecked());
		}
		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, sf::Vector2<T> const& value)
	{
		v8::EscapableHandleScope scope(isolate);
		const auto ctx = isolate->GetCurrentContext();
		v8::Local<v8::Object> obj = v8::Object::New(isolate);
		obj->Set(ctx, v8pp::to_v8(isolate, "x"), v8pp::to_v8(isolate, value.x));
		obj->Set(ctx, v8pp::to_v8(isolate, "y"), v8pp::to_v8(isolate, value.y));
		return scope.Escape(obj);
	}
};

template<typename T>
struct v8pp::is_wrapped_class<sf::Vector2<T>> : std::false_type {};


template<typename T>
struct v8pp::convert<sf::Vector3<T>>
{
	using from_type = sf::Vector3<T>;
	using to_type = v8::Local<v8::Object>;

	static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		const auto ctx = isolate->GetCurrentContext();
		return !value.IsEmpty() && (value->IsArray() && value.As<v8::Array>()->Length() == 3)
			|| (value->IsObject() && value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "x")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "y")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "z")).FromMaybe(false));
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		v8::HandleScope scope(isolate);
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected [x, y, z] array or {x, y, z} object");
		}
		from_type result;
		const auto ctx = isolate->GetCurrentContext();
		if (value->IsArray())
		{
			v8::Local<v8::Array> arr = value.As<v8::Array>();
			result.x = v8pp::from_v8<T>(isolate, arr->Get(ctx, 0).ToLocalChecked());
			result.y = v8pp::from_v8<T>(isolate, arr->Get(ctx, 1).ToLocalChecked());
			result.z = v8pp::from_v8<T>(isolate, arr->Get(ctx, 2).ToLocalChecked());
		}
		else {
			v8::Local<v8::Object> obj = value.As<v8::Object>();
			result.x = v8pp::from_v8<T>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "x")).ToLocalChecked());
			result.y = v8pp::from_v8<T>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "y")).ToLocalChecked());
			result.z = v8pp::from_v8<T>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "z")).ToLocalChecked());
		}
		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, sf::Vector3<T> const& value)
	{
		v8::EscapableHandleScope scope(isolate);
		const auto ctx = isolate->GetCurrentContext();
		v8::Local<v8::Object> obj = v8::Object::New(isolate);
		obj->Set(ctx, v8pp::to_v8(isolate, "x"), v8pp::to_v8(isolate, value.x));
		obj->Set(ctx, v8pp::to_v8(isolate, "y"), v8pp::to_v8(isolate, value.y));
		obj->Set(ctx, v8pp::to_v8(isolate, "z"), v8pp::to_v8(isolate, value.z));
		return scope.Escape(obj);
	}
};

template<typename T>
struct v8pp::is_wrapped_class<sf::Vector3<T>> : std::false_type {};

template<>
struct v8pp::convert<sf::String>
{
	using from_type = sf::String;
	using to_type = v8::Local<v8::Value>;

	static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		const auto ctx = isolate->GetCurrentContext();
		return !value.IsEmpty() && value->IsString();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		v8::HandleScope scope(isolate);
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected string");
		}
		return sf::String(v8pp::from_v8<std::string>(isolate, value).c_str());
	}

	static to_type to_v8(v8::Isolate* isolate, sf::String const& value)
	{
		v8::EscapableHandleScope scope(isolate);
		const v8::Local<v8::Value> obj = v8pp::to_v8(isolate, value.toAnsiString());
		return scope.Escape(obj);
	}
};

template<>
struct v8pp::is_wrapped_class<sf::String> : std::false_type {};