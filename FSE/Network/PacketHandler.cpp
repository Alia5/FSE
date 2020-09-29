#include "PacketHandler.h"
#include "NetworkHandler.h"
#include  "../Application.h"

namespace fse
{
    PacketHandler::PacketHandler() : network_handler_(Application::get()->getNetworkHandler())
    {
    }

    void PacketHandler::netUpdate(float deltaTime)
    {
        net_time_ += deltaTime;
        if (net_time_ >= tick_)
        {
            udp_packets_ = network_handler_->getUdpPackets(network_id_);
            tcp_packets_ = network_handler_->getTcpPackets(network_id_);
            all_packets_ = tcp_packets_;
            all_packets_.reserve(udp_packets_.size() + tcp_packets_.size());
            all_packets_.insert(all_packets_.end(), udp_packets_.begin(), udp_packets_.end());
        } 
        else
        {
            all_packets_.clear();
            tcp_packets_.clear();
            udp_packets_.clear();
        }
        if (net_time_ >= tick_)
            net_time_ = 0;
    }

    void PacketHandler::setNetworkId(const std::string& net_id)
    {
        network_id_ = net_id;
    }

    const std::string_view PacketHandler::getNetworkId() const
    {
        return network_id_;
    }


    bool PacketHandler::sendUdpPacket(sf::Packet& packet, bool important) const
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

    void PacketHandler::sendTcpPacket(sf::Packet& packet) const
    {
        network_handler_->sendPacket(packet, true);
    }

    FSE_V8_REGISTER_BASE_FIRST(PacketHandler)
    {
        v8::HandleScope handle_scope(isolate);
        v8pp::class_<PacketHandler, v8pp::shared_ptr_traits> PacketHandlerClass(isolate);
        PacketHandlerClass
            .auto_wrap_objects()
            .ctor<>()
            .function("netUpdate", &PacketHandler::netUpdate)
            .function("setNetworkId", &PacketHandler::setNetworkId)
            .function("sendUdpPacket", &PacketHandler::sendUdpPacket)
            .function("sendTcpPacket", &PacketHandler::sendTcpPacket)
            .var("net_time", &PacketHandler::net_time_)
            .var("network_id", &PacketHandler::network_id_)
            .var("udp_packets", &PacketHandler::udp_packets_)
            .var("tcp_packets", &PacketHandler::tcp_packets_)
            .var("all_packets", &PacketHandler::all_packets_)
        ;

        module.class_("PacketHandler", PacketHandlerClass);

    }

}
