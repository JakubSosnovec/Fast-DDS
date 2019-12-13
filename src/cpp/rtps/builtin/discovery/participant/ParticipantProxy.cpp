// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file ParticipantProxy.cpp
 *
 */

#include <fastrtps/rtps/builtin/discovery/participant/ParticipantProxy.h>
#include <fastrtps/rtps/resources/TimedEvent.h>

using namespace eprosima::fastrtps;

namespace eprosima {
namespace fastrtps{
namespace rtps {

ParticipantProxy::ParticipantProxy(const RTPSParticipantAllocationAttributes& allocation)
    : should_check_lease_duration_(false)
    , readers_(allocation.readers)
    , writers_(allocation.writers)
    , builtin_readers_(allocation.participants) 
    , builtin_writers_(allocation.participants)
    , lease_duration_event_(nullptr)
{
    // TODO: Barro, increase on security case
    ResourceLimitedContainerConfig builtin;
    builtin.initial = 8 * allocation.participants.increment;
    builtin.maximum = 8 * allocation.participants.maximum;
    builtin.increment = 8 * allocation.participants.increment;

    builtin_readers_ =
        ResourceLimitedVector<std::shared_ptr<ReaderProxyData>>(builtin);
    builtin_writers_ =
        ResourceLimitedVector<std::shared_ptr<WriterProxyData>>(builtin);
}

ParticipantProxy::~ParticipantProxy()
{
    if(lease_duration_event_)
    {
        delete lease_duration_event_;
    }
}

void ParticipantProxy::clear()
{
    readers_.clear();
    writers_.clear();
    builtin_readers_.clear();
    builtin_writers_.clear();

    // Cancel lease event
    if(lease_duration_event_ != nullptr)
    {
        lease_duration_event_->cancel_timer();
    }

    should_check_lease_duration_ = false;

    // release strong reference to proxy data
    proxy_data_.reset();
}

void ParticipantProxy::assert_liveliness()
{
    last_received_message_tm_ = std::chrono::steady_clock::now();
}

} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */