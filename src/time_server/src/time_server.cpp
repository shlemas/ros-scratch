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

#include "time_server.h"

/// Constructor.
TimeServer::TimeServer() :
    m_node(),
    m_clockPublisher(m_node.advertise<rosgraph_msgs::Clock>("/clock", 10000)),
    m_clock(),
    m_clockUpdatePeriod(0, 7812500), // 128 Hz
    m_clockUpdateTimer(CreateClockUpdateTimer()),
    m_cmdClockRateSubscriber(m_node.subscribe("/command_clock_rate", 10, &TimeServer::SetClockRate, this))
{
    m_node.setParam("/use_sim_time", true);
}

/// Destructor.
TimeServer::~TimeServer()
{
}

/// @returns A timer that periodically triggers simulation time to be updated
/// and published on the /clock topic. The timer rate is modifiable by a
/// multiplier received as a Float32 message on the /command_clock_rate topic.
/// @param rate The multiplier rate at which the timer should run. Default = 1.0 (real time).
ros::WallTimer TimeServer::CreateClockUpdateTimer(float rate)
{
    return m_node.createWallTimer(m_clockUpdatePeriod * (1.0 / rate), &TimeServer::UpdateClock, this);
}

/// Updates simulation time and publishes it on the /clock topic.
void TimeServer::UpdateClock(const ros::WallTimerEvent& /*ignored*/)
{
    m_clock.clock += ros::Duration(m_clockUpdatePeriod.sec, m_clockUpdatePeriod.nsec);
    ROS_INFO("Clock = %lf", m_clock.clock.toSec());
    m_clockPublisher.publish(m_clock);
}

/// Callback on the /command_clock_rate topic. The received Float32
/// represents the rate of execution that sim time should propagate.
/// @param rateMsg The rate at which sim time should propagate. 1.0 = real time.
void TimeServer::SetClockRate(const std_msgs::Float32::ConstPtr& rateMsg)
{
    m_clockUpdateTimer = CreateClockUpdateTimer(rateMsg->data);
}
