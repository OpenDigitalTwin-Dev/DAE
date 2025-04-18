// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*
 * RTPSReader.cpp
 *
 */

#include <typeinfo>
#include <algorithm>
#include <chrono>

#include <fastdds/rtps/reader/RTPSReader.h>

#include <fastdds/dds/log/Log.hpp>
#include <fastdds/rtps/history/ReaderHistory.h>
#include <fastdds/rtps/reader/LocalReaderPointer.hpp>
#include <fastdds/rtps/reader/ReaderListener.h>
#include <fastdds/rtps/resources/ResourceEvent.h>

#include <foonathan/memory/namespace_alias.hpp>

#include <statistics/rtps/StatisticsBase.hpp>

#include <rtps/DataSharing/DataSharingListener.hpp>
#include <rtps/history/BasicPayloadPool.hpp>
#include <rtps/history/CacheChangePool.h>
#include <rtps/participant/RTPSParticipantImpl.h>
#include <rtps/reader/ReaderHistoryState.hpp>


namespace eprosima {
namespace fastrtps {
namespace rtps {

RTPSReader::RTPSReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        ReaderHistory* hist,
        ReaderListener* rlisten)
    : Endpoint(pimpl, guid, att.endpoint)
    , mp_history(hist)
    , mp_listener(rlisten)
    , m_acceptMessagesToUnknownReaders(true)
    , m_acceptMessagesFromUnkownWriters(false)
    , m_expectsInlineQos(att.expectsInlineQos)
    , history_state_(new ReaderHistoryState(att.matched_writers_allocation.initial))
    , liveliness_kind_(att.liveliness_kind_)
    , liveliness_lease_duration_(att.liveliness_lease_duration)
{
    PoolConfig cfg = PoolConfig::from_history_attributes(hist->m_att);
    std::shared_ptr<IChangePool> change_pool;
    std::shared_ptr<IPayloadPool> payload_pool;
    payload_pool = BasicPayloadPool::get(cfg, change_pool);

    init(payload_pool, change_pool, att);
}

RTPSReader::RTPSReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        const std::shared_ptr<IPayloadPool>& payload_pool,
        ReaderHistory* hist,
        ReaderListener* rlisten)
    : RTPSReader(
        pimpl, guid, att, payload_pool,
        std::make_shared<CacheChangePool>(PoolConfig::from_history_attributes(hist->m_att)),
        hist, rlisten)
{
}

RTPSReader::RTPSReader(
        RTPSParticipantImpl* pimpl,
        const GUID_t& guid,
        const ReaderAttributes& att,
        const std::shared_ptr<IPayloadPool>& payload_pool,
        const std::shared_ptr<IChangePool>& change_pool,
        ReaderHistory* hist,
        ReaderListener* rlisten)
    : Endpoint(pimpl, guid, att.endpoint)
    , mp_history(hist)
    , mp_listener(rlisten)
    , m_acceptMessagesToUnknownReaders(true)
    , m_acceptMessagesFromUnkownWriters(false)
    , m_expectsInlineQos(att.expectsInlineQos)
    , history_state_(new ReaderHistoryState(att.matched_writers_allocation.initial))
    , liveliness_kind_(att.liveliness_kind_)
    , liveliness_lease_duration_(att.liveliness_lease_duration)
{
    init(payload_pool, change_pool, att);
}

void RTPSReader::local_actions_on_reader_removed()
{
    local_ptr_->deactivate();
}

void RTPSReader::init(
        const std::shared_ptr<IPayloadPool>& payload_pool,
        const std::shared_ptr<IChangePool>& change_pool,
        const ReaderAttributes& att)
{
    payload_pool_ = payload_pool;
    change_pool_ = change_pool;
    fixed_payload_size_ = 0;
    if (mp_history->m_att.memoryPolicy == PREALLOCATED_MEMORY_MODE)
    {
        fixed_payload_size_ = mp_history->m_att.payloadMaxSize;
    }

    if (att.endpoint.data_sharing_configuration().kind() != OFF)
    {
        using std::placeholders::_1;
        std::shared_ptr<DataSharingNotification> notification =
                DataSharingNotification::create_notification(
            getGuid(), att.endpoint.data_sharing_configuration().shm_directory());
        if (notification)
        {
            is_datasharing_compatible_ = true;
            datasharing_listener_.reset(new DataSharingListener(
                        notification,
                        att.endpoint.data_sharing_configuration().shm_directory(),
                        att.data_sharing_listener_thread,
                        att.matched_writers_allocation,
                        this));

            // We can start the listener here, as no writer can be matched already,
            // so no notification will occur until the non-virtual instance is constructed.
            // But we need to stop the listener in the non-virtual instance destructor.
            datasharing_listener_->start();
        }
    }

    mp_history->mp_reader = this;
    mp_history->mp_mutex = &mp_mutex;

    local_ptr_ = std::make_shared<LocalReaderPointer>(this);

    EPROSIMA_LOG_INFO(RTPS_READER, "RTPSReader created correctly");
}

RTPSReader::~RTPSReader()
{
    EPROSIMA_LOG_INFO(RTPS_READER, "Removing reader " << this->getGuid().entityId; );

    for (auto it = mp_history->changesBegin(); it != mp_history->changesEnd(); ++it)
    {
        releaseCache(*it);
    }

    delete history_state_;
    mp_history->mp_reader = nullptr;
    mp_history->mp_mutex = nullptr;
}

bool RTPSReader::reserveCache(
        CacheChange_t** change,
        uint32_t dataCdrSerializedSize)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);

    *change = nullptr;

    CacheChange_t* reserved_change = nullptr;
    if (!change_pool_->reserve_cache(reserved_change))
    {
        EPROSIMA_LOG_WARNING(RTPS_READER, "Problem reserving cache from pool");
        return false;
    }

    uint32_t payload_size = fixed_payload_size_ ? fixed_payload_size_ : dataCdrSerializedSize;
    if (!payload_pool_->get_payload(payload_size, *reserved_change))
    {
        change_pool_->release_cache(reserved_change);
        EPROSIMA_LOG_WARNING(RTPS_READER, "Problem reserving payload from pool");
        return false;
    }

    *change = reserved_change;
    return true;
}

