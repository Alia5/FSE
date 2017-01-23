#pragma once

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <utility>
#include <list>
#include <mutex>
#include <codecvt>
#include <Box2D/Common/b2Settings.h>

class NetworkHandler {
public:
	NetworkHandler();
	NetworkHandler(bool server);
	~NetworkHandler();

	void setServer(bool server);

	void setServerIP(std::wstring ip);

	bool connect();
	void disconnect();

	void sendPacket(sf::Packet& packet, bool tcp);

	std::vector<sf::Packet> getUdpPackets(uint32_t objectID);
	std::vector<sf::Packet> getTcpPackets(uint32_t objectID);

	void netThreadRunServer();
	void netThreadRunClient();

	NetworkHandler& operator=(const NetworkHandler& handler);

	std::mutex mtx_;

private:
	void swap(const NetworkHandler& other);
	sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout);

	bool run_thread_ = false;

	bool is_server_ = false;
	bool tcp_connected_ = false;

	unsigned short tcp_port_ = 23651;
	unsigned short  udp_port_ = 23652;
	unsigned short  udp_port_client_ = 23652;

	sf::IpAddress ip_ = "";

	sf::TcpListener tcp_listener_;
	sf::TcpSocket tcp_socket_;

	sf::UdpSocket udp_socket_;

	std::thread network_thread_;
	
	std::list<sf::Packet> udp_message_queue_;
	std::list<sf::Packet> udp_received_packets_queue_;

	std::list<sf::Packet> tcp_message_queue_;
	std::list<sf::Packet> tcp_received_packets_queue_;

	sf::SocketSelector sock_selector_;

	sf::Clock network_clock_;

	float tick = 1.f / 120.f;
	int queueSize = 100;

};
