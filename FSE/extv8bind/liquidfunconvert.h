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


template<>
struct v8pp::convert<b2Vec2>
{
	using from_type = b2Vec2;
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
			result.x = v8pp::from_v8<float>(isolate, arr->Get(ctx, 0).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, arr->Get(ctx, 1).ToLocalChecked());
		}
		else {
			v8::Local<v8::Object> obj = value.As<v8::Object>();
			result.x = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "x")).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "y")).ToLocalChecked());
		}
		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, b2Vec2 const& value)
	{
		v8::EscapableHandleScope scope(isolate);
		const auto ctx = isolate->GetCurrentContext();
		v8::Local<v8::Object> obj = v8::Object::New(isolate);
		obj->Set(ctx, v8pp::to_v8(isolate, "x"), v8pp::to_v8(isolate, value.x));
		obj->Set(ctx, v8pp::to_v8(isolate, "y"), v8pp::to_v8(isolate, value.y));
		return scope.Escape(obj);
	}
};

template<>
struct v8pp::is_wrapped_class<b2Vec2> : std::false_type {};


template<>
struct v8pp::convert<b2Vec3>
{
	using from_type = b2Vec3;
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
			result.x = v8pp::from_v8<float>(isolate, arr->Get(ctx, 0).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, arr->Get(ctx, 1).ToLocalChecked());
			result.z = v8pp::from_v8<float>(isolate, arr->Get(ctx, 2).ToLocalChecked());
		}
		else {
			v8::Local<v8::Object> obj = value.As<v8::Object>();
			result.x = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "x")).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "y")).ToLocalChecked());
			result.z = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "z")).ToLocalChecked());
		}
		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, b2Vec3 const& value)
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

template<>
struct v8pp::is_wrapped_class<b2Vec3> : std::false_type {};

template<>
struct v8pp::convert<b2Vec4>
{
	using from_type = b2Vec4;
	using to_type = v8::Local<v8::Object>;

	static bool is_valid(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		const auto ctx = isolate->GetCurrentContext();
		return !value.IsEmpty() && (value->IsArray() && value.As<v8::Array>()->Length() == 4)
			|| (value->IsObject() && value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "x")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "y")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "z")).FromMaybe(false)
				&& value.As<v8::Object>()->HasOwnProperty(ctx, v8pp::to_v8(isolate, "w")).FromMaybe(false));
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		v8::HandleScope scope(isolate);
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected [x, y, z, w] array or {x, y, z, w} object");
		}
		from_type result;
		const auto ctx = isolate->GetCurrentContext();
		if (value->IsArray())
		{
			v8::Local<v8::Array> arr = value.As<v8::Array>();
			result.x = v8pp::from_v8<float>(isolate, arr->Get(ctx, 0).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, arr->Get(ctx, 1).ToLocalChecked());
			result.z = v8pp::from_v8<float>(isolate, arr->Get(ctx, 2).ToLocalChecked());
			result.w = v8pp::from_v8<float>(isolate, arr->Get(ctx, 3).ToLocalChecked());
		}
		else {
			v8::Local<v8::Object> obj = value.As<v8::Object>();
			result.x = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "x")).ToLocalChecked());
			result.y = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "y")).ToLocalChecked());
			result.z = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "z")).ToLocalChecked());
			result.w = v8pp::from_v8<float>(isolate, obj->Get(ctx, v8pp::to_v8(isolate, "w")).ToLocalChecked());
		}
		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, b2Vec4 const& value)
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

template<>
struct v8pp::is_wrapped_class<b2Vec4> : std::false_type {};