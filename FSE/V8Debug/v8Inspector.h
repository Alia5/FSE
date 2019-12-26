#pragma once


#include <iostream>
#include <functional>
#include <vector>
#include "v8InspectorClient.h"
#include "WebSocketServer.h"

class FSEInspector {
public:
	FSEInspector(v8::Platform* platform, int webSocketPort, bool block);
	void startAgent() const;
	void startAgentBlocking() const;
	void poll() const;
	void quit() const;
private:
	void onMessage(const std::string& message) const;
	void sendMessage(const std::string& message) const;
	void waitForFrontendMessage() const;

	std::unique_ptr<WebSocketServer> websocket_server_;
	std::unique_ptr<V8InspectorClientImpl> inspector_client_;
	std::vector<std::string> scripts = {};
	bool block_;

};
