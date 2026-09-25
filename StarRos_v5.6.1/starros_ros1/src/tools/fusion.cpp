#include <time.h>
#include <ros/ros.h>
#include <iostream>

#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_broadcaster.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>


using namespace std;

using namespace sensor_msgs;
using namespace message_filters;

ros::Publisher fusion_pointCloud_pub;
ros::Subscriber single_point_sub;
std::vector<int> num_Pub;

void callback_pub(const sensor_msgs::PointCloud2ConstPtr &point)
{
  fusion_pointCloud_pub.publish(*point);
}

void callback_pub(const sensor_msgs::PointCloud2ConstPtr &point1, const sensor_msgs::PointCloud2ConstPtr &point2)
{
  sensor_msgs::PointCloud2 msg_pub;

  msg_pub.header.frame_id = "world";
  msg_pub.height = 1;
  msg_pub.point_step = point1->point_step;
  msg_pub.fields = point1->fields;
  msg_pub.width = point1->width + point2->width;
  msg_pub.row_step = msg_pub.width * msg_pub.point_step;
  msg_pub.data.resize(msg_pub.row_step);
  memcpy(&msg_pub.data[0], &point1->data[0], point1->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size(), &point2->data[0], point2->data.size());
  fusion_pointCloud_pub.publish(msg_pub);
}

void callback_pub(const sensor_msgs::PointCloud2ConstPtr &point1, const sensor_msgs::PointCloud2ConstPtr &point2,
                  const sensor_msgs::PointCloud2ConstPtr &point3)
{
  sensor_msgs::PointCloud2 msg_pub;

  msg_pub.header.frame_id = "world";
  msg_pub.height = 1;
  msg_pub.point_step = point1->point_step;
  msg_pub.fields = point1->fields;
  msg_pub.width = point1->width + point2->width + point3->width;
  msg_pub.row_step = msg_pub.width * msg_pub.point_step;
  msg_pub.data.resize(msg_pub.row_step);
  memcpy(&msg_pub.data[0], &point1->data[0], point1->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size(), &point2->data[0], point2->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size() + point2->data.size(), &point3->data[0], point3->data.size());
  fusion_pointCloud_pub.publish(msg_pub);
}

void callback_pub(const sensor_msgs::PointCloud2ConstPtr &point1, const sensor_msgs::PointCloud2ConstPtr &point2,
                  const sensor_msgs::PointCloud2ConstPtr &point3, const sensor_msgs::PointCloud2ConstPtr &point4)
{
  sensor_msgs::PointCloud2 msg_pub;

  msg_pub.header.frame_id = "world";
  msg_pub.height = 1;
  msg_pub.point_step = point1->point_step;
  msg_pub.fields = point1->fields;
  msg_pub.width = point1->width + point2->width + point3->width + point4->width;
  msg_pub.row_step = msg_pub.width * msg_pub.point_step;
  msg_pub.data.resize(msg_pub.row_step);
  memcpy(&msg_pub.data[0], &point1->data[0], point1->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size(), &point2->data[0], point2->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size() + point2->data.size(), &point3->data[0], point3->data.size());
  memcpy(&msg_pub.data[0] + point1->data.size() + point2->data.size() + point3->data.size(), &point4->data[0], point4->data.size());
  fusion_pointCloud_pub.publish(msg_pub);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fusion_node");
  ros::NodeHandle nh("~");

  std::vector<std::string> fuse_sub_string;
  fusion_pointCloud_pub = nh.advertise<sensor_msgs::PointCloud2>("/fusion_point", 1);
  fuse_sub_string.push_back("/ns1/points_raw");
  fuse_sub_string.push_back("/ns2/points_raw");
  fuse_sub_string.push_back("/ns3/points_raw");
  fuse_sub_string.push_back("/ns4/points_raw");

  bool start_flag[4] = {0};
  nh.param<bool>("ns1_flag", start_flag[0], false);
  nh.param<bool>("ns2_flag", start_flag[1], false);
  nh.param<bool>("ns3_flag", start_flag[2], false);
  nh.param<bool>("ns4_flag", start_flag[3], false);

  for (int i = 0; i < 4; i++)
  {
    if (start_flag[i])
    {
      num_Pub.push_back(i);
    }
  }

  switch (num_Pub.size())
  {
  case 1:
  {
    single_point_sub = nh.subscribe(fuse_sub_string.at(num_Pub.at(0)), 1, &callback_pub);
    ros::spin();
    break;
  }
  case 2:
  {
    typedef sync_policies::ApproximateTime<PointCloud2, PointCloud2> MySyncPolicy;
    message_filters::Subscriber<PointCloud2> point1_sub(nh, fuse_sub_string.at(num_Pub.at(0)), 1);
    message_filters::Subscriber<PointCloud2> point2_sub(nh, fuse_sub_string.at(num_Pub.at(1)), 1);
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), point1_sub, point2_sub);

    sync.registerCallback(boost::bind(&callback_pub, _1, _2));
    ros::spin();
    break;
  }
  case 3:
  {
    typedef sync_policies::ApproximateTime<PointCloud2, PointCloud2, PointCloud2> MySyncPolicy;
    message_filters::Subscriber<sensor_msgs::PointCloud2> point1_sub(nh, fuse_sub_string.at(num_Pub.at(0)), 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> point2_sub(nh, fuse_sub_string.at(num_Pub.at(1)), 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> point3_sub(nh, fuse_sub_string.at(num_Pub.at(2)), 1);
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), point1_sub, point2_sub, point3_sub);

    sync.registerCallback(boost::bind(&callback_pub, _1, _2, _3));
    ros::spin();
    break;
  }
  case 4:
  {
    typedef sync_policies::ApproximateTime<PointCloud2, PointCloud2, PointCloud2, PointCloud2> MySyncPolicy;
    message_filters::Subscriber<sensor_msgs::PointCloud2> point1_sub(nh, fuse_sub_string.at(num_Pub.at(0)), 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> point2_sub(nh, fuse_sub_string.at(num_Pub.at(1)), 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> point3_sub(nh, fuse_sub_string.at(num_Pub.at(2)), 1);
    message_filters::Subscriber<sensor_msgs::PointCloud2> point4_sub(nh, fuse_sub_string.at(num_Pub.at(3)), 1);
    Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), point1_sub, point2_sub, point3_sub, point4_sub);

    sync.registerCallback(boost::bind(&callback_pub, _1, _2, _3, _4));
    ros::spin();
    break;
  }
  default:
    ROS_INFO("message of topic is error");
  }

  return 0;
}
