/**
 * @file main.cpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @license License BSD-3-Clause
 * @copyright Copyright (c) 2019, New York University and Max Planck
 * Gesellschaft.
 * @date 2019-05-22
 *
 * @brief Main file that runs all unittest using gtest
 * @see
 * https://git-amd.tuebingen.mpg.de/amd-clmc/ci_example/wikis/catkin:-how-to-implement-unit-tests
 */

#include <gtest/gtest.h>

#include <ros/ros.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "ros_wbmpc_gtest");
  if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug)) {
    ros::console::notifyLoggerLevelsChanged();
  }
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  ros::shutdown();
}