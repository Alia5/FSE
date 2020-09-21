#pragma once
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include "Scene.h"
#include "FSEObject/FSEObject.h"


//template<typename T>
//struct v8pp::convert<sf::Vector2<T>>
//{
//	using from_type = sf::Vector2<T>;
//	using to_type = v8::Local<v8::Array>;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty() && value->IsArray()
//			&& value.As<v8::Array>()->Length() == 2;
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected [x, y] array");
//		}
//
//		v8::HandleScope scope(isolate);
//		v8::Local<v8::Array> arr = value.As<v8::Array>();
//
//		from_type result;
//		result.x = v8pp::from_v8<T>(isolate, arr->Get(isolate->GetCurrentContext(), 0).ToLocalChecked());
//		result.y = v8pp::from_v8<T>(isolate, arr->Get(isolate->GetCurrentContext(),1).ToLocalChecked());
//
//		return result;
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, sf::Vector2<T> const& value)
//	{
//		v8::EscapableHandleScope scope(isolate);
//
//		v8::Local<v8::Array> arr = v8::Array::New(isolate, 2);
//		arr->Set(isolate->GetCurrentContext(), 0, v8pp::to_v8(isolate, value.x));
//		arr->Set(isolate->GetCurrentContext(), 1, v8pp::to_v8(isolate, value.y));
//
//		return scope.Escape(arr);
//	}
//};
//
//template<typename T>
//struct v8pp::is_wrapped_class<sf::Vector2<T>> : std::false_type {};
//
template<>
struct v8pp::convert<std::shared_ptr<fse::Scene>>
{
    using from_type = std::shared_ptr<fse::Scene>;
    using from_type_raw = fse::Scene*;
    using to_type = v8::Local<v8::Object>;

    typedef v8pp::class_<fse::Scene, v8pp::shared_ptr_traits> my_class_wrapper;
    typedef v8pp::class_<fse::Scene, v8pp::raw_ptr_traits> my_class_wrapper_raw;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
    {
        return !value.IsEmpty();
    }

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
    {
        if (!is_valid(isolate, value))
        {
            throw std::invalid_argument("expected FSEObject");
        }

        return std::shared_ptr<fse::Scene>(my_class_wrapper::unwrap_object(isolate, value));
    }

    static to_type to_v8(v8::Isolate* isolate, std::shared_ptr<fse::Scene> const& value)
    {
        auto val = my_class_wrapper::find_object(isolate, value);
        if (val.IsEmpty())
        {
            val = my_class_wrapper::reference_external(isolate, value);
        }
        return val;
    }

    static to_type to_v8(v8::Isolate* isolate, fse::Scene* value)
    {
        auto val = my_class_wrapper_raw::find_object(isolate, value);
        if (val.IsEmpty())
        {
            val = my_class_wrapper_raw::reference_external(isolate, value);
        }
        return val;
    }
};
//
//template<>
//struct v8pp::is_wrapped_class<std::shared_ptr<fse::Scene>> : std::false_type {};
//
//
//template<>
//struct v8pp::convert<fse::FSEObject*>
//{
//	using from_type = fse::FSEObject*;
//	using to_type = v8::Local<v8::Object>;
//
//	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected FSEObject");
//		}
//
//		return my_class_wrapper::unwrap_object(isolate, value);
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, fse::FSEObject* const& value)
//	{
//		auto val = my_class_wrapper::find_object(isolate, value);
//		if (val.IsEmpty())
//		{
//			for (auto& func : fse::fseV8DownCastHelpers<fse::FSEObject>)
//			{
//				val = func(value, isolate);
//				if (!val.IsEmpty())
//					return val;
//			}
//			val = my_class_wrapper::reference_external(isolate, value);
//		}
//		return val;
//	}
//};
//
//template<>
//struct v8pp::is_wrapped_class<fse::FSEObject*> : std::false_type {};
//
//template<>
//struct v8pp::convert<std::shared_ptr<fse::FSEObject>>
//{
//	using from_type = std::shared_ptr<fse::FSEObject>;
//	using from_type_raw = fse::FSEObject*;
//	using to_type = v8::Local<v8::Object>;
//
//	typedef v8pp::class_<fse::FSEObject, v8pp::shared_ptr_traits> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected FSEObject");
//		}
//
//		return std::shared_ptr<fse::FSEObject>(my_class_wrapper::unwrap_object(isolate, value));
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, std::shared_ptr<fse::FSEObject> const& value)
//	{
//		auto val = my_class_wrapper::find_object(isolate, value);
//		if (val.IsEmpty())
//		{
//			for (auto& func : fse::fseV8SharedPtrDownCastHelpers<fse::FSEObject>)
//			{
//				val = func(value, isolate);
//				if (!val.IsEmpty())
//					return val;
//			}
//			val = my_class_wrapper::reference_external(isolate, value);
//		}
//		return val;
//	}
//
//};
//
//template<>
//struct v8pp::is_wrapped_class<std::shared_ptr<fse::FSEObject>> : std::false_type {};
//
//
template<>
struct v8pp::convert<std::weak_ptr<fse::FSEObject>>
{
    using from_type = std::weak_ptr<fse::FSEObject>;
    //using from_type_raw = fse::FSEObject*;
    using to_type = v8::Local<v8::Object>;

