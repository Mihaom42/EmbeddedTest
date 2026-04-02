#include "rclcpp/rclcpp.hpp"
#include "gps_package/msg/gps_msg.hpp"
#include "PacketHandler.h"

#include <thread>
#include <iostream>

class GpsNode : public rclcpp::Node
{
public:
    GpsNode() : Node("gps_node")
    {
        publisher_ = this->create_publisher<gps_package::msg::GpsMsg>("gps_position", 10);

        input_thread_ = std::thread(&GpsNode::inputLoop, this);
    }

    ~GpsNode()
    {
        running_ = false;

        if (input_thread_.joinable())
            input_thread_.join();
    }

private:
    void inputLoop()
    {
        while (running_)
        {
            std::string data;

            if (!std::getline(std::cin, data))
                break;

            PointInfo point = handler_.parsePacket(data);

            if (handler_.getFeedbackValue() != OK)
                continue;

            gps_package::msg::GpsMsg msg;

            msg.latitude = point.lat;
            msg.longitude = point.lon;
            msg.satellites = static_cast<uint8_t>(point.sats);

            publisher_->publish(msg);

            RCLCPP_INFO(this->get_logger(), "Published GPS");
        }
    }

    bool running_ = true;
    std::thread input_thread_;

    PacketHandler handler_;
    rclcpp::Publisher<gps_package::msg::GpsMsg>::SharedPtr publisher_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GpsNode>());
    rclcpp::shutdown();
    return 0;
}