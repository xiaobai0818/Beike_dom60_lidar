#!/usr/bin/python3
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import LifecycleNode
from launch_ros.actions import Node

import os

def generate_launch_description():
    rviz_dir = os.path.join(get_package_share_directory(
        'starros'), 'rviz', 'singleRviz.rviz')

    driver_node = LifecycleNode(package='starros',
                                namespace='',
                                executable='driver_node',
                                name='driver_node',
                                output='screen',
                                emulate_tty=True,
                                parameters=[
                                    {"config_path":"/launch/config/CK128.yaml"}
                                ]
                                )
    rviz_node = Node(
        package='rviz2',
        namespace='',
        executable='rviz2',
        name='rviz2',
        arguments=['-d', rviz_dir],
        output='screen')

    return LaunchDescription([
        driver_node,
        rviz_node
    ])
