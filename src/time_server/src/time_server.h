/*
 * Copyright (c) 2016 Shawn LeMaster
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef TIME_SERVER_H
#define TIME_SERVER_H

#include "ros/ros.h"
#include "rosgraph_msgs/Clock.h"
#include "std_msgs/Float32.h"

/// This class manages enables and maintains simulation time for a ROS system.
/// Simulation time is updated in real time at 128 Hz by default but can be
/// commanded to propagate faster (or slower) than real time by posting a
/// Float32 message on the /command_clock_rate topic. E.g., the value 2.0 will
/// cause simulation time to propagate twice as fast as real time.
class TimeServer
{
public:

    TimeServer();
    virtual ~TimeServer();

private:

    ros::WallTimer CreateClockUpdateTimer(float rate = 1.0);

    void UpdateClock(const ros::WallTimerEvent& /*ignored*/);
    void SetClockRate(const std_msgs::Float32::ConstPtr& rateMsg);

    ros::NodeHandle         m_node;                   ///< Handle for this object's ROS node.
    ros::Publisher          m_clockPublisher;         ///< Publisher for /clock (sim time) topic.
    rosgraph_msgs::Clock    m_clock;                  ///< Clock message that is posted on /clock topic.
    const ros::WallDuration m_clockUpdatePeriod;      ///< The period at which the /clock topic is updated.
    ros::WallTimer          m_clockUpdateTimer;       ///< The wall timer for updating the /clock topic.
    ros::Subscriber         m_cmdClockRateSubscriber; ///< Subscribes to the /command_clock_rate topic.
};

#endif  // TIME_SERVER_H
