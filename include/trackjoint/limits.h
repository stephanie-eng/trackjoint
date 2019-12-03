/*********************************************************************
 * Copyright (c) 2019, PickNik Consulting
 * All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *********************************************************************/

#pragma once

namespace trackjoint {
/**
 * \brief Maximum dynamics and parameters of TrackJoint
 */
struct Limits {
  // Scalar limit on angular velocity [rad/s]
  double velocity_limit;
  // Scalar limit on angular acceleration [rad/s^2]
  double acceleration_limit;
  // Scalar limit on angular jerk [rad/s^3]
  double jerk_limit;
};
}  // namespace trackjoint
