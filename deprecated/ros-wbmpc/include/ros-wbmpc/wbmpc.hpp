/*
 * Copyright 2020 PAL Robotics SL. All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited,
 * unless it was supplied under the terms of a license agreement or
 * nondisclosure agreement with PAL Robotics SL. In this case it may not be
 * copied or disclosed except in accordance with the terms of that agreement.
 */
#ifndef ROS_WBMPC_WBMPC_HPP
#define ROS_WBMPC_WBMPC_HPP

#include <pinocchio/fwd.hpp>
#include <pinocchio/multibody/model.hpp>

// ROS C++ api
#include <ros/ros.h>

// ROS Messages.
#include <linear_feedback_controller_msgs/Sensor.h>
#include <linear_feedback_controller_msgs/Control.h>

// #include <boost/algorithm/string.hpp>
// #include <algorithm>
// #include <pinocchio/parsers/urdf.hpp>
// #include <pinocchio/parsers/srdf.hpp>
// #include <pinocchio/algorithm/joint-configuration.hpp>
// #include <tf/transform_broadcaster.h>
// #include <tf2_msgs/TFMessage.h>
// #include <pinocchio/algorithm/model.hpp>

// #include <pal_base_ros_controller/base_robot_with_estimator_controller.h>
// #include <realtime_tools/realtime_publisher.h>
// #include <sensor_msgs/JointState.h>
// #include <memmo_trajectory_controller/JointStateLPF.h>
// #include <ddynamic_reconfigure/ddynamic_reconfigure.h>
// #include <nav_msgs/Odometry.h>

namespace ros_wbmpc {

/**
 * @brief This class has for purpose to connect Whole Body Model Predictive
 * Controllers from https://github.com/loco-3d/crocoddyl and
 * https://github.com/MeMory-of-MOtion/sobec to the low level controller.
 *
 * This part of the controller interpolate the controls from the Ricatti gains
 * and the feed-forward terms. It runs at the robot low level frequency.
 */
class RosWBMPC {
 public:  // Methods
  RosWBMPC();

  ~RosWBMPC();

  /**
   * @brief Allocate all internal memory and spawn the ROS Topics for sending
   * the sensors and receiving the controls.
   *
   * @param node_handle this is the ROS object allowing us to use the middleware
   * ROS. It is responsible for the creation of the "Subsribers" and
   * "Publishers".
   */
  void initialize(const ros::NodeHandle& node_handle);

  void parse_controlled_joint_names(
    const std::vector<std::string>& controlled_joint_names,
    std::vector<std::string>& controlled_joint_names,
    std::vector<std::string>& controlled_joint_ids);

  /**
   * @brief Run one iteration of control. It computes the low level control from
   * the received Control message.
   *
   * It check the date of the last sent controls for safety and flags if there
   * is an issue.
   */
  bool run_one_iteration();

  const std::vector<std::string>& get_controlled_joint_names(){
    return controlled_joint_names_;
  }

  const std::vector<long unsigned int>& get_controlled_joint_ids(){
    return controlled_joint_ids_;
  }

  const std::vector<long unsigned int>& get_locked_joint_ids(){
    return locked_joints_ids_;
  }

 private:  // Methods
  // void controlCb(const memmo_trajectory_controller::JointStateLPF &desired_state);
 private:  // Members
  
  // Settings:
  /// @brief String containing the model of the robot in xml/urdf format.
  std::string in_urdf_;
  /// @brief String containing the extras of the model of the robot.
  std::string in_srdf_;
  /// @brief List of names that correspond to the joints controlled by the MPC.
  std::vector<std::string> in_controlled_joint_names_;
  /// @brief Are we using a robot that has a free-flyer?
  bool in_robot_has_free_flyer_;
  
  /// @brief ROS handler allowing us to create topics/services etc.
  ros::NodeHandle node_handle_;

  /// @brief Pinocchio (Rigid body dynamics robot model).
  pinocchio::Model pinocchio_model_complete_;
  pinocchio::Model pinocchio_model_reduced_;
  
  /// @brief Controlled joint ids sorted in the urdf order.
  std::vector<pinocchio::JointIndex> controlled_joint_ids_;
  /// @brief Sort the controlled joint names using the urdf order.
  std::vector<std::string> controlled_joint_names_;
  /// @brief Sort the locked (position controlled) joint names using the urdf order.
  std::vector<pinocchio::JointIndex> locked_joints_ids_;

  /// @brief Initial whole body configuration setup in the SRDF file.
  Eigen::VectorXd q_default_complete_;

  /// @brief Actual robot state publisher.
  realtime_tools::RealtimePublisher<linear_feedback_controller_msgs::Sensor> state_publisher_; 
  
  // ros::Subscriber state_sub_;


  // std::timed_mutex mutex_;
  // pinocchio::Model pin_model_;
  // std::vector<std::string> controlled_joint_names_;
  // std::map<std::string, int> actual_state_map_;
  // Eigen::VectorXd desired_effort_;
  // Eigen::VectorXd desired_pos_;
  // Eigen::VectorXd desired_vel_;
  // Eigen::VectorXd actual_pos_;
  // Eigen::VectorXd actual_vel_;
  // Eigen::VectorXd actual_tau_;
  // Eigen::VectorXd desired_tau_;
  // Eigen::VectorXd diff_state_;
  // Eigen::MatrixXd riccati_gains_;
  // memmo_trajectory_controller::JointStateLPF desired_state_;
  // std::vector<double> initial_torque_;
  // std::vector<double> initial_position_;
  // std::vector<double> desired_torque_;
  // std::vector<double> actual_torque_;
  // std::vector<double> torque_offsets_;

  // ddynamic_reconfigure::DDynamicReconfigurePtr dd_reconfigure_;
  // double p_arm_gain_;
  // double d_arm_gain_;
  // double p_torso_gain_;
  // double d_torso_gain_;
  // double p_leg_gain_;
  // double d_leg_gain_;

  // boost::shared_ptr<realtime_tools::RealtimePublisher<linear_feedback_controller_msgs::JointState>> joint_states_pub_;
  // boost::shared_ptr<realtime_tools::RealtimePublisher<nav_msgs::Odometry>> base_state_pub_;
  // sensor_msgs::JointState actual_js_state_;
  // nav_msgs::Odometry actual_base_state_;

  // pal_statistics::RegistrationsRAII registered_variables_;
  // pal_robot_tools::TimeProfilerPtr tp_;
  // int ms_mutex_;

  // Eigen::Vector3d base_linear_vel_;
  // Eigen::Vector3d base_angular_vel_;
  // Eigen::Quaterniond base_orientation_;
  // Eigen::Vector3d base_position_;
  // std::deque<std::vector<double>> v_measures_;
  // std::deque<std::vector<double>> tau_measures_;
  // double nb_measure_;
};

}  // namespace ros_wbmpc

#endif  // ROS_WBMPC_WBMPC_HPP