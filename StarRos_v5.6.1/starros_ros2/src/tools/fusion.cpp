#include "rclcpp/rclcpp.hpp"

#include <sensor_msgs/msg/point_cloud2.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2> SSPolicy2;
typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2> SSPolicy3;
typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2, sensor_msgs::msg::PointCloud2> SSPolicy4;

class Fusion : public rclcpp::Node
{
public:
  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr fusion_pointcloud_pub_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud_sub_;
  message_filters::Subscriber<sensor_msgs::msg::PointCloud2> pointcloud_sub1_, pointcloud_sub2_, pointcloud_sub3_, pointcloud_sub4_;

  std::shared_ptr<message_filters::Synchronizer<SSPolicy2>> sync2_;
  std::shared_ptr<message_filters::Synchronizer<SSPolicy3>> sync3_;
  std::shared_ptr<message_filters::Synchronizer<SSPolicy4>> sync4_;

  std::string lidar1_topic_, lidar2_topic_, lidar3_topic_, lidar4_topic_;
  std::vector<std::string> sub_str_;

  void pointcloudCallback(const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud)
  {
    fusion_pointcloud_pub_->publish(*pointcloud);
  }

  void pointcloudCallback2(const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud1, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud2)
  {
    sensor_msgs::msg::PointCloud2 msg_pub;
    msg_pub.header.frame_id = "world";
    msg_pub.height = 1;
    msg_pub.point_step = pointcloud1->point_step;
    msg_pub.width = pointcloud1->width + pointcloud2->width;
    msg_pub.fields = pointcloud1->fields;
    msg_pub.data.resize(msg_pub.width * msg_pub.point_step);
    msg_pub.row_step = msg_pub.data.size();
    memcpy(&msg_pub.data[0], &pointcloud1->data[0], pointcloud1->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size(), &pointcloud2->data[0], pointcloud2->data.size());
    fusion_pointcloud_pub_->publish(msg_pub);
  }

  void pointcloudCallback3(const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud1, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud2, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud3)
  {
    sensor_msgs::msg::PointCloud2 msg_pub;
    msg_pub.header.frame_id = "world";
    msg_pub.height = 1;
    msg_pub.point_step = pointcloud1->point_step;
    msg_pub.width = pointcloud1->width + pointcloud2->width + pointcloud3->width;
    msg_pub.fields = pointcloud1->fields;
    msg_pub.data.resize(msg_pub.width * msg_pub.point_step);
    msg_pub.row_step = msg_pub.data.size();
    memcpy(&msg_pub.data[0], &pointcloud1->data[0], pointcloud1->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size(), &pointcloud2->data[0], pointcloud2->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size() + pointcloud2->data.size(), &pointcloud3->data[0], pointcloud3->data.size());
    fusion_pointcloud_pub_->publish(msg_pub);
  }

  void pointcloudCallback4(const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud1, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud2, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud3, const sensor_msgs::msg::PointCloud2::ConstSharedPtr pointcloud4)
  {
    sensor_msgs::msg::PointCloud2 msg_pub;
    msg_pub.header.frame_id = "world";
    msg_pub.height = 1;
    msg_pub.point_step = pointcloud1->point_step;
    msg_pub.width = pointcloud1->width + pointcloud2->width + pointcloud3->width;
    msg_pub.fields = pointcloud1->fields;
    msg_pub.data.resize(msg_pub.width * msg_pub.point_step);
    msg_pub.row_step = msg_pub.data.size();
    memcpy(&msg_pub.data[0], &pointcloud1->data[0], pointcloud1->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size(), &pointcloud2->data[0], pointcloud2->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size() + pointcloud2->data.size(), &pointcloud3->data[0], pointcloud3->data.size());
    memcpy(&msg_pub.data[0] + pointcloud1->data.size() + pointcloud2->data.size() + pointcloud3->data.size(), &pointcloud4->data[0], pointcloud4->data.size());
    fusion_pointcloud_pub_->publish(msg_pub);
  }

