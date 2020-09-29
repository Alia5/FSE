#pragma once
#include <SFML/Network/Packet.hpp>
#include "NetworkHandler.h"
#include "../FSEV8Lib.h"

namespace fse
{
    /*!
     * \brief helper class for networked objects
     * Usage: Inherit networked objects in addition to FSEObject \n 
     * Implement hostUpdate() and ClientUpdate() \n
     * Call netupdate on begin of the objects update method
     */
    class PacketHandler
    {
    public:
        PacketHandler();
        virtual ~PacketHandler() = default;

        /*!
         * Call on begin of your objects update method
         * \param deltaTime elapsed time in seconds
         */
        virtual void netUpdate(float deltaTime);


        /*!
         * Set a unique network network id for object syncing
         * \param net_id unique network id for object syncing
         */
        void setNetworkId(const std::string& net_id);


        const std::string_view getNetworkId() const;


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

        float net_time_ = 0;
        float tick_ = 1.f / 60.f;

        std::string network_id_;
        NetworkHandler* network_handler_;

        std::vector<sf::Packet> udp_packets_;
        std::vector<sf::Packet> tcp_packets_;
        std::vector<sf::Packet> all_packets_;

        FSE_V8_REGISTRATION_FRIEND;
        FSE_V8_ENABLE(PacketHandler);

    };
}
