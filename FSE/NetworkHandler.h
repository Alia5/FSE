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
	//NetworkHandler(const NetworkHandler& handler);
	~NetworkHandler();

	void setServer(bool server);

	void setServerIP(std::wstring ip);

	bool connect();
	void disconnect();

	void sendPacket(sf::Packet packet, bool important);

	sf::Packet getLatestUDPPacket();

	sf::Packet getLatestUDPPacket(uint32_t objectID);

	void netThreadRunServer();
	void netThreadRunClient();

	NetworkHandler& operator=(const NetworkHandler& handler);

	std::mutex mtx;

private:
	bool runThread = false;

	bool m_isServer = false;
	bool tcpConnected = false;

	unsigned short tcpPort = 23651;
	unsigned short  udpPort = 23652;

	unsigned short  udpPortClient = 23652;

	sf::IpAddress ip = "192.168.1.159";

	sf::TcpListener tcpListener;
	sf::TcpSocket tcpSocket;

	sf::UdpSocket udpSocket;

	std::thread networkThread;
	
	std::list<sf::Packet> udpMessageQueue;
	std::list<sf::Packet> udpReceivedPacketsQueue;

	sf::Packet latestUDPPacket;

	void swap(const NetworkHandler& other);

	sf::SocketSelector sockSelector;

	sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout);

};
