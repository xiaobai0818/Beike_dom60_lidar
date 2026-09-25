/**
 * @file ros_header.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

// #define ROS1

#ifdef ROS1
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>

using Publisher = ros::Publisher;
using PointCloud2 = sensor_msgs::PointCloud2;
using PointField = sensor_msgs::PointField;
using ImuMsg = sensor_msgs::Imu;
#elif defined ROS2
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>

template <typename MessageT> using PublisherPtr = std::shared_ptr<rclcpp::Publisher<MessageT>>;
using PointCloud2 = sensor_msgs::msg::PointCloud2;
using PointField = sensor_msgs::msg::PointField;
using ImuMsg = sensor_msgs::msg::Imu;
#endif

template <typename MessageT> using PointCloud2Iterator = sensor_msgs::PointCloud2Iterator<MessageT>;