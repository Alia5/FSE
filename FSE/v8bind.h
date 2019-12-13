#pragma once
#include <v8.h>

#include<type_traits>
#include <utility>
#include <functional>
namespace fse
{

	namespace  v8bind
	{

		
		class module
		{
		public:
			explicit module(v8::Isolate* isolate) : isolate_(isolate){
				object_template_ = v8::ObjectTemplate::New(isolate_);
			}

			template<typename Function>
			auto function(std::string_view name, Function&& func)
			{
				object_template_->Set(v8::String::NewFromUtf8(isolate_, name.data()).ToLocalChecked(), func );
				return *this;
			}
			auto create() {
				return object_template_->NewInstance(isolate_->GetCurrentContext()).ToLocalChecked();
			}
			
		private:
			v8::Isolate* isolate_;
			v8::Local<v8::ObjectTemplate> object_template_;
		};



		
	}
	
}
