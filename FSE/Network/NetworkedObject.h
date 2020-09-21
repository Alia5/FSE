#pragma once
#include <SFML/Network/Packet.hpp>
#include "../NetworkHandler.h"
#include "../FSEObject/FSEObject.h"

namespace fse
{
	/*!
	 * \brief helper class for networked objects
	 * Usage: Inherit networked objects in addition to FSEObject \n 
	 * Implement hostUpdate() and ClientUpdate() \n
	 * Call netupdate on begin of the objects update method
	 */
	class NetworkedObject
	{
	public:
		NetworkedObject();
		virtual ~NetworkedObject() = default;
		/*!
		 * Called on host systems \n 
		 * always called first
		 * \param deltaTime elapsed time in seconds
		 */
		virtual void hostUpdate(float deltaTime) = 0;
		/*! 
		 * Called on host and clientsystems
		 * \param deltaTime elapsed time in seconds
		 */
		virtual void clientUpdate(float deltaTime) = 0;

		/*!
		 * Call on begin of your objects update method
		 * \param deltaTime elapsed time in seconds
		 */
		void netUpdate(float deltaTime);

		/*!
		 * Set wether the object runs networked or not \n 
		 * default: false
		 * \param networked networked
		 */

		void setNetworked(bool networked);
		/*!
		* Set wether the object runs on host or not \n
		* default: true
		* \param host runs on host
		*/

		void setRunsOnHost(bool host);
		/*!
		 * Set a unique network network id for object syncing
		 * \param net_id unique network id for object syncing
		 */
		void setNetworkId(uint32_t net_id);


		/*!
		 * \brief Send UDP packet
		 * Send UDP packet \n
		 * Unimportant packages may get dropped by the engine
		 * 
		 * \param packet packet to send
		 * \param important important package
		 */

		bool sendUdpPacket(sf::Packet& packet, bool important) const;
		/*!
		 * Send TCP packet
		 */
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
