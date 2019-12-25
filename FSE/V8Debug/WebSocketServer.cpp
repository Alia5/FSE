#include "WebSocketServer.h"

WebSocketServer::WebSocketServer(unsigned short port, std::function<void(std::string)> onMessage)
{
	port_ = port;
	on_message_ = std::move(onMessage);
	//run();
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
		// store the connection so events can be sent back to the client without the client sending something first
		ws_server_.set_open_handler(bind(&WebSocketServer::on_open, this, websocketpp::lib::placeholders::_1));
		// note that the client disconnected
		ws_server_.set_close_handler(
			bind(&WebSocketServer::on_close, this, websocketpp::lib::placeholders::_1));
		// handle websocket messages from the client
		ws_server_.set_message_handler(
			bind(&WebSocketServer::on_message, this, websocketpp::lib::placeholders::_1,
				websocketpp::lib::placeholders::_2));
		ws_server_.set_http_handler(
			websocketpp::lib::bind(&WebSocketServer::on_http, this, websocketpp::lib::placeholders::_1));
		//ws_server_.set_interrupt_handler(bind(&WebSocketServer::on_interrupt, this, websocketpp::lib::placeholders::_1));
		ws_server_.init_asio();
		ws_server_.set_reuse_addr(true);

		//v8toolkit::log.info(LogT::Subjects::DebugWebSocket, "Debug context websocket channel listening on port: {}", this->port);
		ws_server_.listen(port_);
		ws_server_.start_accept();
		//v8toolkit::log.info(LogT::Subjects::DebugWebSocket, "Done creating WebSocketService");

		// ...
		
		//auto const address = net::ip::make_address("127.0.0.1");
		//net::io_context ioc{ 1 };
		//tcp::acceptor acceptor{ ioc, {address, static_cast<unsigned short>(port_)} };
		printListeningMessage();

		//tcp::socket socket{ ioc };
		//acceptor.accept(socket);
		//ws_ = std::unique_ptr<websocket::stream<tcp::socket>>(new websocket::stream<tcp::socket>(std::move(socket)));
		
		startListening();
	}
	catch (const std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void WebSocketServer::sendMessage(const std::string& message)
{
	//try {
	//	boost::beast::multi_buffer b;
	//	boost::beast::ostream(b) << message;

	//	ws_->text(ws_->got_text());
	//	ws_->write(b.data());
	//}
	//catch (beast::system_error const& se) {
	//	if (se.code() != websocket::error::closed)
	//		std::cerr << "Error: " << se.code().message() << std::endl;
	//}
	//catch (std::exception const& e)
	//{
	//	std::cerr << "Error: " << e.what() << std::endl;
	//}
	ws_server_.pause_reading(connections_[0]);
	std::string msg(message);
	ws_server_.send(connections_[0], message, websocketpp::frame::opcode::TEXT);
	ws_server_.resume_reading(connections_[0]);
	
}

void WebSocketServer::startListening()
{
	//try {
	//	ws_->accept();
	//	while (true) {
	//		waitFrontendMessage();
	//	}
	//}
	//catch (beast::system_error const& se) {
	//	if (se.code() != websocket::error::closed)
	//		std::cerr << "Error: " << se.code().message() << std::endl;
	//}
	//catch (std::exception const& e) {
	//	std::cerr << "Error: " << e.what() << std::endl;
	//}

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
	//beast::flat_buffer buffer;
	//ws_->read(buffer);
	//std::string message = boost::beast::buffers_to_string(buffer.data());
	//on_message_(std::move(message));
	//if (!connections_.empty())
	//{
	//	ws_server_.interrupt(connections_[0]);
	//}
	ws_server_.run_one();
}


bool WebSocketServer::websocket_validation_handler(websocketpp::connection_hdl hdl) {
	// only allow one connection
	//log.info(LogT::Subjects::DebugWebSocket, "Websocket validation handler, checking that connection count is 0, not sure why");
	return connections_.size() == 0;
}


void WebSocketServer::on_message(websocketpp::connection_hdl hdl, WebSocketServer::WSServer::message_ptr msg) {
	//std::smatch matches;
	std::string message_payload = msg->get_payload();
	std::cout << "ws message: " << message_payload << "\n";
	on_message_(std::move(message_payload));

	//log.info(LogT::Subjects::DebugWebSocket, "Got websocket message: {}", message_payload);

	////    this->message_manager.process_request_message(message_payload);
	//v8_inspector::StringView message_view((uint8_t const*)message_payload.c_str(), message_payload.length());

	//GLOBAL_CONTEXT_SCOPED_RUN(this->debug_context.get_isolate(), this->debug_context.get_global_context());
	//this->debug_context.get_session().dispatchProtocolMessage(message_view);
	//this->message_received_time = std::chrono::high_resolution_clock::now();
}


void WebSocketServer::on_open(const websocketpp::connection_hdl& connection) {
	//log.info(LogT::Subjects::DebugWebSocket, "Got websocket connection");
	assert(connections_.size() == 0);
	connections_.emplace_back(connection);
	//this->debug_context.connect_with_channel(&this->connections.find(connection)->second);
	//this->message_received_time = std::chrono::high_resolution_clock::now();
}



void WebSocketServer::on_close(const websocketpp::connection_hdl& hdl) {
	assert(connections_.size() == 1);
	connections_.clear();
	assert(connections_.size() == 0);
	
	//log.info(LogT::Subjects::DebugWebSocket, "Websocket connection closed");

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

	// Set status to 200 rather than the default error code
	con->set_status(websocketpp::http::status_code::ok);
	// Set body text to the HTML created above
	con->set_body(output.str());
}

void WebSocketServer::on_interrupt(const websocketpp::connection_hdl& hdl)
{
}
