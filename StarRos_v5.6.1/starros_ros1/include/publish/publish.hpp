/**
 * @file publish.hpp
 * @author yuansheng
 * @brief 
 * @version 0.1
 * @date 2024-01-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "ros_header.h"

#include <mutex>
#include <condition_variable>

#include <calc.hpp>

class Publish
{
public:

  /**
   * @brief 单例模式实例化对象获取函数
   * 
   * @return Publish* 实例化对象指针
   */
  static Publish* getInstance()
  {
    static Publish* ptr = nullptr;
    if (nullptr == ptr)
    {
      ptr = new Publish();
    }
    return ptr;
  };

public:
#ifdef ROS1
  /// @brief ros1节点，用于发布点云、IMU话题数据
  std::shared_ptr<ros::NodeHandle> m_nh;
#elif defined ROS2
  /// @brief ros2节点，用于发布点云、IMU话题数据
  std::shared_ptr<rclcpp::Node> m_nh;
#endif
private:

#ifdef ROS1
  /// @brief ros1点云消息发布器
  Publisher m_pointcloud_pub;

  /// @brief ros1IMU消息发布器
  Publisher m_imu_pub;
#elif defined ROS2
  /// @brief ros1点云消息发布器
  PublisherPtr<PointCloud2> m_pointcloud_pub;

  /// @brief ros1IMU消息发布器
  PublisherPtr<ImuMsg> m_imu_pub;
#endif


  /// @brief 点云消息frame_id
  std::string m_frame_id;

  /// @brief 点云话题
  std::string m_pointcloud_topic;

  /// @brief IMU话题
  std::string m_imu_topic;

  /// @brief 实时、离线模式标志位
  bool m_is_real_time_flag;

  /// @brief 时间同步标志位
  bool m_time_sync;

  /// @brief 扫描帧累积标志
  bool m_is_sum_pointcloud;

  /// @brief 点云频率
  int32_t m_scan_speed;

  /// @brief 初始化标志
  bool m_init_flag;

  /// @brief 消息发布线程标志
  bool m_start_flag;

  /// @brief 消息发布线程
  std::shared_ptr<std::thread> m_publish_thread;

  /// @brief 点云消息（SDK格式）
  L1DataFrame_S m_pointcloud_msg;

  /// @brief 消息发布锁
  std::mutex m_msg_mutex;

  /// @brief 消息条件变量
  std::condition_variable m_msg_cond;

  bool extrinsic_enable;

  double extrinsic_R[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

  double extrinsic_t[3] = {0};

  uint8_t m_set_frame_num;

public:
  
  /**
   * @brief Construct a new Publish object
   * 
   */
  Publish();

  /**
   * @brief Destroy the Publish object
   * 
   */
  ~Publish();

  /**
   * @brief 初始化参数
   * 
   * @param input_para 配置参数
   * @return int32_t 
   */
  int32_t setupParam(InputPara& input_para);

  /**
   * @brief 消息发布初始化函数
   * 
   * @return int32_t 
   */
  int32_t init();

  /**
   * @brief 消息发布线程启动函数
   * 
   * @return int32_t 
   */
  int32_t start();

  /**
   * @brief 消息发布线程停止函数
   * 
   * @return int32_t 
   */
  int32_t stop();

  /**
   * @brief 添加点云到发布线程
   * 
   * @param data_frame 
   * @return int32_t 
   */
  int32_t addPointCloudMsg(L1DataFrame_S& data_frame);

  /**
   * @brief 点云消息发布函数
   * 
   * @param pointcloud_msg sensor_msgs格式点云
   * @return int32_t 
   */
  int32_t sendPointCloudMsg(PointCloud2& pointcloud_msg);

  /**
   * @brief 添加IMU消息
   * 
   * @param raw_imu_msg 
   * @return int32_t 
   */
  int32_t addIMUMsg(std::vector<L1IMU_S>& raw_imu_msg);

  /**
   * @brief 发布IMU消息
   * 
   * @param imu_msg 
   * @return int32_t 
   */
  int32_t sendIMUMsg(ImuMsg& imu_msg);

  /**
   * @brief 消息发布线程
   * 
   * @return int32_t 
   */
  int32_t sendMsg();

  /**
   * @brief 获取实时模式标志
   * 
   * @return true 实时模式
   * @return false 离线模式
   */
  inline bool getRealTimeFlag()
  {
    return m_is_real_time_flag;
  };

  /**
   * @brief 获取设备转速
   * 
   * @return int32_t 转速
   */
  inline int32_t getScanSpeed()
  {
    return m_scan_speed;
  }

  /**
   * @brief 设置设备转速
   * 
   * @return int32_t 转速
   */
  inline void setScanSpeed(int32_t speed)
  {
    m_scan_speed = speed;
    if (m_is_sum_pointcloud)
    {
      m_set_frame_num = (uint8_t)(m_scan_speed * 0.1);
    }
  }
};

