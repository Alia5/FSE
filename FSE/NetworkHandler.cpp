#include "NetworkHandler.h"
#include <future>

NetworkHandler::NetworkHandler()
{
	m_isServer = false;
	runThread = false;
}

NetworkHandler::NetworkHandler(bool server)
{
	m_isServer = server;
}

//NetworkHandler::NetworkHandler(const NetworkHandler& handler)
//{
//	//Copy constructor
//}

NetworkHandler::~NetworkHandler()
{
	if (tcpConnected)
		disconnect();
}

void NetworkHandler::setServer(bool server)
{
	m_isServer = server;
}

void NetworkHandler::setServerIP(std::wstring ip)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::string ipstring = converter.to_bytes(ip);

	this->ip = sf::IpAddress(ipstring);
}

bool NetworkHandler::connect()
{
	tcpListener.close();
	tcpSocket.disconnect();
	udpSocket.unbind();


	if (m_isServer)
	{

		std::wcout << L"Binding UPD connection...\n";
		if (udpSocket.bind(udpPort) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't bind port for listening!\n";
		} 


		std::wcout << L"Binding TCP connection...\n";

		if (tcpListener.listen(tcpPort) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't bind port for listening!\n";
		}

		if (tcpListener.accept(tcpSocket) != sf::Socket::Done)
		{
			std::wcout << L"Couldn't accept connection\n";
		} else {
			std::wcout << L"Connected!\n";
			tcpConnected = true;

			ip = tcpSocket.getRemoteAddress();

			runThread = true;
			std::wcout << L"Firing SERVER thread!!!" << std::endl;
			networkThread = std::thread(&NetworkHandler::netThreadRunServer, this);

			return true;

		}


	} else {
		
		sf::Socket::Status status = tcpSocket.connect(ip, tcpPort);
		if (status != sf::Socket::Done)
		{
			std::wcout << L"Couldn't connect\n";
		} else {
			std::wcout << L"Connected!\n";
			tcpConnected = true;

			std::wcout << L"Binding UPD connection...\n";
			if (udpSocket.bind(udpPortClient) != sf::Socket::Done)
			{
				std::wcout << L"Couldn't bind port for listening!\n";
			}

			runThread = true;
			std::wcout << L"Firing CLIENT thread!!!" << std::endl;
			networkThread = std::thread(&NetworkHandler::netThreadRunClient, this);

			return true;

		}

	}

	return false;

}

void NetworkHandler::disconnect()
{
	runThread = false;
	udpSocket.unbind();
	tcpSocket.disconnect();
	tcpListener.close();
	networkThread.join();
	tcpConnected = false;
}

void NetworkHandler::sendPacket(sf::Packet packet, bool important)
{
	if (important)
	{
		
	} else {
		
	}

	mtx.lock();
	if (udpMessageQueue.size() > 1000)
		udpMessageQueue.pop_front();
	udpMessageQueue.push_back(packet);
	mtx.unlock();
}

sf::Packet NetworkHandler::getLatestUDPPacket()
{
	sf::Packet temp;
	mtx.lock();
		temp = latestUDPPacket;
	mtx.unlock();
	return temp;
}

sf::Packet NetworkHandler::getLatestUDPPacket(uint32_t objectID)
{
	sf::Packet pack;

	uint32_t id;
	float32 timestamp = 0;
	float32 packetStamp = 0;

	mtx.lock();
	for (auto packet = udpReceivedPacketsQueue.begin(); packet != udpReceivedPacketsQueue.end(); ++packet)
	{
		if (!(*packet).endOfPacket())
		{
			(*packet) >> id >> packetStamp;
			if (id == objectID && packetStamp > timestamp)
			{
				pack = (*packet);
				timestamp = packetStamp;
				udpReceivedPacketsQueue.erase(packet--);
			}
		}
	}
	mtx.unlock();

	return pack;
}

