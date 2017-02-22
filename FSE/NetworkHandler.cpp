#include "NetworkHandler.h"
#include <codecvt>
#include <iostream>

namespace fse
{


	NetworkHandler::NetworkHandler()
	{
		is_server_ = false;
		run_thread_ = false;
	}

	NetworkHandler::~NetworkHandler()
	{
		disconnectAll();
	}

	void NetworkHandler::setServer(bool server)
	{
		is_server_ = server;
	}

	void NetworkHandler::setMaxConnections(size_t maxConnections)
	{
		socket_mtx_.lock();
		tcp_sockets_.reserve(maxConnections);
		udp_sockets_.reserve(maxConnections);
		max_client_connections = maxConnections;
		socket_mtx_.unlock();
	}

	void NetworkHandler::setServerIP(std::wstring ip)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		std::string ipstring = converter.to_bytes(ip);

		this->server_ip_ = sf::IpAddress(ipstring);
	}

	int NetworkHandler::getPing() const
	{
		return ping_;
	}

	bool NetworkHandler::awaitConnections()
	{
		if (!is_server_)
			return false;

		if (await_connections_)
			return false;
		await_connections_ = true;

		if (!listener_bound_)
			bindListeners();

		if (listener_bound_)
		{
			network_clock_.restart();
			await_connections_thread_ = std::thread(&NetworkHandler::awaitConnectionThreadRun, this);
			return true;
		}

		return false;
	}

	void NetworkHandler::stopAwaitingConnections()
	{
		await_connections_ = false;
	}

	void NetworkHandler::disconnectAll()
	{
		if (connected_clients_ > 0 || connected_to_server_)
		{
			if (socket_mtx_.try_lock())
			{
				sf::Packet pack;
				pack << static_cast<uint8_t>(MessageType::bye);
				for (unsigned int i = 0; i < tcp_sockets_.size(); i++)
				{
					tcp_sockets_[i]->send(pack);
				}
				socket_mtx_.unlock();
			} 
		}

		run_thread_ = false;
		listener_bound_ = false;
		connected_to_server_ = false;
		await_connections_ = false;
		if (is_server_)
		{
			tcp_listener_.close();
			if (await_connections_thread_.joinable())
				await_connections_thread_.join();
		}
		if (network_thread_.joinable())
			network_thread_.join();
		for (auto& socket : tcp_sockets_)
		{
			socket->disconnect();
		}
		tcp_sockets_.clear();
		for (auto& socket : udp_sockets_)
		{
			socket->unbind();
		}
		udp_sockets_.clear();
		udp_ports_.clear();
		socket_selector.clear();
		tcp_timeout_clocks.clear();
		udp_timeout_clocks.clear();	
		connected_clients_ = 0;
		for (unsigned int i = 0; i < connected_clients_; i++)
		{
			disconnected_clients_.push_back(i);
		}
	}

	bool NetworkHandler::isConnected() const
	{
		if (connected_clients_ > 0 || connected_to_server_)
			return true;
		return false;
	}

	bool NetworkHandler::connect()
	{
		if (is_server_)
			return false;

		if (connected_to_server_)
			return true;


		auto tcpSocket = std::make_unique<sf::TcpSocket>();
		auto udpSocket = std::make_unique<sf::UdpSocket>();
		sf::Packet packet;
		if (tcpSocket->connect(server_ip_, tcp_port_, sf::seconds(5)) == sf::Socket::Done)
		{
			sf::SocketSelector selector;
			selector.add(*tcpSocket);
			if (selector.wait(sf::seconds(5)))
			{
				uint8_t type;
				tcpSocket->receive(packet);
				packet >> type;
				if (type == MessageType::initial)
				{
					unsigned short udpPort;
					packet >> udpPort;
					packet >> timestamp_offset_;

					network_clock_.restart();

					sf::Socket::Status status = sf::Socket::Error;
					for (int i = 0; i < 16; i++)
					{
						if (usable_udp_ports_[i] < udpPort)
							continue;
						status = udpSocket->bind(usable_udp_ports_[i]);
						if (status == sf::Socket::Done)
							break;
					}

					if (status == sf::Socket::Done)
					{
						selector.clear();
						selector.add(*tcpSocket);
						packet.clear();
						packet << static_cast<uint8_t>(MessageType::initial);
						packet << static_cast<uint16_t>(udpSocket->getLocalPort());
						tcpSocket->send(packet);
						udpSocket->send(packet, server_ip_, udpPort);

						if (selector.wait(sf::seconds(5)))
						{
							udpSocket->send(packet, server_ip_, udpPort);

							connected_to_server_ = true;
							socket_mtx_.lock();
							tba_tcp_sockets_.push_back(std::move(tcpSocket));
							tba_udp_sockets_.push_back(std::move(udpSocket));
							tba_udp_ports_.push_back(udpPort);
							socket_mtx_.unlock();
							if (!run_thread_)
							{
								//start the network handling thread
								run_thread_ = true;
								network_thread_ = std::thread(&NetworkHandler::netThreadRun, this);
							}
							selector.clear();
							return true;
						}
						udpSocket->unbind();
					}
				}
			}
			tcpSocket->disconnect();
		}
		return false;
	}

	void NetworkHandler::disconnect()
	{
		if (is_server_)
			return;
		disconnectAll();
	}

	int NetworkHandler::getConnectedClients() const
	{
		int ret;
		if (socket_mtx_.try_lock())
		{
			ret = connected_clients_;
			socket_mtx_.unlock();
			return ret;
		} 
		else //already locked.
		{
			return connected_clients_;
		}
	}

	void NetworkHandler::updateSignals()
	{
		socket_mtx_.lock();
		if (num_clients_ < connected_clients_)
		{
			onConnected(connected_clients_ - num_clients_);
			num_clients_ = connected_clients_;
		}
		else if (num_clients_ > connected_clients_)
		{
			onDisconnected(disconnected_clients_);
			disconnected_clients_.clear();
			if (!is_server_)
			{
				socket_mtx_.unlock();
				disconnectAll();
				socket_mtx_.lock();
			}
			num_clients_ = connected_clients_;
		}
		socket_mtx_.unlock();
	}

	void NetworkHandler::sendPacket(sf::Packet& packet, bool tcp, bool unaltered)
	{
		sf::Packet pack;
		uint8_t byte;

		if (!unaltered)
		{
			pack << static_cast<uint8_t>(MessageType::generic);
			pack << static_cast<uint32_t>(network_clock_.getElapsedTime().asMilliseconds() + timestamp_offset_);
			while (!packet.endOfPacket()) // Why this way SFML?
			{
				packet >> byte;
				pack << byte;
			}

		}

		mtx_.lock();
		if (tcp)
		{
			if (tcp_message_queue_.size() > queueSize) //TODO: we've got a problem...
				tcp_message_queue_.pop_front();
			tcp_message_queue_.push_back(pack);
		}
		else {
			if (udp_message_queue_.size() > queueSize)
				udp_message_queue_.pop_front();
			udp_message_queue_.push_back(pack);
		}
		mtx_.unlock();
	}


	std::vector<sf::Packet> NetworkHandler::getUdpPackets(uint32_t objectID)
	{
		std::vector<sf::Packet> packs;
		mtx_.lock();

		uint32_t id = 0;
		uint32_t packetStamp = 0;

		udp_received_packets_queue_.remove_if([&](sf::Packet packet) {
			if (packet.endOfPacket())
				return true;
			packet >> packetStamp >> id;

			if (packet.endOfPacket())
				return true;

			if (id == objectID)
			{
				packet << packetStamp;
				packs.push_back(packet);
				return true;
			}

			return false;
		});
		mtx_.unlock();
		return packs;
	}

	std::vector<sf::Packet> NetworkHandler::getTcpPackets(uint32_t objectID)
	{
		std::vector<sf::Packet> packs;
		mtx_.lock();

		uint32_t id = 0;
		uint32_t packetStamp = 0;

		tcp_received_packets_queue_.remove_if([&](sf::Packet packet) {
			if (packet.endOfPacket())
				return true;
			packet >> packetStamp >> id;

			if (packet.endOfPacket())
				return true;

			if (id == objectID)
			{
				packet << packetStamp;
				packs.push_back(packet);
				return true;
			}

			return false;
		});
		mtx_.unlock();
		return packs;
	}


	void NetworkHandler::awaitConnectionThreadRun()
	{
		socket_mtx_.lock();
		unsigned int clientConnections = connected_clients_;
		socket_mtx_.unlock();
		while (clientConnections < max_client_connections && await_connections_)
		{
			sf::Packet packet;
			auto client = std::make_unique<sf::TcpSocket>();
			if (tcp_listener_.accept(*client) == sf::Socket::Done)
			{
				if (await_connections_) //should we still await connections?
				{
					//bind udp socket
					auto udpSocket = std::make_unique<sf::UdpSocket>();
					sf::IpAddress ip = client->getRemoteAddress();
					sf::Socket::Status status = sf::Socket::Error;

					for (int i = 0; i < 16; i++)
					{
						status = udpSocket->bind(usable_udp_ports_[i]);
						if (status == sf::Socket::Done)
							break;
					}

					if (status == sf::Socket::Done)
					{
						packet.clear();
						packet << static_cast<uint8_t>(MessageType::initial);
						packet << static_cast<uint16_t>(udpSocket->getLocalPort());
						packet << network_clock_.getElapsedTime().asMilliseconds(); //for timestamp syncing
						client->send(packet);			//send packet back to let the client know servers udp port
						
						sf::SocketSelector selector;
						selector.add(*client);
						if (selector.wait(sf::seconds(5)))	//wait for the right packet...
						{
							uint8_t type;
							packet.clear();
							client->receive(packet);
							packet >> type;
							if (type == MessageType::initial)	//right packet? 
							{
								unsigned short udpPort;
								packet >> udpPort;
								selector.clear();

								//send initial keepalive packet
								packet.clear();
								packet << static_cast<uint8_t>(MessageType::keepalive);
								client->send(packet);
								udpSocket->send(packet, ip, udpPort);

								socket_mtx_.lock();
								tba_tcp_sockets_.push_back(std::move(client));
								tba_udp_sockets_.push_back(std::move(udpSocket));
								tba_udp_ports_.push_back(udpPort);
								clientConnections = connected_clients_;		//finally make the sockets usable and update connected clients
								socket_mtx_.unlock();
								if (!run_thread_)
								{
									//start the network handling thread if not already running
									run_thread_ = true;
									network_thread_ = std::thread(&NetworkHandler::netThreadRun, this);
								}
								selector.clear();
								continue;
							}
						}
						udpSocket->unbind();
					}
				}
				packet << static_cast<uint8_t>(MessageType::bye);
				client->send(packet);
				client->disconnect();
			}
		}
	}

	void NetworkHandler::netThreadRun()
	{
		ip = server_ip_;
		socket_mtx_.lock();
		tcp_keepalive_clock.restart();
		udp_keepalive_clock.restart();
		tcp_timeout_clocks.reserve(tcp_sockets_.size());
		udp_timeout_clocks.reserve(tcp_sockets_.size());
		for (auto& socket : tcp_sockets_)
		{
			socket_selector.add(*socket);
			tcp_timeout_clocks.emplace_back();
		}
		for (auto& socket : udp_sockets_)
		{
			socket_selector.add(*socket);
			udp_timeout_clocks.emplace_back();
		}
		socket_mtx_.unlock();

		while (run_thread_)
		{
			if (socket_selector.wait(sf::seconds(tick)))
			{
				receive();
			}
			else
			{
				send();
			}
			process_tba_connections();
		}
		socket_selector.clear();
	}

	void NetworkHandler::bindListeners()
	{
		if (is_server_ && !listener_bound_)
		{
			if (tcp_listener_.listen(tcp_port_) != sf::Socket::Done)
			{
				std::wcout << L"Couldn't bind port for listening!\n";
				return;
			}
			listener_bound_ = true;
		}
	}

	void NetworkHandler::receive()
	{
		sf::IpAddress ip = server_ip_;
		for (unsigned int i = 0; i < tcp_sockets_.size(); i++)
		{
			if (socket_selector.isReady(*tcp_sockets_[i]))
			{
				pack.clear();
				tcp_sockets_[i]->receive(pack);
				tcp_timeout_clocks[i].restart();

				pack >> type;
				switch (type)
				{
				case MessageType::generic:
					mtx_.lock();
					if (tcp_received_packets_queue_.size() > queueSize)
					{
						std::wcout << "TCP RECEIVED PACKAGE QUEUE FULL!!!!!\n"; //TODO: thats a problem...
						tcp_received_packets_queue_.pop_front();
					}
					tcp_received_packets_queue_.push_back(pack);
					mtx_.unlock();
					break;
				case MessageType::keepalive:

					if (is_server_)
					{
						pack.clear();
						pack << static_cast<uint8_t>(MessageType::keepalive);
						tcp_sockets_[i]->send(pack);
					}

					break;
				case MessageType::bye:
					tbdisconnected_clients.push_back(i);
					break;
				default:
					break;
				}
			}
			if (socket_selector.isReady(*udp_sockets_[i])) //same in green for udp...
			{
				pack.clear();
				if (is_server_)
					ip = tcp_sockets_[i]->getRemoteAddress();

				udp_sockets_[i]->receive(pack, ip, udp_ports_[i]);
				udp_timeout_clocks[i].restart();

				pack >> type;
				switch (type)
				{
				case MessageType::generic:
					mtx_.lock();
					if (udp_received_packets_queue_.size() > queueSize)
					{
						std::wcout << "UDP RECEIVED PACKAGE QUEUE FULL!!!!!\n"; //meh.. not so important...
						udp_received_packets_queue_.pop_front();
					}
					udp_received_packets_queue_.push_back(pack);
					mtx_.unlock();
					break;
				case MessageType::keepalive:

					if (is_server_)
					{
						pack.clear();
						pack << static_cast<uint8_t>(MessageType::keepalive);
						udp_sockets_[i]->send(pack, ip, udp_ports_[i]);
					}
					else
					{
						ping_ = network_clock_.getElapsedTime().asMilliseconds() - last_udp_keepalive_sent_;
					}

					break;
				default:
					break;
				}
			}
		}
	}

	void NetworkHandler::send()
	{
		for (unsigned int i = 0; i < tcp_sockets_.size(); i++)
		{
			toSendPacks.clear();
			mtx_.lock();
			if (tcp_message_queue_.size() > 0)
			{
				toSendPacks = tcp_message_queue_;
			}
			mtx_.unlock();

			for (auto& toSendPacket : toSendPacks)
			{
				tcp_sockets_[i]->send(toSendPacket);
			}

			if (!is_server_)
			{
				if (tcp_keepalive_clock.getElapsedTime().asSeconds() >= tcp_keepalive_secs)
				{
					pack.clear();
					pack << static_cast<uint8_t>(MessageType::keepalive);
					tcp_sockets_[i]->send(pack);
					if (i + 1 == tcp_sockets_.size())
						tcp_keepalive_clock.restart();
				}
			}


			toSendPacks.clear();
			mtx_.lock();
			if (udp_message_queue_.size() > 0)
			{
				toSendPacks = udp_message_queue_;
			}
			mtx_.unlock();

			if (is_server_)
				ip = tcp_sockets_[i]->getRemoteAddress();

			for (auto& toSendPacket : toSendPacks)
			{
				udp_sockets_[i]->send(toSendPacket, ip, udp_ports_[i]);
			}

			if (!is_server_)
			{
				if (udp_keepalive_clock.getElapsedTime().asSeconds() >= keepalive_secs)
				{
					pack.clear();
					pack << static_cast<uint8_t>(MessageType::keepalive);
					udp_sockets_[i]->send(pack, ip, udp_ports_[i]);
					if (i + 1 == udp_sockets_.size())
						udp_keepalive_clock.restart();

					last_udp_keepalive_sent_ = network_clock_.getElapsedTime().asMilliseconds();

				}
			}
		}

		mtx_.lock();
		tcp_message_queue_.clear();
		udp_message_queue_.clear();
		mtx_.unlock();
	}

	void NetworkHandler::process_tba_connections()
	{
		bool signalvec_is_empty;
		socket_mtx_.lock();
		signalvec_is_empty = disconnected_clients_.empty();
		socket_mtx_.unlock();
		if (signalvec_is_empty)
		{
			for (unsigned int i = 0; i < tcp_timeout_clocks.size(); i++)
			{
				if (tcp_timeout_clocks[i].getElapsedTime().asSeconds() > tcp_timeout_secs
					|| udp_timeout_clocks[i].getElapsedTime().asSeconds() > timeout_secs)
				{
					if (std::find(tbdisconnected_clients.begin(), tbdisconnected_clients.end(), i)
						== tbdisconnected_clients.end())
						tbdisconnected_clients.push_back(i);
					std::wcout << L"Connection timed out...\n";
				}
			}

			//remove only at last element of loop...
			//only if there is stuff to remove...
			socket_mtx_.lock();
			disconnected_clients_ = tbdisconnected_clients;
			tbdisconnected_clients.clear();
			for (unsigned int offset, j = 0; j < disconnected_clients_.size(); j++)
			{
				offset = disconnected_clients_[j] - j;
				socket_selector.remove(*tcp_sockets_[disconnected_clients_[j]]);
				socket_selector.remove(*udp_sockets_[disconnected_clients_[j]]);

				tcp_sockets_.erase(tcp_sockets_.begin() + offset);
				udp_sockets_.erase(udp_sockets_.begin() + offset);
				udp_ports_.erase(udp_ports_.begin() + offset);

				tcp_timeout_clocks.erase(tcp_timeout_clocks.begin() + offset);
				udp_timeout_clocks.erase(udp_timeout_clocks.begin() + offset);
				connected_clients_--;
			}
			socket_mtx_.unlock();
		}

		//add new clients...
		socket_mtx_.lock();
		if (!tba_tcp_sockets_.empty())
		{
			unsigned int sizebefore = tcp_sockets_.size();
			tcp_sockets_.reserve(tcp_sockets_.size() + tba_tcp_sockets_.size());
			std::move(std::begin(tba_tcp_sockets_), std::end(tba_tcp_sockets_), std::back_inserter(tcp_sockets_));
			tba_tcp_sockets_.clear();

			udp_sockets_.reserve(udp_sockets_.size() + tba_udp_sockets_.size());
			std::move(std::begin(tba_udp_sockets_), std::end(tba_udp_sockets_), std::back_inserter(udp_sockets_));
			tba_udp_sockets_.clear();

			udp_ports_.reserve(udp_ports_.size() + tba_udp_ports_.size());
			std::move(std::begin(tba_udp_ports_), std::end(tba_udp_ports_), std::back_inserter(udp_ports_));
			tba_udp_ports_.clear();

			for (unsigned int i = sizebefore; i < tcp_sockets_.size(); i++)
			{
				socket_selector.add(*tcp_sockets_[i]);
				socket_selector.add(*udp_sockets_[i]);
				tcp_timeout_clocks.emplace_back();
				udp_timeout_clocks.emplace_back();
				connected_clients_++;
			}
		}

		if (connected_clients_ == 0)
		{
			if (!is_server_)
				run_thread_ = false;
			connected_to_server_ = false;
		}

		socket_mtx_.unlock();
	}

	sf::Socket::Status NetworkHandler::receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout)
	{
		sf::SocketSelector selector;
		selector.add(socket);
		if (selector.wait(timeout))
			return socket.receive(packet);
		return sf::Socket::NotReady;
	}
}