    typedef v8pp::class_<fse::FSEObject, v8pp::shared_ptr_traits> my_class_wrapper;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
    {
        return !value.IsEmpty();
    }

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
    {
        if (!is_valid(isolate, value))
        {
            throw std::invalid_argument("expected FSEObject");
        }

        return std::shared_ptr<fse::FSEObject>(my_class_wrapper::unwrap_object(isolate, value));
    }

    static to_type to_v8(v8::Isolate* isolate, std::weak_ptr<fse::FSEObject> const& value)
    {
        if (value.expired())
        {
            throw std::invalid_argument("Value is expired");
        }
        const auto shared_ptr = value.lock();

        auto val = my_class_wrapper::find_object(isolate, shared_ptr);
        if (val.IsEmpty())
        {
            val = convert<std::shared_ptr<fse::FSEObject>>::to_v8(isolate, shared_ptr);
        }
        return val;
    }
};

template<>
struct v8pp::is_wrapped_class<std::weak_ptr<fse::FSEObject>> : std::false_type {};
//
//
//template<>
//struct v8pp::convert<std::vector<std::shared_ptr<fse::FSEObject>>>
//{
//	using from_type = std::vector<std::shared_ptr<fse::FSEObject>>;
//	//using from_type_raw = std::vector<fse::FSEObject*>;
//	using to_type = v8::Local<v8::Array>;
//
//	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty() && value->IsArray();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected FSEObject array");
//		}
//
//		v8::Local<v8::Array> arr = value.As<v8::Array>();
//
//		from_type result;
//		result.reserve(arr->Length());
//		for (unsigned i = 0; i < arr->Length(); ++i)
//		{
//			result.push_back(
//				convert<std::shared_ptr<fse::FSEObject>>
//				::from_v8(isolate, arr->Get(isolate->GetCurrentContext(), i).ToLocalChecked()));
//		}
//
//		return result;
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, std::vector<std::shared_ptr<fse::FSEObject>> const& value)
//	{
//		v8::EscapableHandleScope scope(isolate);
//
//		v8::Local<v8::Array> arr = v8::Array::New(isolate, value.size());
//		for (unsigned i = 0; i < arr->Length(); ++i) {
//			arr->Set(isolate->GetCurrentContext(), i,
//				convert<std::shared_ptr<fse::FSEObject>>
//				::to_v8(isolate, value[i]));
//		}
//
//		return scope.Escape(arr);
//	}
//};
//template<>
//struct v8pp::is_wrapped_class<std::vector<std::shared_ptr<fse::FSEObject>> > : std::false_type {};
//
//
//template<>
//struct v8pp::convert<fse::Component*>
//{
//	using from_type = fse::Component*;
//	using to_type = v8::Local<v8::Object>;
//
//	typedef v8pp::class_<fse::Component> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected Component");
//		}
//
//		return my_class_wrapper::unwrap_object(isolate, value);
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, fse::Component* const& value)
//	{
//		auto val = my_class_wrapper::find_object(isolate, value);
//		if (val.IsEmpty())
//		{
//			for (auto& func : fse::fseV8DownCastHelpers<fse::Component>)
//			{
//				val = func(value, isolate);
//				if (!val.IsEmpty())
//					return val;
//			}
//			val = my_class_wrapper::reference_external(isolate, value);
//		}
//		return val;
//	}
//};
//
//template<>
//struct v8pp::is_wrapped_class<fse::Component*> : std::false_type {};
//
//template<>
//struct v8pp::convert<std::shared_ptr<fse::Component>>
//{
//	using from_type = std::shared_ptr<fse::Component>;
//	using from_type_raw = fse::Component*;
//	using to_type = v8::Local<v8::Object>;
//
//	typedef v8pp::class_<fse::Component, v8pp::shared_ptr_traits> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected Component");
//		}
//
//		return std::shared_ptr<fse::Component>(my_class_wrapper::unwrap_object(isolate, value));
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, std::shared_ptr<fse::Component> const& value)
//	{
//		auto val = my_class_wrapper::find_object(isolate, value);
//		if (val.IsEmpty())
//		{
//			for (auto& func : fse::fseV8SharedPtrDownCastHelpers<fse::Component>)
//			{
//				val = func(value, isolate);
//				if (!val.IsEmpty())
//					return val;
//			}
//			val = my_class_wrapper::reference_external(isolate, value);
//		}
//		return val;
//	}
//
//};
//
//template<>
//struct v8pp::is_wrapped_class<std::shared_ptr<fse::Component>> : std::false_type {};
//
//
template<>
struct v8pp::convert<std::weak_ptr<fse::Component>>
{
    using from_type = std::weak_ptr<fse::Component>;
    //using from_type_raw = fse::Component*;
    using to_type = v8::Local<v8::Object>;