void RTPSReader::releaseCache(
        CacheChange_t* change)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);

    IPayloadPool* pool = change->payload_owner();
    if (pool)
    {
        pool->release_payload(*change);
    }
    change_pool_->release_cache(change);
}

ReaderListener* RTPSReader::getListener() const
{
    return mp_listener;
}

bool RTPSReader::setListener(
        ReaderListener* target)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    mp_listener = target;
    return true;
}

std::shared_ptr<LocalReaderPointer> RTPSReader::get_local_pointer()
{
    return local_ptr_;
}

History::const_iterator RTPSReader::findCacheInFragmentedProcess(
        const SequenceNumber_t& sequence_number,
        const GUID_t& writer_guid,
        CacheChange_t** change,
        History::const_iterator hint) const
{
    History::const_iterator ret_val = mp_history->get_change_nts(sequence_number, writer_guid, change, hint);

    if (nullptr != *change && (*change)->is_fully_assembled())
    {
        *change = nullptr;
    }

    return ret_val;
}

void RTPSReader::add_persistence_guid(
        const GUID_t& guid,
        const GUID_t& persistence_guid)
{
    if (c_Guid_Unknown == persistence_guid || persistence_guid == guid)
    {
        std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
        history_state_->persistence_guid_map[guid] = guid;
        history_state_->persistence_guid_count[guid]++;
    }
    else
    {
        std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
        history_state_->persistence_guid_map[guid] = persistence_guid;
        history_state_->persistence_guid_count[persistence_guid]++;

        // Could happen that a value has already been stored in the record with the guid and not the persistence guid
        // This is because received_change is called before Proxy is created
        // In this case, we substitute the guid for the persistence (in case they are not equal)
        auto spourious_record = history_state_->history_record.find(guid);
        if (spourious_record != history_state_->history_record.end())
        {
            EPROSIMA_LOG_INFO(RTPS_READER, "Sporious record found, changing guid "
                    << guid << " for persistence guid " << persistence_guid);
            update_last_notified(guid, spourious_record->second);
            history_state_->history_record.erase(spourious_record);
        }
    }
}

bool RTPSReader::may_remove_history_record(
        bool removed_by_lease)
{
    return !removed_by_lease;
}

void RTPSReader::remove_persistence_guid(
        const GUID_t& guid,
        const GUID_t& persistence_guid,
        bool removed_by_lease)
{
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    GUID_t persistence_guid_stored = (c_Guid_Unknown == persistence_guid) ? guid : persistence_guid;
    history_state_->persistence_guid_map.erase(guid);
    auto count = --history_state_->persistence_guid_count[persistence_guid_stored];
    if (count <= 0 && may_remove_history_record(removed_by_lease))
    {
        history_state_->history_record.erase(persistence_guid_stored);
        history_state_->persistence_guid_count.erase(persistence_guid_stored);
    }
}

