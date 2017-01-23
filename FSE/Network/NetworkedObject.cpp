#include "NetworkedObject.h"
#include "../NetworkHandler.h"
#include  "../Application.h"

namespace fse
{
	NetworkedObject::NetworkedObject(Scene* scene)
	{

		network_handler_ = scene->getApplication()->getNetworkHandler();
	}

	void NetworkedObject::netUpdate(float deltaTime)
	{
		net_time_ += deltaTime;

		//sf::Packet packet = network_handler_->getLatestUDPPacket(network_id_);
		//if (!packet.endOfPacket())
		//{
		//	latest_udp_packet_ = packet;
		//}
		if (net_time_ >= tick_)
		{
			udp_packets_ = network_handler_->getUdpPackets(network_id_);
			tcp_packets_ = network_handler_->getTcpPackets(network_id_);
			all_packets_.clear();
			all_packets_.reserve(udp_packets_.size() + tcp_packets_.size());
			all_packets_.insert(all_packets_.end(), tcp_packets_.begin(), tcp_packets_.end());
			all_packets_.insert(all_packets_.end(), udp_packets_.begin(), udp_packets_.end());
		} 
		else
		{
			all_packets_.clear();
			tcp_packets_.clear();
			udp_packets_.clear();
		}
		if (runs_on_host_)
			hostUpdate(deltaTime);
		if (networked_)
			clientUpdate(deltaTime);
		if (net_time_ >= tick_)
			net_time_ = 0;
	}

	void NetworkedObject::setNetworked(bool networked)
	{
		networked_ = networked;
	}

	void NetworkedObject::setRunsOnHost(bool host)
	{
		runs_on_host_ = host;
	}

	void NetworkedObject::setNetworkId(uint32_t net_id)
	{
		network_id_ = net_id;
	}

	bool NetworkedObject::sendUdpPacket(sf::Packet& packet, bool important) const
	{
		if (important)
		{
			network_handler_->sendPacket(packet, false);
			return true;
		}

		if (net_time_ >= tick_)
		{
			network_handler_->sendPacket(packet, false);
			return true;
		}
		return false;
	}

	void NetworkedObject::sendTcpPacket(sf::Packet& packet) const
	{
		network_handler_->sendPacket(packet, true);
	}
}
