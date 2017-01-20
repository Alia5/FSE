#include "NetworkHandler.h"
#include <future>

NetworkHandler::NetworkHandler()
{
	is_server_ = false;
	run_thread_ = false;
}

NetworkHandler::NetworkHandler(bool server)
{
	is_server_ = server;
}

//NetworkHandler::NetworkHandler(const NetworkHandler& handler)
//{
//	//Copy constructor
//}

NetworkHandler::~NetworkHandler()
{
	if (tcp_connected_)
		disconnect();
}

void NetworkHandler::setServer(bool server)
{
	is_server_ = server;
}

void NetworkHandler::setServerIP(std::wstring ip)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string ipstring = converter.to_bytes(ip);

	this->ip_ = sf::IpAddress(ipstring);
}

bool NetworkHandler::connect()
{
	tcp_listener_.close();
	tcp_socket_.disconnect();
	udp_socket_.unbind();


	if (is_server_)
	{

		std::wcout << L"Binding UPD connection...\n";
		if (udp_socket_.bind(udp_port_) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't bind port for listening!\n";
		} 


		std::wcout << L"Binding TCP connection...\n";

		if (tcp_listener_.listen(tcp_port_) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't bind port for listening!\n";
		}

		if (tcp_listener_.accept(tcp_socket_) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't accept connection\n";
		} else {
			std::wcout << L"Connected!\n";
			tcp_connected_ = true;

			ip_ = tcp_socket_.getRemoteAddress();

			run_thread_ = true;
			std::wcout << L"Firing SERVER thread!!!" << std::endl;
			network_thread_ = std::thread(&NetworkHandler::netThreadRunServer, this);

			return true;

		}


	} else {
		
		sf::Socket::Status status = tcp_socket_.connect(ip_, tcp_port_);
		if (status != sf::Socket::Done)
		{
			std::wcout << L"Couldn't connect\n";
		} else {
			std::wcout << L"Connected!\n";
			tcp_connected_ = true;

			std::wcout << L"Binding UPD connection...\n";
			if (udp_socket_.bind(udp_port_client_) != sf::Socket::Done)
			{
				std::wcout << L"Couldn't bind port for listening!\n";
			}

			run_thread_ = true;
			std::wcout << L"Firing CLIENT thread!!!" << std::endl;
			network_thread_ = std::thread(&NetworkHandler::netThreadRunClient, this);

			return true;

		}

	}

	return false;

}

void NetworkHandler::disconnect()
{
	run_thread_ = false;
	udp_socket_.unbind();
	tcp_socket_.disconnect();
	tcp_listener_.close();
	network_thread_.join();
	tcp_connected_ = false;
}

void NetworkHandler::sendPacket(sf::Packet packet, bool important)
{
	if (important)
	{
		
	} else {
		
	}

	mtx_.lock();
	if (udp_message_queue_.size() > 1000)
		udp_message_queue_.pop_front();
	udp_message_queue_.push_back(packet);
	mtx_.unlock();
}

sf::Packet NetworkHandler::getLatestUDPPacket()
{
	sf::Packet temp;
	mtx_.lock();
		temp = latest_udp_packet_;
	mtx_.unlock();
	return temp;
}

sf::Packet NetworkHandler::getLatestUDPPacket(uint32_t objectID)
{
	sf::Packet pack;

	uint32_t id;
	float32 timestamp = 0;
	float32 packetStamp = 0;

	mtx_.lock();
	for (auto packet = udp_received_packets_queue_.begin(); packet != udp_received_packets_queue_.end(); ++packet)
	{
		if (!(*packet).endOfPacket())
		{
			(*packet) >> id >> packetStamp;
			if (id == objectID && packetStamp > timestamp)
			{
				pack = (*packet);
				timestamp = packetStamp;
				udp_received_packets_queue_.erase(packet--);
			}
		}
	}
	mtx_.unlock();

	return pack;
}