    typedef v8pp::class_<fse::Component, v8pp::shared_ptr_traits> my_class_wrapper;

    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
    {
        return !value.IsEmpty();
    }

    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
    {
        if (!is_valid(isolate, value))
        {
            throw std::invalid_argument("expected Component");
        }

        return std::shared_ptr<fse::Component>(my_class_wrapper::unwrap_object(isolate, value));
    }

    static to_type to_v8(v8::Isolate* isolate, std::weak_ptr<fse::Component> const& value)
    {
        if (value.expired())
        {
            throw std::invalid_argument("Value is expired");
        }
        const auto shared_ptr = value.lock();

        auto val = my_class_wrapper::find_object(isolate, shared_ptr);
        if (val.IsEmpty())
        {
            val = convert<std::shared_ptr<fse::Component>>::to_v8(isolate, shared_ptr);
        }
        return val;
    }
};

template<>
struct v8pp::is_wrapped_class<std::weak_ptr<fse::Component>> : std::false_type {};
//
//
//template<>
//struct v8pp::convert<std::vector<std::shared_ptr<fse::Component>>>
//{
//	using from_type = std::vector<std::shared_ptr<fse::Component>>;
//	//using from_type_raw = std::vector<fse::Component*>;
//	using to_type = v8::Local<v8::Array>;
//
//	typedef v8pp::class_<fse::Component> my_class_wrapper;
//
//	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//	{
//		return !value.IsEmpty() && value->IsArray();
//	}
//
//	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//	{
//		if (!is_valid(isolate, value))
//		{
//			throw std::invalid_argument("expected Component array");
//		}
//
//		v8::Local<v8::Array> arr = value.As<v8::Array>();
//
//		from_type result;
//		result.reserve(arr->Length());
//		for (unsigned i = 0; i < arr->Length(); ++i)
//		{
//			result.push_back(
//				convert<std::shared_ptr<fse::Component>>
//				::from_v8(isolate, arr->Get(isolate->GetCurrentContext(), i).ToLocalChecked()));
//		}
//
//		return result;
//	}
//
//	static to_type to_v8(v8::Isolate* isolate, std::vector<std::shared_ptr<fse::Component>> const& value)
//	{
//		v8::EscapableHandleScope scope(isolate);
//
//		v8::Local<v8::Array> arr = v8::Array::New(isolate, value.size());
//		for (unsigned i = 0; i < arr->Length(); ++i) {
//			arr->Set(isolate->GetCurrentContext(), i,
//				convert<std::shared_ptr<fse::Component>>
//				::to_v8(isolate, value[i]));
//		}
//
//		return scope.Escape(arr);
//	}
//};
//template<>
//struct v8pp::is_wrapped_class<std::vector<std::shared_ptr<fse::Component>> > : std::false_type {};


//template<typename... Args>
//struct v8pp::convert<SConnection<Args...>*>
//{
//    using from_type = SConnection<Args...>*;
//    using to_type = v8::Local<v8::Object>;
//
//    typedef v8pp::class_<SConnection<Args...>> my_class_wrapper;
//
//    static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
//    {
//        return !value.IsEmpty();
//    }
//
//    static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
//    {
//        if (!is_valid(isolate, value))
//        {
//            throw std::invalid_argument("expected SConnection<Args...>");
//        }
//
//        return my_class_wrapper::unwrap_object(isolate, value);
//    }
//
//    static to_type to_v8(v8::Isolate* isolate, SConnection<Args...>* const& value)
//    {
//        auto val = my_class_wrapper::find_object(isolate, value);
//        if (val.IsEmpty())
//        {
//            val = my_class_wrapper::import_external(isolate, value);
//            // val->Set(isolate->GetCurrentContext(), v8pp::to_v8(isolate, "disconnect"), v8pp::to_v8("meh"));
//        }
//        return val;
//    }
//
//};
//template<typename... Args>
//struct v8pp::is_wrapped_class<SConnection<Args...>*> : std::false_type {};