Publish::Publish()
: m_frame_id("world")
, m_pointcloud_topic("points_raw")
, m_imu_topic("imu_raw")
, m_is_real_time_flag(false)
, m_time_sync(false)
, m_is_sum_pointcloud(false)
, m_scan_speed(0)
, m_init_flag(false)
, m_start_flag(false)
, extrinsic_enable(false)
, m_set_frame_num(0)
{
#ifdef ROS1
  m_nh = std::unique_ptr<ros::NodeHandle>(new ros::NodeHandle());
#elif defined ROS2
  m_nh = std::make_shared<rclcpp::Node>("driver");
  m_nh->declare_parameter<std::string>("config_path", "");
#endif
}

Publish::~Publish()
{
}


int32_t Publish::setupParam(InputPara& input_para)
{
  m_is_real_time_flag = input_para.isf_path.empty() && input_para.pcap_path.empty();

  m_time_sync = input_para.time_sync;

  if("Dome" == input_para.device_name)
  {
    m_scan_speed = input_para.zenith_speed;
    m_is_sum_pointcloud = true;
  }
  else if("CK-128" == input_para.device_name)
  {
    m_scan_speed = input_para.scan_speed;
  }
  else
  {
    m_scan_speed = input_para.scan_speed;
  }

  m_frame_id = input_para.frame_id;
  m_pointcloud_topic = input_para.pointcloud_topic;
  m_imu_topic = input_para.imu_topic;

  if(input_para.extrinsin_enable)
  {
    extrinsic_enable = input_para.extrinsin_enable;

    extrinsic_t[0] = input_para.extrinsic_tx;
    extrinsic_t[1] = input_para.extrinsic_ty;
    extrinsic_t[2] = input_para.extrinsic_tz;

    double cos_roll = cos(static_cast<double>(input_para.extrinsic_roll * PI / 180.0));
    double cos_pitch = cos(static_cast<double>(input_para.extrinsic_pitch * PI / 180.0));
    double cos_yaw = cos(static_cast<double>(input_para.extrinsic_yaw * PI / 180.0));
    double sin_roll = sin(static_cast<double>(input_para.extrinsic_roll * PI / 180.0));
    double sin_pitch = sin(static_cast<double>(input_para.extrinsic_pitch * PI / 180.0));
    double sin_yaw = sin(static_cast<double>(input_para.extrinsic_yaw * PI / 180.0));

    extrinsic_R[0][0] = cos_pitch * cos_yaw;
    extrinsic_R[0][1] = sin_roll * sin_pitch * cos_yaw - cos_roll * sin_yaw;
    extrinsic_R[0][2] = cos_roll * sin_pitch * cos_yaw + sin_roll * sin_yaw;

    extrinsic_R[1][0] = cos_pitch * sin_yaw;
    extrinsic_R[1][1] = sin_roll * sin_pitch * sin_yaw + cos_roll * cos_yaw;
    extrinsic_R[1][2] = cos_roll * sin_pitch * sin_yaw - sin_roll * cos_yaw;

    extrinsic_R[2][0] = -sin_pitch;
    extrinsic_R[2][1] = sin_roll * cos_pitch;
    extrinsic_R[2][2] = cos_roll * cos_pitch;
  }

  return 0;
}

int32_t Publish::init()
{
  if(m_init_flag)
  {
    return 0;
  }
  m_init_flag = true;

#ifdef ROS1
  m_pointcloud_pub = m_nh->advertise<PointCloud2>(m_pointcloud_topic, 10);
  m_imu_pub = m_nh->advertise<ImuMsg>(m_imu_topic, 10);
#elif defined ROS2
  m_pointcloud_pub = m_nh->create_publisher<PointCloud2>(m_pointcloud_topic, 10);
  m_imu_pub = m_nh->create_publisher<ImuMsg>(m_imu_topic, 10);
#endif

  return 0;
}

int32_t Publish::start()
{
  if(m_start_flag)
  {
    return 0;
  }
  m_start_flag = 0;

  m_publish_thread = std::shared_ptr<std::thread>(new std::thread(std::bind(&Publish::sendMsg, this)));

  return 0;
}

int32_t Publish::stop()
{
  if(m_start_flag)
  {
    m_publish_thread->join();
    m_start_flag = false;
  }
  return 0;
}

int32_t Publish::addPointCloudMsg(L1DataFrame_S& data_frame)
{
  std::unique_lock<std::mutex> msg_guard(m_msg_mutex);
  m_pointcloud_msg = data_frame;
  m_msg_cond.notify_one();
  return 0;
}