  void setupNodeParams()
  {
    this->declare_parameter<std::string>("lidar1_topic", "");
    this->get_parameter("lidar1_topic", lidar1_topic_);

    this->declare_parameter<std::string>("lidar2_topic", "");
    this->get_parameter("lidar2_topic", lidar2_topic_);

    this->declare_parameter<std::string>("lidar3_topic", "");
    this->get_parameter("lidar3_topic", lidar3_topic_);

    this->declare_parameter<std::string>("lidar4_topic", "");
    this->get_parameter("lidar4_topic", lidar4_topic_);

    bool lidar;
    this->declare_parameter<bool>("lidar1", false);
    this->get_parameter("lidar1", lidar);
    if (lidar)
    {
      sub_str_.push_back(lidar1_topic_);
    }
    this->declare_parameter<bool>("lidar2", false);
    this->get_parameter("lidar2", lidar);
    if (lidar)
    {
      sub_str_.push_back(lidar2_topic_);
    }
    this->declare_parameter<bool>("lidar3", false);
    this->get_parameter("lidar3", lidar);
    if (lidar)
    {
      sub_str_.push_back(lidar3_topic_);
    }
    this->declare_parameter<bool>("lidar4", false);
    this->get_parameter("lidar4", lidar);
    if (lidar)
    {
      sub_str_.push_back(lidar4_topic_);
    }
  }

  void initSync()
  {
    rclcpp::QoS qos(10);
    rmw_qos_profile_t rmw_qos_profile = qos.get_rmw_qos_profile();

    switch (sub_str_.size())
    {
    case 1:
      pointcloud_sub_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(sub_str_[0], 1, std::bind(&Fusion::pointcloudCallback, this, std::placeholders::_1));
      break;
    case 2:
      pointcloud_sub1_.subscribe(this, sub_str_[0], rmw_qos_profile);
      pointcloud_sub2_.subscribe(this, sub_str_[1], rmw_qos_profile);
      std::cout << sub_str_[0] << " " << sub_str_[1] << std::endl;
      sync2_ = std::make_shared<message_filters::Synchronizer<SSPolicy2>>(SSPolicy2(10), pointcloud_sub1_, pointcloud_sub2_);
      sync2_->registerCallback(std::bind(&Fusion::pointcloudCallback2, this, std::placeholders::_1, std::placeholders::_2));
      break;
    case 3:
      pointcloud_sub1_.subscribe(this, sub_str_[0], rmw_qos_profile);
      pointcloud_sub2_.subscribe(this, sub_str_[1], rmw_qos_profile);
      pointcloud_sub3_.subscribe(this, sub_str_[2], rmw_qos_profile);
      sync3_ = std::make_shared<message_filters::Synchronizer<SSPolicy3>>(SSPolicy3(10), pointcloud_sub1_, pointcloud_sub2_, pointcloud_sub3_);
      sync3_->registerCallback(std::bind(&Fusion::pointcloudCallback3, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      break;
    case 4:
      pointcloud_sub1_.subscribe(this, sub_str_[0], rmw_qos_profile);
      pointcloud_sub2_.subscribe(this, sub_str_[1], rmw_qos_profile);
      pointcloud_sub3_.subscribe(this, sub_str_[2], rmw_qos_profile);
      pointcloud_sub4_.subscribe(this, sub_str_[3], rmw_qos_profile);
      sync4_ = std::make_shared<message_filters::Synchronizer<SSPolicy4>>(SSPolicy4(10), pointcloud_sub1_, pointcloud_sub2_, pointcloud_sub3_, pointcloud_sub4_);
      sync4_->registerCallback(std::bind(&Fusion::pointcloudCallback4, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
      break;
    default:
      break;
    }
  }
  Fusion(std::string name) : Node(name)
  {
    rclcpp::QoS qos(10);
    rmw_qos_profile_t rmw_qos_profile = qos.get_rmw_qos_profile();

    fusion_pointcloud_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/fusion_points_raw", qos);

    setupNodeParams();
    initSync();
  }
  ~Fusion(){};
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Fusion>("Fusion"));

  rclcpp::shutdown();
  return 0;
}