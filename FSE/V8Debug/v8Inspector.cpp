#include "v8Inspector.h"
#include "v8DebugUtils.h"


FSEInspector::FSEInspector(v8::Platform* platform, const int webSocketPort, bool block) {
	block_ = block;
	websocket_server_ = std::make_unique<WebSocketServer>(webSocketPort,
		std::bind(&FSEInspector::onMessage, this,
			std::placeholders::_1));
	inspector_client_ = std::make_unique<V8InspectorClientImpl>(platform, v8::Isolate::GetCurrent()->GetCurrentContext(),
		std::bind(&FSEInspector::sendMessage, this,
			std::placeholders::_1), std::bind(
				&FSEInspector::waitForFrontendMessage, this));
}

void FSEInspector::onMessage(const std::string& message) const
{
	//std::cout << "CDT message: " << message << std::endl;
	const v8_inspector::StringView protocolMessage = convertToStringView(message);
	inspector_client_->dispatchProtocolMessage(protocolMessage);

	const v8::Local<v8::Object> jsonObject = parseJson(v8::Isolate::GetCurrent()->GetCurrentContext(), message);
	if (!jsonObject.IsEmpty()) {
		const std::string method = getPropertyFromJson(v8::Isolate::GetCurrent(), jsonObject, "method");
		if (block_ && method == "Runtime.runIfWaitingForDebugger") {
			inspector_client_->schedulePauseOnNextStatement(convertToStringView("Inspect-Break"));
		}
	}
}

void FSEInspector::sendMessage(const std::string& message) const
{
	//std::cout << "Message to frontend: " << message << std::endl;
	websocket_server_->sendMessage(message);
}

void FSEInspector::startAgent() const
{
	websocket_server_->run(block_);
}

void FSEInspector::startAgentBlocking() const
{
	websocket_server_->run(true);
}

void FSEInspector::poll() const
{
	v8::Isolate* iso = v8::Isolate::GetCurrent();
	v8::HandleScope handle_scope(iso);
	websocket_server_->poll();
}

void FSEInspector::quit() const
{
	websocket_server_->quit();
	inspector_client_->quit();
}

void FSEInspector::waitForFrontendMessage() const
{
	websocket_server_->waitForFrontendMessageOnPause();
}