int32_t Publish::sendPointCloudMsg(PointCloud2& pointcloud_msg)
{
  pointcloud_msg.header.frame_id = m_frame_id;
#ifdef ROS1
  m_pointcloud_pub.publish(pointcloud_msg);
#elif defined ROS2
  m_pointcloud_pub->publish(pointcloud_msg);
#endif

  return 0;
}

int32_t Publish::addIMUMsg(std::vector<L1IMU_S>& raw_imu_msg)
{
  static ImuMsg imu_msg;

  for(auto it = raw_imu_msg.begin(); it!=raw_imu_msg.end(); it ++)
  {
    imu_msg.angular_velocity.x = it->gyro_x;
    imu_msg.angular_velocity.y = it->gyro_y;
    imu_msg.angular_velocity.z = it->gyro_z;

    imu_msg.linear_acceleration.x = it->accel_x;
    imu_msg.linear_acceleration.y = it->accel_y;
    imu_msg.linear_acceleration.z = it->accel_z;

    if(m_time_sync)
    {
      // 采用设备时间（时间同步后）
      imu_msg.header.stamp.sec = it->clock_cnt / 1000000000;
#ifdef ROS1
      imu_msg.header.stamp.nsec = it->clock_cnt % 1000000000;
#elif defined ROS2
      imu_msg.header.stamp.nanosec = it->clock_cnt % 1000000000;
#endif
    }
    else
    {
      // 采用系统时间
#ifdef ROS1
      imu_msg.header.stamp = ros::Time::now();
#elif defined ROS2
      imu_msg.header.stamp = m_nh->get_clock()->now();
#endif
    }

    sendIMUMsg(imu_msg);
  }

  return 0;
}

int32_t Publish::sendIMUMsg(ImuMsg& imu_msg)
{
  // 离线模式设定IMU频率为200Hz
#ifdef ROS1
  static ros::Rate rate_loop(200);
#elif defined ROS2
  static rclcpp::WallRate rate_loop(200);
#endif
  imu_msg.header.frame_id = m_frame_id;
#ifdef ROS1
  m_imu_pub.publish(imu_msg);
#elif defined ROS2
  m_imu_pub->publish(imu_msg);
#endif

  if(!m_is_real_time_flag)
  {
    rate_loop.sleep();
  }
  return 0;
}

