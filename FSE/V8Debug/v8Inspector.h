#pragma once


#include <iostream>
#include <functional>
#include <vector>
#include "v8InspectorClient.h"
#include "WebSocketServer.h"

class Inspector {
public:
	Inspector(v8::Platform* platform, const v8::Local<v8::Context>& context, int webSocketPort);
	void startAgent() const;
	void addFileForInspection(const std::string& filePath);
private:
	void executeScripts();
	void onMessage(const std::string& message);
	void sendMessage(const std::string& message) const;
	bool compileScript(const v8::Local<v8::String>& source, const std::string& filePath, v8::Local<v8::Script>& script, const v8::TryCatch& tryCatch) const;
	bool executeScript(const v8::Local<v8::Script>& script, const v8::TryCatch& tryCatch) const;
	int waitForFrontendMessage() const;

	v8::Handle<v8::Context> context_;
	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;
	std::vector<std::string> scripts = {};

};
