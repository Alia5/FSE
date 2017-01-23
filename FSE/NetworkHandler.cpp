#include "NetworkHandler.h"

NetworkHandler::NetworkHandler()
{
	is_server_ = false;
	run_thread_ = false;
}

NetworkHandler::NetworkHandler(bool server)
{
	is_server_ = server;
}

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
			std::wcout << L"Couldn't bind UDP port for listening!\n";
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

			network_clock_.restart();

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
				std::wcout << L"Couldn't bind UDP port for listening!\n";
			}

			run_thread_ = true;
			std::wcout << L"Firing CLIENT thread!!!" << std::endl;
			network_thread_ = std::thread(&NetworkHandler::netThreadRunClient, this);

			network_clock_.restart();

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

void NetworkHandler::sendPacket(sf::Packet& packet, bool tcp)
{
	if (tcp)
	{
		sf::Packet pack;
		pack << static_cast<float32>(network_clock_.getElapsedTime().asSeconds());
		uint8_t byte;
		while (!packet.endOfPacket()) // Why this way SFML?
		{
			packet >> byte;
			pack << byte;
		}

		mtx_.lock();
		if (tcp_message_queue_.size() > 1000)
			tcp_message_queue_.pop_front();
		tcp_message_queue_.push_back(pack);
		mtx_.unlock();
	} else {
		sf::Packet pack;
		pack << static_cast<float32>(network_clock_.getElapsedTime().asSeconds());
		uint8_t byte;
		while (!packet.endOfPacket()) // Why this way SFML?
		{
			packet >> byte;
			pack << byte;
		}

		mtx_.lock();
		if (udp_message_queue_.size() > 1000)
			udp_message_queue_.pop_front();
		udp_message_queue_.push_back(pack);
		mtx_.unlock();
	}
}




std::vector<sf::Packet> NetworkHandler::getUdpPackets(uint32_t objectID)
{
	std::vector<sf::Packet> packs;

	uint32_t id;
	float32 packetStamp = 0;

	std::list<std::list<sf::Packet>::iterator> to_delete;
	mtx_.lock();

	auto packet = udp_received_packets_queue_.begin();
	while (packet != udp_received_packets_queue_.end())
	{
		if (!(*packet).endOfPacket())
		{
			(*packet) >> packetStamp >> id;
			if (id == objectID)
			{
				(*packet) << packetStamp;
				packs.push_back(*packet);
				to_delete.push_back(packet);
			}
		}
		++packet;
	}
	for (auto td = udp_received_packets_queue_.begin(); packet != udp_received_packets_queue_.end(); ++td)
	{
		udp_received_packets_queue_.erase(td);
	}
	mtx_.unlock();

	return packs;
}

std::vector<sf::Packet> NetworkHandler::getTcpPackets(uint32_t objectID)
{
	std::vector<sf::Packet> packs;

	uint32_t id;
	float32 packetStamp = 0;

	std::list<std::list<sf::Packet>::iterator> to_delete;
	mtx_.lock();

	auto packet = tcp_received_packets_queue_.begin();
	while (packet != tcp_received_packets_queue_.end())
	{
		if (!(*packet).endOfPacket())
		{
			(*packet) >> packetStamp >> id;
			if (id == objectID)
			{
				(*packet) << packetStamp;
				packs.push_back(*packet);
				to_delete.push_back(packet);
			}
		}
		++packet;
	}
	for (auto td = tcp_received_packets_queue_.begin(); packet != tcp_received_packets_queue_.end(); ++td)
	{
		tcp_received_packets_queue_.erase(td);
	}
	mtx_.unlock();

	return packs;
}

void NetworkHandler::netThreadRunServer()
{
	sock_selector_.add(udp_socket_);
	sock_selector_.add(tcp_socket_);
	uint8_t type;

	sf::Packet packet;

	std::wcout << L"Waiting to receive initial KeepAlive packet!\n";

	udp_socket_.receive(packet, ip_, udp_port_client_);
	packet >> type;

	std::wcout << "OK!\n";

	srand(time(nullptr));

	while (run_thread_)
	{

		if (sock_selector_.wait(sf::seconds(1.f/60.f)))
		{
			if (sock_selector_.isReady(udp_socket_))
			{
				sf::Packet pack;
				udp_socket_.receive(pack, ip_, udp_port_client_);

				mtx_.lock();

				if (udp_received_packets_queue_.size() > 1000)
					udp_received_packets_queue_.pop_front();
				udp_received_packets_queue_.push_back(pack);

				mtx_.unlock();			
			} 
			else if (sock_selector_.isReady(tcp_socket_))
			{
				sf::Packet pack;
				tcp_socket_.receive(pack);
				mtx_.lock();
				if (tcp_received_packets_queue_.size() > 1000)
					tcp_received_packets_queue_.pop_front();
				tcp_received_packets_queue_.push_back(pack);
				mtx_.unlock();
			}
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

			for (auto& toSendPacket : toSendPacks)
			{
				udp_socket_.send(toSendPacket, ip_, udp_port_client_);
			}

			toSendPacks.clear();
			mtx_.lock();
			if (tcp_message_queue_.size() > 0)
			{
				toSendPacks = tcp_message_queue_;
				tcp_message_queue_.clear();
			}
			mtx_.unlock();
			for (auto& toSendPacket : toSendPacks)
			{
				tcp_socket_.send(toSendPacket);
			}
		}
	}

	sock_selector_.remove(udp_socket_);
	sock_selector_.remove(tcp_socket_);
}

void NetworkHandler::netThreadRunClient()
{

	sock_selector_.add(udp_socket_);
	sock_selector_.add(tcp_socket_);
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

				if (udp_received_packets_queue_.size() > 1000)
					udp_received_packets_queue_.pop_front();
				udp_received_packets_queue_.push_back(pack);

				mtx_.unlock();
			} else if (sock_selector_.isReady(tcp_socket_))
			{
				sf::Packet pack;
				tcp_socket_.receive(pack);
				mtx_.lock();
				if (tcp_received_packets_queue_.size() > 1000)
					tcp_received_packets_queue_.pop_front();
				tcp_received_packets_queue_.push_back(pack);
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

			for (auto& toSendPacket : toSendPacks)
			{
				udp_socket_.send(toSendPacket, ip_, udp_port_);
			}

			toSendPacks.clear();
			mtx_.lock();
			if (tcp_message_queue_.size() > 0)
			{
				toSendPacks = tcp_message_queue_;
				tcp_message_queue_.clear();
			}
			mtx_.unlock();
			for (auto& toSendPacket : toSendPacks)
			{
				tcp_socket_.send(toSendPacket);
			}

			if (timer.getElapsedTime().asSeconds() > 1)
			{
				std::wcout << L"Sending KeepAlive packet!\n";
				sf::Packet packet;
				type = 0x01;
				packet << type;
				udp_socket_.send(packet, ip_, udp_port_);

				timer.restart();
			}
		}
	}

	sock_selector_.remove(udp_socket_);
	sock_selector_.remove(tcp_socket_);
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
