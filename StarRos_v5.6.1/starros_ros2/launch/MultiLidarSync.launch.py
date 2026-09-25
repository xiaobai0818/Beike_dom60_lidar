#!/usr/bin/python3
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import LifecycleNode
from launch_ros.actions import Node

import os


def generate_launch_description():
    driver_node1 = LifecycleNode( package='starros',
                                  namespace='',
                                  executable='driver_node',
                                  name='dome_driver',
                                  output='screen',
                                  emulate_tty=True,
                                  parameters=[{"config_path":"/launch/config/Dome.yaml"}],
                                  remappings=[('/points_raw', '/ns1/points_raw')]
                                 )

    driver_node2 = LifecycleNode( package='starros',
                                  namespace='',
                                  executable='driver_node',
                                  name='rfans_driver',
                                  output='screen',
                                  emulate_tty=True,
                                  parameters=[{"config_path":"/launch/config/RFans.yaml"}],
                                  remappings=[('/points_raw', '/ns2/points_raw')]
                                 )

    driver_node3 = LifecycleNode( package='starros',
                                  namespace='',
                                  executable='driver_node',
                                  name='ck128_driver',
                                  output='screen',
                                  emulate_tty=True,
                                  parameters=[{"config_path":"/launch/config/CK128.yaml"}],
                                  remappings=[('/points_raw', '/ns3/points_raw')]
                                 )

    driver_node4 = LifecycleNode( package='starros',
                                  namespace='',
                                  executable='driver_node',
                                  name='cfans_driver',
                                  output='screen',
                                  emulate_tty=True,
                                  parameters=[{"config_path":"/launch/config/CFans.yaml"}],
                                  remappings=[('/points_raw', '/ns4/points_raw')]
                                 )

    fusion_node = LifecycleNode(package='starros',
                                namespace='fusion_node',
                                executable='fusion_node',
                                name='fusion_node',
                                output='screen',
                                emulate_tty=True,
                                parameters=[
                                    {'lidar1': True},
                                    {'lidar2': True},
                                    {'lidar3': False},
                                    {'lidar4': False},
                                    {'lidar1_topic': '/ns1/points_raw'},
                                    {'lidar2_topic': '/ns2/points_raw'},
                                    {'lidar3_topic': '/ns3/points_raw'},
                                    {'lidar4_topic': '/ns4/points_raw'}]
                                )

    rviz_dir = os.path.join(get_package_share_directory(
        'starros'), 'rviz', 'multiRviz.rviz')
    rviz_node = Node(
        package='rviz2',
        namespace='',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_dir],
        output='screen')

    return LaunchDescription([
        driver_node1,
        driver_node2,
        # driver_node3,
        # driver_node4,
        fusion_node,
        rviz_node
    ])
