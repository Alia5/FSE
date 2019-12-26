#include "WebSocketServer.h"

#include <SFML/System/Clock.hpp>

WebSocketServer::WebSocketServer(unsigned short port, std::function<void(std::string)> onMessage)
{
	port_ = port;
	on_message_ = std::move(onMessage);
}

void WebSocketServer::run() {
	try
	{
		//ws_server_.set_error_channels(websocketpp::log::elevel::all);
		//ws_server_.set_access_channels(websocketpp::log::elevel::all ^ websocketpp::log::alevel::frame_payload);

		// disables all websocketpp debugging messages
		ws_server_.get_alog().clear_channels(websocketpp::log::alevel::all);
		ws_server_.get_elog().clear_channels(websocketpp::log::elevel::all);
		
		// only allow one connection
		ws_server_.set_validate_handler(
			bind(&WebSocketServer::websocket_validation_handler, this, websocketpp::lib::placeholders::_1));
		ws_server_.set_open_handler(bind(&WebSocketServer::on_open, this, websocketpp::lib::placeholders::_1));
		ws_server_.set_close_handler(
			bind(&WebSocketServer::on_close, this, websocketpp::lib::placeholders::_1));
		ws_server_.set_message_handler(
			bind(&WebSocketServer::on_message, this, websocketpp::lib::placeholders::_1,
				websocketpp::lib::placeholders::_2));
		ws_server_.set_http_handler(
			websocketpp::lib::bind(&WebSocketServer::on_http, this, websocketpp::lib::placeholders::_1));
		ws_server_.init_asio();
		ws_server_.set_reuse_addr(true);

		ws_server_.listen(port_);
		ws_server_.start_accept();

		printListeningMessage();
		startListening();
	}
	catch (const std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void WebSocketServer::sendMessage(const std::string& message)
{
	ws_server_.send(connections_[0], message, websocketpp::frame::opcode::TEXT);
}

void WebSocketServer::startListening()
{
	try {
		while (true) {
			waitFrontendMessage();
		}
	}
	catch (std::exception const& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	
}

void WebSocketServer::printListeningMessage() {
	std::cout << "WebSocket based Inspector Agent started" << std::endl;
	std::cout << "Open the following link in your Chrome/Chromium browser: chrome-devtools://devtools/bundled/inspector.html?experiments=true&v8only=true&ws=127.0.0.1:" << port_ << std::endl;
}

void WebSocketServer::waitForFrontendMessageOnPause() {
	waitFrontendMessage();
}

void WebSocketServer::poll()
{
	ws_server_.poll();
}

void WebSocketServer::waitFrontendMessage()
{
	while(receive_buffer_.empty())
	{
		ws_server_.poll();
	}
	sf::Clock clock;
	while (clock.getElapsedTime().asMilliseconds() < 50)
	{
		ws_server_.poll();
	}
	while (!receive_buffer_.empty())
	{
		const auto tmp = receive_buffer_[0];
		receive_buffer_.erase(receive_buffer_.begin());
		on_message_(tmp);
	}
}


bool WebSocketServer::websocket_validation_handler(websocketpp::connection_hdl hdl) {
	return connections_.empty();
}


void WebSocketServer::on_message(websocketpp::connection_hdl hdl, WebSocketServer::WSServer::message_ptr msg) {
	std::string message_payload = msg->get_payload();
	receive_buffer_.push_back(message_payload);
}


void WebSocketServer::on_open(const websocketpp::connection_hdl& connection) {
	assert(connections_.size() == 0);
	connections_.emplace_back(connection);
}



void WebSocketServer::on_close(const websocketpp::connection_hdl& hdl) {
	assert(connections_.size() == 1);
	connections_.clear();
	assert(connections_.size() == 0);
	// TODO: 
	// not sure if this is right, but unpause when debugger disconnects
	////////////this->debug_context.reset_session();
	////////////this->debug_context.paused = false;
	////////////this->message_received_time = std::chrono::high_resolution_clock::now();
}

void WebSocketServer::on_http(const websocketpp::connection_hdl& hdl) {
	WSServer::connection_ptr con = ws_server_.get_con_from_hdl(hdl);
	auto resource = con->get_resource();

	std::stringstream output;
	if (resource == "/json/version")
	{
		output << R"JSON({
			"Browser": "FSE/Dev",
			"Protocol-Version" : "1.1" })JSON";
	}
	else if (resource == "/json/list" || resource == "/json")
	{
		output << R"JSON([ {
			  "description": "FSE instance",
			  "devtoolsFrontendUrl": "chrome-devtools://devtools/bundled/js_app.html?experiments=true&v8only=true&ws=localhost:9847",
			  "devtoolsFrontendUrlCompat": "chrome-devtools://devtools/bundled/inspector.html?experiments=true&v8only=true&ws=localhost:9847",
			  "faviconUrl": "https://nodejs.org/static/images/favicons/favicon.ico",
			  "id": "beb0095e-a81d-42a1-9788-12484a03f74c",
			  "title": "FSE",
			  "type": "node",
			  "url": "file://",
			  "webSocketDebuggerUrl": "ws://localhost:9847"
			} ]
			)JSON";
	}
	else
	{
		output << "<html class=\"gr__localhost\"><head></head><body data-gr-c-s-loaded=\"true\">WebSockets request was expected< / body > < / html>";
	}
	con->set_status(websocketpp::http::status_code::ok);
	con->set_body(output.str());
}