SequenceNumber_t RTPSReader::update_last_notified(
        const GUID_t& guid,
        const SequenceNumber_t& seq)
{
    SequenceNumber_t ret_val;
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    GUID_t guid_to_look = guid;
    auto p_guid = history_state_->persistence_guid_map.find(guid);
    if (p_guid != history_state_->persistence_guid_map.end())
    {
        guid_to_look = p_guid->second;
    }

    auto p_seq = history_state_->history_record.find(guid_to_look);
    if (p_seq != history_state_->history_record.end())
    {
        ret_val = p_seq->second;
    }

    if (ret_val < seq)
    {
        set_last_notified(guid_to_look, seq);
        new_notification_cv_.notify_all();
    }

    return ret_val;
}

SequenceNumber_t RTPSReader::get_last_notified(
        const GUID_t& guid)
{
    SequenceNumber_t ret_val;
    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    GUID_t guid_to_look = guid;
    auto p_guid = history_state_->persistence_guid_map.find(guid);
    if (p_guid != history_state_->persistence_guid_map.end())
    {
        guid_to_look = p_guid->second;
    }

    auto p_seq = history_state_->history_record.find(guid_to_look);
    if (p_seq != history_state_->history_record.end())
    {
        ret_val = p_seq->second;
    }

    return ret_val;
}

void RTPSReader::set_last_notified(
        const GUID_t& peristence_guid,
        const SequenceNumber_t& seq)
{
    history_state_->history_record[peristence_guid] = seq;
}

bool RTPSReader::wait_for_unread_cache(
        const eprosima::fastrtps::Duration_t& timeout)
{
    auto time_out = std::chrono::steady_clock::now() + std::chrono::seconds(timeout.seconds) +
            std::chrono::nanoseconds(timeout.nanosec);

#if HAVE_STRICT_REALTIME
    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex, std::defer_lock);
    if (lock.try_lock_until(time_out))
#else
    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);
#endif  // HAVE_STRICT_REALTIME
    {
        if (new_notification_cv_.wait_until(
                    lock, time_out,
                    [&]()
                    {
                        return total_unread_ > 0;
                    }))
        {
            return true;
        }
    }

    return false;
}

uint64_t RTPSReader::get_unread_count() const
{
    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);
    return total_unread_;
}

uint64_t RTPSReader::get_unread_count(
        bool mark_as_read)
{
    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);
    uint64_t ret_val = total_unread_;

    if (mark_as_read)
    {
        for (auto it = mp_history->changesBegin(); 0 < total_unread_ && it != mp_history->changesEnd(); ++it)
        {
            CacheChange_t* change = *it;
            if (!change->isRead && get_last_notified(change->writerGUID) >= change->sequenceNumber)
            {
                change->isRead = true;
                assert(0 < total_unread_);
                --total_unread_;
            }
        }
        assert(0 == total_unread_);
    }
    return ret_val;
}

bool RTPSReader::is_datasharing_compatible_with(
        const WriterProxyData& wdata)
{
    if (!is_datasharing_compatible_ ||
            wdata.m_qos.data_sharing.kind() == fastdds::dds::OFF)
    {
        return false;
    }

    for (auto id : wdata.m_qos.data_sharing.domain_ids())
    {
        if (std::find(m_att.data_sharing_configuration().domain_ids().begin(),
                m_att.data_sharing_configuration().domain_ids().end(), id)
                != m_att.data_sharing_configuration().domain_ids().end())
        {
            return true;
        }
    }

    return false;
}

bool RTPSReader::is_sample_valid(
        const void* data,
        const GUID_t& writer,
        const SequenceNumber_t& sn) const
{
    if (is_datasharing_compatible_ && datasharing_listener_->writer_is_matched(writer))
    {
        // Check if the payload is dirty
        // Note the Payloads used in loans include a mandatory RTPS 2.3 extra header
        if (!DataSharingPayloadPool::check_sequence_number(
                    static_cast<const octet*>(data) - SerializedPayload_t::representation_header_size,
                    sn))
        {
            return false;
        }
    }
    return true;
}

#ifdef FASTDDS_STATISTICS

bool RTPSReader::add_statistics_listener(
        std::shared_ptr<fastdds::statistics::IListener> listener)
{
    return add_statistics_listener_impl(listener);
}

bool RTPSReader::remove_statistics_listener(
        std::shared_ptr<fastdds::statistics::IListener> listener)
{
    return remove_statistics_listener_impl(listener);
}

void RTPSReader::set_enabled_statistics_writers_mask(
        uint32_t enabled_writers)
{
    set_enabled_statistics_writers_mask_impl(enabled_writers);
}

#endif // FASTDDS_STATISTICS

} /* namespace rtps */
} /* namespace fastrtps */
} /* namespace eprosima */
