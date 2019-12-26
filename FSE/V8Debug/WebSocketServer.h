#pragma once
#include <memory>
#include <string>
#include <functional>

#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "v8InspectorChannel.h"
#include <SFML/Network/TcpSocket.hpp>

class WebSocketServer
{
public:
	WebSocketServer(unsigned short port, std::function<void(std::string)> onMessage);

	using WSServer = websocketpp::server<websocketpp::config::asio>;
	using WSConnections = std::vector<websocketpp::connection_hdl>;

	
	void run(bool block = true);
	void sendMessage(const std::string& message);
	void waitForFrontendMessageOnPause();
	void poll();
	void quit();
private:
	void startListening();
	void printListeningMessage() const;
	void waitFrontendMessage();

	unsigned short port_;
	std::function<void(std::string)> on_message_;
	std::vector<std::string> receive_buffer_;
	bool poll_ = true;
	
	WSServer ws_server_;
	WSConnections connections_;
	bool websocket_validation_handler(websocketpp::connection_hdl hdl);
	void on_open(const websocketpp::connection_hdl& hdl);
	void on_close(const websocketpp::connection_hdl& hdl);
	void on_message(websocketpp::connection_hdl hdl, WSServer::message_ptr msg);
	void on_http(const websocketpp::connection_hdl& hdl);
	void on_interrupt(const websocketpp::connection_hdl& hdl);
};