int32_t Publish::sendMsg()
{
  int32_t frame_id = 0;
  uint32_t point_num = 0;
  if(m_is_sum_pointcloud)
  {
    m_set_frame_num = (uint8_t)(m_scan_speed * 0.1);
  }
  uint8_t cur_frame_num = 0;
  int offset = 0;
  PointCloud2 raw_msg;
  raw_msg.fields.clear();
  raw_msg.fields.reserve(7);

  offset = addPointField(raw_msg, "x", 1, PointField::FLOAT32, offset);
  offset = addPointField(raw_msg, "y", 1, PointField::FLOAT32, offset);
  offset = addPointField(raw_msg, "z", 1, PointField::FLOAT32, offset);
  offset = addPointField(raw_msg, "intensity", 1, PointField::FLOAT32, offset);
  offset = addPointField(raw_msg, "ring", 1, PointField::UINT16, offset);
  offset = addPointField(raw_msg, "flag", 1, PointField::UINT16, offset);
  offset = addPointField(raw_msg, "timestamp", 1, PointField::FLOAT64, offset);
  raw_msg.point_step = offset;
  raw_msg.height = 1;

  while(true)
  {
    std::unique_lock<std::mutex> msg_guard(m_msg_mutex);

    m_msg_cond.wait(msg_guard, [this]{if(m_pointcloud_msg.save_num) return true;else return false;});

    uint64_t time_start, time_end;
    if(0 == cur_frame_num)
    {
      // 最大250000个有效回波
      raw_msg.width = 250000;
      raw_msg.row_step = raw_msg.width * raw_msg.point_step;
      raw_msg.data.resize(raw_msg.row_step);
      time_start = 0xFFFFFFFFFFFFFFFF, time_end = 0;
    }

    // 初始化迭代器
    PointCloud2Iterator<float> iter_x(raw_msg, "x");
    PointCloud2Iterator<float> iter_y(raw_msg, "y");
    PointCloud2Iterator<float> iter_z(raw_msg, "z");
    PointCloud2Iterator<float> iter_i(raw_msg, "intensity");
    PointCloud2Iterator<uint16_t> iter_r(raw_msg, "ring");
    PointCloud2Iterator<uint16_t> iter_f(raw_msg, "flag");
    PointCloud2Iterator<double> iter_t(raw_msg, "timestamp");

    // 如果为dome设备，需要累积指定时间扫描帧
    // TODO: 后续改为sdk累积，取用数据帧数据
    if(cur_frame_num < m_set_frame_num)
    {
      iter_x += point_num;
      iter_y += point_num;
      iter_z += point_num;
      iter_i += point_num;
      iter_r += point_num;
      iter_f += point_num;
      iter_t += point_num;
    }

    // 取点并释放扫描帧内存
    for (int32_t i = 0; i < m_pointcloud_msg.save_num; ++i)
    {
      L1FrameEcho& scan = *m_pointcloud_msg.scan_frame[i];
      for(int32_t row = 0; row < scan.mat_row; ++row)
      {
        for(uint32_t col = 0; col < scan.mat_column; ++col)
        {
          L1XYZShots_S& point = scan.point_matrix[row][col];
          for(int32_t num = 0; num < point.echo_num; ++num)
          {
            if(!point.xyz_cell[num].flag) continue;

            if(extrinsic_enable)
            {
              *iter_x = point.xyz_cell[num].X * extrinsic_R[0][0] +
                        point.xyz_cell[num].Y * extrinsic_R[0][1] +
                        point.xyz_cell[num].Z * extrinsic_R[0][2] + extrinsic_t[0];
              *iter_y = point.xyz_cell[num].X * extrinsic_R[1][0] +
                        point.xyz_cell[num].Y * extrinsic_R[1][1] +
                        point.xyz_cell[num].Z * extrinsic_R[1][2] + extrinsic_t[1];
              *iter_z = point.xyz_cell[num].X * extrinsic_R[2][0] +
                        point.xyz_cell[num].Y * extrinsic_R[2][1] +
                        point.xyz_cell[num].Z * extrinsic_R[2][2] + extrinsic_t[2];
            }
            else
            {
              *iter_x = point.xyz_cell[num].X;
              *iter_y = point.xyz_cell[num].Y;
              *iter_z = point.xyz_cell[num].Z;
            }


            *iter_i = (float)point.xyz_cell[num].intensity;
            *iter_r = point.laser_num;
            *iter_f = point.xyz_cell[num].echo_match;
            *iter_t = (double)point.utc_ntp_time;

            // 取出最大最小时间戳
            if(time_start > point.utc_ntp_time)
            {
              time_start = point.utc_ntp_time;
            }
            if(time_end < point.utc_ntp_time)
            {
              time_end = point.utc_ntp_time;
            }

            ++iter_x;
            ++iter_y;
            ++iter_z;
            ++iter_i;
            ++iter_r;
            ++iter_f;
            ++iter_t;
            ++point_num;
          }
          point.echo_num = 0;
        }
      }
    }
    Calc::getInstance()->freeDataFrame(&m_pointcloud_msg);

    cur_frame_num++;
    ++frame_id;

    if(cur_frame_num >= m_set_frame_num)
    {
      raw_msg.width = point_num;
      raw_msg.row_step = raw_msg.width * raw_msg.point_step;
      raw_msg.data.resize(raw_msg.row_step);

      if(m_time_sync)
      {
        // 采用设备时间（时间同步后）
        raw_msg.header.stamp.sec = time_end / 1000000000;
#ifdef ROS1
        raw_msg.header.stamp.nsec = time_end % 1000000000;
#elif defined ROS2
        raw_msg.header.stamp.nanosec = time_end % 1000000000;
#endif
        // printf("start %lu end %lu\n", time_start, time_end);
        // std::cout << "sync " << raw_msg.header.stamp << std::endl;
        // std::cout << "real "<< ros::Time::now() << std::endl;
      }
      else
      {
        // 采用系统时间
#ifdef ROS1
        raw_msg.header.stamp = ros::Time::now();
#elif defined ROS2
        raw_msg.header.stamp = m_nh->get_clock()->now();
#endif
      }
      sendPointCloudMsg(raw_msg);

      cur_frame_num = 0;
      point_num = 0;
    }
  }

  return 0;
}

int32_t pointcloudCallback(L1DataFrame_S& data_frame)
{
  static Publish* pub = Publish::getInstance();
  static bool rate_flag = pub->getRealTimeFlag();
#ifdef ROS1
  static ros::Rate rate_loop(pub->getScanSpeed());
#elif defined ROS2
  static rclcpp::WallRate rate_loop(pub->getScanSpeed());
#endif


  pub->addPointCloudMsg(data_frame);

  if(!rate_flag)
  {
    rate_loop.sleep();
  }
  return 0;
}

int32_t imuCallback(std::vector<L1IMU_S>& raw_imu_msg)
{

  Publish::getInstance()->addIMUMsg(raw_imu_msg);
  return 0;
}

void updateScanSpeed(int32_t speed)
{
  Publish::getInstance()->setScanSpeed(speed);
}