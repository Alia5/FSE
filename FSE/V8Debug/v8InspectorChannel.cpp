#include <functional>
#include "v8InspectorChannel.h"
#include <v8pp/call_from_v8.hpp>
#include "v8DebugUtils.h"

V8InspectorChannelImp::V8InspectorChannelImp(v8::Isolate* isolate, const std::function<void(std::string)>& onResponse) {
	isolate_ = isolate;
	on_response_ = onResponse;
}

void V8InspectorChannelImp::sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message) {
	const std::string response = convertToString(isolate_, message->string());
	on_response_(response);
}

void V8InspectorChannelImp::sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message) {
	const std::string notification = convertToString(isolate_, message->string());
	on_response_(notification);
}

void V8InspectorChannelImp::flushProtocolNotifications() {
	// flush protocol notification
}
