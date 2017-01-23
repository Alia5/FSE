#pragma once
#include <SFML/Network/Packet.hpp>
#include "../NetworkHandler.h"
#include "../FSEObject/FSEObject.h"

namespace fse
{
	class NetworkedObject
	{
	public:
		NetworkedObject(Scene* scene);
		virtual ~NetworkedObject() = default;
		virtual void hostUpdate(float deltaTime) = 0;
		virtual void clientUpdate(float deltaTime) = 0;

		void netUpdate(float deltaTime);

		void setNetworked(bool networked);
		void setRunsOnHost(bool host);

		void setNetworkId(uint32_t net_id);

		bool sendUdpPacket(sf::Packet& packet, bool important) const;
		void sendTcpPacket(sf::Packet& packet) const;

	protected:
		bool networked_ = false;
		bool runs_on_host_ = true;

		float net_time_ = 0;
		float tick_ = 1.f / 60.f;

		uint32_t network_id_ = 0;
		NetworkHandler* network_handler_;

		std::vector<sf::Packet> udp_packets_;
		std::vector<sf::Packet> tcp_packets_;
		std::vector<sf::Packet> all_packets_;



	};
}
