/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2020, PickNik LLC
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of PickNik LLC nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Sebastian Jahr
   Description: Defines an interface for a trajectory operator plugin implementation for the local planner component node.
 */

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <moveit_msgs/msg/robot_trajectory.hpp>
#include <moveit_msgs/msg/constraints.hpp>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

namespace moveit_hybrid_planning
{
/**
 * Class TrajectoryOperatorInterface - Base class for a trajectory operator. The operator's task is manage the local
 * planner's global reference trajectory. This includes trajectory matching based on the current state to identify the
 * current planning problem and blending of new global trajectory updates into the currently processed reference
 * trajectory.
 */
class TrajectoryOperatorInterface
{
public:
  /**
   * Initialize trajectory operator
   * @return True if initialization was successful
   */
  virtual bool initialize(const rclcpp::Node::SharedPtr& node, moveit::core::RobotModelConstPtr robot_model,
                          std::string group_name) = 0;

  /**
   * Add a new reference trajectory segment to the vector of global trajectory segments to process
   * @param new_trajectory New reference trajectory segment to add
   * @return True if segment was successfully added
   */
  virtual bool addTrajectorySegment(const robot_trajectory::RobotTrajectory& new_trajectory) = 0;

  /**
   * Return the current local constraints based on the newest robot state
   * @param current_state Current RobotState
   * @return Current local constraints that define the local planning goal
   */
  virtual std::vector<moveit_msgs::msg::Constraints> getLocalProblem(moveit::core::RobotState current_state) = 0;

  /**
   * Return the processing status of the reference trajectory's execution based on a user defined
   * metric.
   * @param current_state Current RobotState
   * @return A value between 0.0 (start) to 1.0 (completion).
   */
  virtual double getTrajectoryProgress(moveit::core::RobotState current_state) = 0;
  virtual ~TrajectoryOperatorInterface(){};

protected:
  /** \brief Constructor */
  TrajectoryOperatorInterface(){};

  // Reference trajectory to be precessed
  robot_trajectory::RobotTrajectoryPtr reference_trajectory_;
  std::string group_;
};
}  // namespace moveit_hybrid_planning
