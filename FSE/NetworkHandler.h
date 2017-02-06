#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include <list>
#include <mutex>
#include "Signals.h"
#include <atomic>

namespace fse
{


	class NetworkHandler {
	public:
		NetworkHandler();
		~NetworkHandler();

		void setServer(bool server);
		void setMaxConnections(size_t maxConnections);

		void setServerIP(std::wstring ip);

		int getPing() const;

		bool awaitConnections();
		void stopAwaitingConnections();
		void disconnectAll();
		bool isConnected() const;
		bool connect();
		void disconnect();

		int getConnectedClients() const;

		void updateSignals();

		void sendPacket(sf::Packet& packet, bool tcp, bool unaltered = false);

		std::vector<sf::Packet> getUdpPackets(uint32_t objectID);
		std::vector<sf::Packet> getTcpPackets(uint32_t objectID);

	private:

		enum MessageType : uint8_t 
		{
			undefined = 0,
			keepalive,
			initial,
			bye,
			generic,
		};

		std::atomic<int> ping_ = 0;

		void awaitConnectionThreadRun();
		void netThreadRun();
		void bindListeners();
		void receive();
		void send();
		void process_tba_connections();

		static sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout);

		mutable std::mutex mtx_;
		mutable std::mutex socket_mtx_;

		bool listener_bound_ = false;

		std::atomic<bool> await_connections_ = false;

		std::atomic<bool> run_thread_ = false;

		bool is_server_ = false;
		bool connected_to_server_ = false;

		unsigned int max_client_connections = 0;

		unsigned short tcp_port_ = 23651;
		unsigned short usable_udp_ports_[16] = { 23652, 23653, 23654, 23655, 23656, 23657, 23658, 23659,
												23660, 23661, 23662, 23663, 23664, 23665, 23666, 23667 };

		sf::IpAddress server_ip_ = "127.0.0.1";

		sf::TcpListener tcp_listener_;
		std::vector<std::unique_ptr<sf::TcpSocket>> tcp_sockets_;
		std::vector<std::unique_ptr<sf::UdpSocket>> udp_sockets_;
		std::vector<unsigned short> udp_ports_;
		sf::SocketSelector socket_selector;
		std::vector<sf::Clock> tcp_timeout_clocks;
		std::vector<sf::Clock> udp_timeout_clocks;

		std::vector<std::unique_ptr<sf::TcpSocket>> tba_tcp_sockets_;
		std::vector<std::unique_ptr<sf::UdpSocket>> tba_udp_sockets_;
		std::vector<unsigned short> tba_udp_ports_;

		//variables for server/client thread
		sf::Clock tcp_keepalive_clock;
		sf::Clock udp_keepalive_clock;
		sf::IpAddress ip = server_ip_;
		sf::Packet pack;
		std::list<sf::Packet> toSendPacks;
		uint8_t type;
		////

		unsigned int connected_clients_ = 0;

		std::thread await_connections_thread_;
		std::thread network_thread_;

		std::list<sf::Packet> udp_message_queue_;
		std::list<sf::Packet> udp_received_packets_queue_;

		std::list<sf::Packet> tcp_message_queue_;
		std::list<sf::Packet> tcp_received_packets_queue_;

		sf::Clock network_clock_;

		int last_udp_keepalive_sent_ = 0;

		sf::Int32 timestamp_offset_ = 0;

		float tick = 1.f / 120.f;
		unsigned int queueSize = 10000;

		int timeout_secs = 5;
		int tcp_timeout_secs = 15;
		int tcp_keepalive_secs = 5;
		int keepalive_secs = 1;

		unsigned int num_clients_ = 0; ///< internal use for signals...
		std::vector<int> tbdisconnected_clients;
		std::vector<int> disconnected_clients_; ///< internal use for signals..

	PUBLIC_SIGNALS:
		using OnConnectedSignal = Signal<int>;
		using OnDisconnectedSignal = Signal<std::vector<int>>;

		//Number of clients connected - as a client this value is always 1
		OnConnectedSignal onConnected;
		//which clients are disconnected - as a client this vector is always size of 1 with value 1
		OnDisconnectedSignal onDisconnected;



	};
}