void NetworkHandler::netThreadRunServer()
{
	//sf::Packet packet;
	//while (runThread)
	//{
	//	udpSocket.receive(packet, ip, udpPortClient);
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
	//	udpSocket.send(packet, ip, udpPort);

	//	sf::sleep(sf::milliseconds(10));
	//}


	sockSelector.add(udpSocket);
	uint8_t type;

	sf::Packet packet;

	std::wcout << L"Waiting to receive initial KeepAlive packet!\n";

	udpSocket.receive(packet, ip, udpPortClient);
	packet >> type;

	std::wcout << "OK!\n";

	//sf::Clock timer;

	srand(time(nullptr));

	while (runThread)
	{

		if (sockSelector.wait(sf::seconds(1.f/60.f)))
		{
			if (sockSelector.isReady(udpSocket))
			{
				sf::Packet pack;
				udpSocket.receive(pack, ip, udpPortClient);


				//uint8_t typ;
				//pack >> typ;

				//if (typ == 0x01)
				//{
				//	std::wcout << "Received KeepAlive Packet!\n";
				//} 

				mtx.lock();
				latestUDPPacket = pack;

				if (udpReceivedPacketsQueue.size() > 1000)
					udpReceivedPacketsQueue.pop_front();
				udpReceivedPacketsQueue.push_back(pack);

				mtx.unlock();
			

			}
			//timer.restart();
		}
		else
		{
			mtx.lock();
			std::list<sf::Packet> toSendPacks;
			if (udpMessageQueue.size() > 0)
			{
				toSendPacks = udpMessageQueue;
				udpMessageQueue.clear();
			}
			mtx.unlock();

			//sf::sleep(sf::milliseconds(50));
			//sf::sleep(sf::milliseconds(rand() % 51));
			for (auto& toSendPacket : toSendPacks)
			{
				udpSocket.send(toSendPacket, ip, udpPortClient);
				//std::wcout << "Sending position packet!!!\n";
			}
		}
	}

	sockSelector.remove(udpSocket);

}

void NetworkHandler::netThreadRunClient()
{

	sockSelector.add(udpSocket);
	uint8_t type;

	sf::Packet packet;
	type = 0x01;

	std::wcout << L"Sending Initial KeepAlive packet!\n";

	packet << type;
	udpSocket.send(packet, ip, udpPort);

	std::wcout << "OK!\n";

	sf::Clock timer;

	while (runThread)
	{
		
		if (sockSelector.wait(sf::seconds(1.f/60.f)))
		{
			if (sockSelector.isReady(udpSocket))
			{
				sf::Packet pack;
				udpSocket.receive(pack, ip, udpPort);

				mtx.lock();
				latestUDPPacket = pack;

				if (udpReceivedPacketsQueue.size() > 1000)
					udpReceivedPacketsQueue.pop_front();
				udpReceivedPacketsQueue.push_back(pack);

				mtx.unlock();
			}

			timer.restart();
		} else {

			mtx.lock();
			std::list<sf::Packet> toSendPacks;
			if (udpMessageQueue.size() > 0)
			{
				toSendPacks = udpMessageQueue;
				udpMessageQueue.clear();
			}
			mtx.unlock();

			//sf::sleep(sf::milliseconds(50));
			//sf::sleep(sf::milliseconds(rand() % 51));
			for (auto& toSendPacket : toSendPacks)
			{
				udpSocket.send(toSendPacket, ip, udpPort);
				//std::wcout << "Sending position packet!!!\n";
			}

			if (timer.getElapsedTime().asSeconds() > 1)
			{
				std::wcout << L"Sending KeepAlive packet!\n";
				sf::Packet packet;
				type = 0x01;
				packet << type;
				udpSocket.send(packet, ip, udpPort);

				timer.restart();
				//TODO: Some timeout stuff
			}
		}
	}

	sockSelector.remove(udpSocket);
}

void NetworkHandler::swap(const NetworkHandler& other)
{
	m_isServer = other.m_isServer;
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
