## StarRos

### 关于这个项目

StarRos支持如下设备:
- RFans
- CFans
- CK
- Dome

### 1. 坏境和依赖

#### 1.1 环境

程序运行环境为:Linux+ROS

推荐版本:
- Ubuntu16.04 + ROS Kinetic
- Ubuntu18.04 + ROS Melodic
- Ubuntu20.04 + ROS Noetic

- Ubuntu20.04 + ROS2 foxy或Galsctic
- Ubuntu22.04 + ROS2 Humble

#### 1.2 依赖

本环境需要安装**yaml**库。

``` bash
sudo apt install libyaml-cpp-dev
```

（可选）本环境需要安装**pcap**库。

该库在PCAP文件回放时需要使用。

``` bash
sudo apt install libpcap-dev
```

### 2. 参数配置

通过编辑launch文件夹中的设备yaml配置文件来配置参数。

``` bash
cd starros_ws/src
gedit launch/config/CK128.yaml
```

#### 2.1 IP、端口、转速配置

需要根据真实使用设备IP、端口来配置对应选项，rps配置项则填写期望转速值。

``` yaml
# 设备IP、数据包端口、控制端口、心跳包端口、转速
device_ip: "192.168.0.10"
data_port: 2014
ctrl_port: 2013
heart_port: 2030
rps: 10
```

#### 2.2 时间同步

时间同步需要在设备端启用后，在对应设备的yaml文件中将时间同步开关打开，这样点云消息与IMU消息时间即为时间同步后的时间。

时间同步开关如下：

``` yaml
time_sync: false
```

#### 2.3 设备数据协议

需要和设备配置的协议相匹配：

``` yaml
# 设备数据协议
data_format: 0719
```

#### 2.4 外参输入

通过修改配置文件中的外参参数，可以旋转平移点云，参数如下：

``` yaml
# 是否使用外参
extrinsin_enable: false

# 外参旋转 单位：°
extrinsic_yaw: 0
extrinsic_pitch: 0
extrinsic_roll: 0

# 外参平移 单位：m
extrinsic_tx: 0
extrinsic_ty: 0
extrinsic_tz: 0
```


#### 2.5 ISF文件录制配置

ISF文件录制需要在config中对应设备的yaml配置文件中配置：

``` yaml
# 是否保存ISF原始数据
save_isf: false

# ISF原始数据保存路径
save_isf_path: ""
```

#### 2.6 算法相关配置

根据需要选择是否开启算法，如果开启算法可以选择指定算法：

``` yaml
# 是否使用算法
use_algorithm: true
```

防护窗是否选择：

``` yaml
# 解算模式
mode_type: 1 #无防护窗
mode_type: 2 #有防护窗
```

#### 2.7 多雷达启动

本工程提供了两个版本的多雷达启动，第一种是直接启动多个雷达节点，第二种是启动多个雷达节点后同时启动一个点云融合节点，接受所有点云后发布融合后的点云

外参设置见2.4节描述。

第一种启动方式（多雷达直接启动）：

1. 通过launch/config文件夹下面的配置文件模板，生成对应多个设备的配置文件；

2. 修改**MultiLidar.launch**或**MultiLidar.launch.py**文件中的**config_path**参数，将其修改为实际设备的配置文件相对位置；

3. 如果是ros2，需要将**LaunchDescription**中对应节点放开；

4. 启动即可，如果想添加更多雷达，仿照其样式添加即可。

ROS1
``` xml
<group ns="ns1">
<node pkg="starros" type="driver_node" name="dome_driver" output="screen">
    <param name="config_path" value="/launch/config/Dome.yaml"/>
</node>
</group>
```

ROS2
``` python
driver_node1 = LifecycleNode( package='starros',
                                namespace='',
                                executable='driver_node',
                                name='dome_driver',
                                output='screen',
                                emulate_tty=True,
                                parameters=[{"config_path":"/launch/config/Dome.yaml"}],
                                remappings=[('/points_raw', '/ns1/points_raw')]
                                )
return LaunchDescription([
    driver_node1,
    driver_node2,
    # driver_node3,
    # driver_node4,
    rviz_node
])
```

第二种方式（时间软同步方式）：

1. 通过launch/config文件夹下面的配置文件模板，生成对应多个设备的配置文件；

2. 修改**MultiLidarSync.launch**或**MultiLidarSync.launch.py**文件中的**config_path**参数，将其修改为实际设备的配置文件相对位置；

3. 修改**fusion_node**中的**nsx_flag**或**lidarx**为**true**；

4. 如果是ros2，需要将**LaunchDescription**中对应节点放开；

5. 启动即可，如果想添加更多雷达，仿照其样式添加即可。

ROS1
``` xml
<group ns="ns1">
<node pkg="starros" type="driver_node" name="dome_driver" output="screen">
    <param name="config_path" value="/launch/config/Dome.yaml"/>
</node>
</group>

<node pkg="starros" type="fusion_node" name="fusion" output="screen">
<param name="ns1_flag" value="true"/>
<param name="ns2_flag" value="true"/>
<param name="ns3_flag" value="false"/>
<param name="ns4_flag" value="false"/>
</node>
```

ROS2
``` python
driver_node1 = LifecycleNode( package='starros',
                                namespace='',
                                executable='driver_node',
                                name='dome_driver',
                                output='screen',
                                emulate_tty=True,
                                parameters=[{"config_path":"/launch/config/Dome.yaml"}],
                                remappings=[('/points_raw', '/ns1/points_raw')]
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
```

### 3. PCAP文件回放

该功能为可选功能，如果需要使用需要做如下步骤：

- 安装**1.2 依赖**中的**libpcap-dev**库
- 在**CMakeLists.txt**中将**PCAP**宏定义放开，并为程序添加链接库

``` CMakeLists
# CMakeLists.txt

add_definitions(-DPCAP)

target_link_libraries(driver_node pcap)

```

### 4. ubuntu中编译运行代码

``` bash
# ROS1
mkdir -p starros_ws/src
cd starros_ws/src
git clone http://172.16.102.31:8081/soft/starros.git
cd ..
catkin_make
source devel/setup.bash
roslaunch starros CK128.launch

# ROS2
mkdir -p starros_ws/src
cd starros_ws/src
git clone http://172.16.102.31:8081/soft/starros.git
cd ..
colcon build
source ./install/setup.bash
ros2 launch starros CK128.launch.py
```

### 5. 常见问题

点云自定义字段如下：

``` yaml
x : FLOAT32 # 单位：m
y : FLOAT32 # 单位：m
z : FLOAT32 # 单位：m
intensity : FLOAT32
ring : UINT16
flag : UINT16
timestamp : FLOAT64 # 单位：ns
```