void NetworkHandler::netThreadRunServer()
{
	//sf::Packet packet;
	//while (run_thread_)
	//{
	//	udp_socket_.receive(packet, ip_, udp_port_client_);
	//	uint8_t type;
	//	packet >> type;
	//	switch (type)
	//	{
	//	case 0x01:
	//		std::wcout << L"Received KeepAlive packet!\n";
	//		break;
	//	case 0x02:
	//		std::wcout << L"Received 0x02 packet!\n";
	//		break;
	//	case 0x03:
	//		std::wcout << L"Received 0x03 packet!\n";
	//		break;
	//	}
	//	//std::flush(std::wcout);

	//	sf::Packet packet;
	//	packet << 0x02;
	//	udp_socket_.send(packet, ip_, udp_port_);

	//	sf::sleep(sf::milliseconds(10));
	//}


	sock_selector_.add(udp_socket_);
	uint8_t type;

	sf::Packet packet;

	std::wcout << L"Waiting to receive initial KeepAlive packet!\n";

	udp_socket_.receive(packet, ip_, udp_port_client_);
	packet >> type;

	std::wcout << "OK!\n";

	//sf::Clock timer;

	srand(time(nullptr));

	while (run_thread_)
	{

		if (sock_selector_.wait(sf::seconds(1.f/60.f)))
		{
			if (sock_selector_.isReady(udp_socket_))
			{
				sf::Packet pack;
				udp_socket_.receive(pack, ip_, udp_port_client_);


				//uint8_t typ;
				//pack >> typ;

				//if (typ == 0x01)
				//{
				//	std::wcout << "Received KeepAlive Packet!\n";
				//} 

				mtx_.lock();
				latest_udp_packet_ = pack;

				if (udp_received_packets_queue_.size() > 1000)
					udp_received_packets_queue_.pop_front();
				udp_received_packets_queue_.push_back(pack);

				mtx_.unlock();
			

			}
			//timer.restart();
		}
		else
		{
			mtx_.lock();
			std::list<sf::Packet> toSendPacks;
			if (udp_message_queue_.size() > 0)
			{
				toSendPacks = udp_message_queue_;
				udp_message_queue_.clear();
			}
			mtx_.unlock();

			//sf::sleep(sf::milliseconds(50));
			//sf::sleep(sf::milliseconds(rand() % 51));
			for (auto& toSendPacket : toSendPacks)
			{
				udp_socket_.send(toSendPacket, ip_, udp_port_client_);
				//std::wcout << "Sending position packet!!!\n";
			}
		}
	}

	sock_selector_.remove(udp_socket_);

}

void NetworkHandler::netThreadRunClient()
{

	sock_selector_.add(udp_socket_);
	uint8_t type;

	sf::Packet packet;
	type = 0x01;

	std::wcout << L"Sending Initial KeepAlive packet!\n";

	packet << type;
	udp_socket_.send(packet, ip_, udp_port_);

	std::wcout << "OK!\n";

	sf::Clock timer;

	while (run_thread_)
	{
		
		if (sock_selector_.wait(sf::seconds(1.f/60.f)))
		{
			if (sock_selector_.isReady(udp_socket_))
			{
				sf::Packet pack;
				udp_socket_.receive(pack, ip_, udp_port_);

				mtx_.lock();
				latest_udp_packet_ = pack;

				if (udp_received_packets_queue_.size() > 1000)
					udp_received_packets_queue_.pop_front();
				udp_received_packets_queue_.push_back(pack);

				mtx_.unlock();
			}

			timer.restart();
		} else {

			mtx_.lock();
			std::list<sf::Packet> toSendPacks;
			if (udp_message_queue_.size() > 0)
			{
				toSendPacks = udp_message_queue_;
				udp_message_queue_.clear();
			}
			mtx_.unlock();

			//sf::sleep(sf::milliseconds(50));
			//sf::sleep(sf::milliseconds(rand() % 51));
			for (auto& toSendPacket : toSendPacks)
			{
				udp_socket_.send(toSendPacket, ip_, udp_port_);
				//std::wcout << "Sending position packet!!!\n";
			}

			if (timer.getElapsedTime().asSeconds() > 1)
			{
				std::wcout << L"Sending KeepAlive packet!\n";
				sf::Packet packet;
				type = 0x01;
				packet << type;
				udp_socket_.send(packet, ip_, udp_port_);

				timer.restart();
				//TODO: Some timeout_ stuff
			}
		}
	}

	sock_selector_.remove(udp_socket_);
}

void NetworkHandler::swap(const NetworkHandler& other)
{
	is_server_ = other.is_server_;
}

sf::Socket::Status NetworkHandler::receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout)
{
	sf::SocketSelector selector;
	selector.add(socket);
	if (selector.wait(timeout))
		return socket.receive(packet);
	return sf::Socket::NotReady;
}

NetworkHandler& NetworkHandler::operator=(const NetworkHandler& handler)
{
	swap(handler);
	return *this;
}
