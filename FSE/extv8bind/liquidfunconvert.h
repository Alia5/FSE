#pragma once
#include <v8pp/convert.hpp>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>

template<>
struct v8pp::convert<b2World*>
{
	using from_type = b2World*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<b2World> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected b2World");
		}

		return my_class_wrapper::unwrap_object(isolate, value);
	}

	static to_type to_v8(v8::Isolate* isolate, b2World* const& value)
	{
		auto val = my_class_wrapper::find_object(isolate, value);
		if (val.IsEmpty())
		{
			val = my_class_wrapper::reference_external(isolate, value);
		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<b2World*> : std::false_type {};

template<>
struct v8pp::convert<b2Body*>
{
	using from_type = b2Body*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<b2Body> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected b2Body");
		}

		return my_class_wrapper::unwrap_object(isolate, value);
	}

	static to_type to_v8(v8::Isolate* isolate, b2Body* const& value)
	{
		auto val = my_class_wrapper::find_object(isolate, value);
		if (val.IsEmpty())
		{
			val = my_class_wrapper::reference_external(isolate, value);
		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<b2Body*> : std::false_type {};

template<>
struct v8pp::convert<b2Fixture*>
{
	using from_type = b2Fixture*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<b2Fixture> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected b2Fixture");
		}

		return my_class_wrapper::unwrap_object(isolate, value);
	}

	static to_type to_v8(v8::Isolate* isolate, b2Fixture* const& value)
	{
		auto val = my_class_wrapper::find_object(isolate, value);
		if (val.IsEmpty())
		{
			val = my_class_wrapper::reference_external(isolate, value);
		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<b2Fixture*> : std::false_type {};