// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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
//
// SPDX-License-Identifier: Apache-2.0
#ifndef IOX_POSH_POPO_BUILDING_BLOCKS_CHUNK_DISTRIBUTOR_INL
#define IOX_POSH_POPO_BUILDING_BLOCKS_CHUNK_DISTRIBUTOR_INL

#include "iceoryx_posh/internal/popo/building_blocks/chunk_distributor.hpp"

namespace iox
{
namespace popo
{
template <typename ChunkDistributorDataType>
inline ChunkDistributor<ChunkDistributorDataType>::ChunkDistributor(
    cxx::not_null<MemberType_t* const> chunkDistrubutorDataPtr) noexcept
    : m_chunkDistrubutorDataPtr(chunkDistrubutorDataPtr)
{
}

template <typename ChunkDistributorDataType>
inline const typename ChunkDistributor<ChunkDistributorDataType>::MemberType_t*
ChunkDistributor<ChunkDistributorDataType>::getMembers() const noexcept
{
    return m_chunkDistrubutorDataPtr;
}

template <typename ChunkDistributorDataType>
inline typename ChunkDistributor<ChunkDistributorDataType>::MemberType_t*
ChunkDistributor<ChunkDistributorDataType>::getMembers() noexcept
{
    return m_chunkDistrubutorDataPtr;
}

template <typename ChunkDistributorDataType>
inline cxx::expected<ChunkDistributorError>
ChunkDistributor<ChunkDistributorDataType>::tryAddQueue(cxx::not_null<ChunkQueueData_t* const> queueToAdd,
                                                        const uint64_t requestedHistory) noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    const auto alreadyKnownReceiver =
        std::find_if(getMembers()->m_queues.begin(),
                     getMembers()->m_queues.end(),
                     [&](const ChunkQueueData_t* const queue) { return queue == queueToAdd; });

    // check if the queue is not already in the list
    if (alreadyKnownReceiver == getMembers()->m_queues.end())
    {
        if (getMembers()->m_queues.size() < getMembers()->m_queues.capacity())
        {
            // PRQA S 3804 1 # we checked the capacity, so pushing will be fine
            getMembers()->m_queues.push_back(rp::RelativePointer<ChunkQueueData_t>(queueToAdd));

            const auto currChunkHistorySize = getMembers()->m_history.size();

            if (requestedHistory > getMembers()->m_historyCapacity)
            {
                LogWarn() << "Chunk history request exceeds history capacity! Request is " << requestedHistory
                          << ". Capacity is " << getMembers()->m_historyCapacity << ".";
            }

            // if the current history is large enough we send the requested number of chunks, else we send the
            // total history
            const auto startIndex =
                (requestedHistory <= currChunkHistorySize) ? currChunkHistorySize - requestedHistory : 0u;
            for (auto i = startIndex; i < currChunkHistorySize; ++i)
            {
                pushToQueue(queueToAdd, getMembers()->m_history[i].cloneToSharedChunk());
            }

            return cxx::success<void>();
        }
        else
        {
            // that's not the fault of the chunk distributor user, we report a moderate error and indicate that adding
            // the queue was not possible
            errorHandler(Error::kPOPO__CHUNK_DISTRIBUTOR_OVERFLOW_OF_QUEUE_CONTAINER, nullptr, ErrorLevel::MODERATE);

            return cxx::error<ChunkDistributorError>(ChunkDistributorError::QUEUE_CONTAINER_OVERFLOW);
        }
    }

    return cxx::success<void>();
}

template <typename ChunkDistributorDataType>
inline cxx::expected<ChunkDistributorError> ChunkDistributor<ChunkDistributorDataType>::tryRemoveQueue(
    cxx::not_null<ChunkQueueData_t* const> queueToRemove) noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    const auto iter = std::find(getMembers()->m_queues.begin(), getMembers()->m_queues.end(), queueToRemove);
    if (iter != getMembers()->m_queues.end())
    {
        // PRQA S 3804 1 # we don't use iter any longer so return value can be ignored
        getMembers()->m_queues.erase(iter);

        return cxx::success<void>();
    }
    else
    {
        return cxx::error<ChunkDistributorError>(ChunkDistributorError::QUEUE_NOT_IN_CONTAINER);
    }
}

template <typename ChunkDistributorDataType>
inline void ChunkDistributor<ChunkDistributorDataType>::removeAllQueues() noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    getMembers()->m_queues.clear();
}

template <typename ChunkDistributorDataType>
inline bool ChunkDistributor<ChunkDistributorDataType>::hasStoredQueues() const noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    return !getMembers()->m_queues.empty();
}

#if (defined(__GNUC__) && __GNUC__ >= 13 && !defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif

template <typename ChunkDistributorDataType>
inline uint64_t ChunkDistributor<ChunkDistributorDataType>::deliverToAllStoredQueues(mepoo::SharedChunk chunk) noexcept
{
    uint64_t numberOfQueuesTheChunkWasDeliveredTo{0U};
    typename ChunkDistributorDataType::QueueContainer_t remainingQueues;
    {
        typename MemberType_t::LockGuard_t lock(*getMembers());

        bool willWaitForConsumer = getMembers()->m_consumerTooSlowPolicy == ConsumerTooSlowPolicy::WAIT_FOR_CONSUMER;
        // send to all the queues
        for (auto& queue : getMembers()->m_queues)
        {
            bool isBlockingQueue = (willWaitForConsumer && queue->m_queueFullPolicy == QueueFullPolicy::BLOCK_PRODUCER);

            if (pushToQueue(queue.get(), chunk))
            {
                ++numberOfQueuesTheChunkWasDeliveredTo;
            }
            else
            {
                if (isBlockingQueue)
                {
                    remainingQueues.emplace_back(queue);
                }
                else
                {
                    ++numberOfQueuesTheChunkWasDeliveredTo;
                    ChunkQueuePusher_t(queue.get()).lostAChunk();
                }
            }
        }
    }

    // busy waiting until every queue is served
    while (!remainingQueues.empty())
    {
        std::this_thread::yield();
        {
            // create intersection of current queues and remainingQueues
            // reason: it is possible that since the last iteration some subscriber have already unsubscribed
            //          and without this intersection we would deliver to dead queues
            typename MemberType_t::LockGuard_t lock(*getMembers());
            typename ChunkDistributorDataType::QueueContainer_t queueIntersection(remainingQueues.size());
            std::sort(getMembers()->m_queues.begin(), getMembers()->m_queues.end());
            std::sort(remainingQueues.begin(), remainingQueues.end());

            auto iter = std::set_intersection(getMembers()->m_queues.begin(),
                                              getMembers()->m_queues.end(),
                                              remainingQueues.begin(),
                                              remainingQueues.end(),
                                              queueIntersection.begin());
            queueIntersection.resize(static_cast<uint64_t>(iter - queueIntersection.begin()));
            remainingQueues = queueIntersection;

            // deliver to remaining queues
            for (uint64_t i = remainingQueues.size() - 1U; !remainingQueues.empty(); --i)
            {
                if (pushToQueue(remainingQueues[i].get(), chunk))
                {
                    remainingQueues.erase(remainingQueues.begin() + i);
                    ++numberOfQueuesTheChunkWasDeliveredTo;
                }

                // don't move this up since the for loop counts downwards and the algorithm would break
                if (i == 0U)
                {
                    break;
                }
            }
        }
    }

    addToHistoryWithoutDelivery(chunk);

    return numberOfQueuesTheChunkWasDeliveredTo;
}

#if (defined(__GNUC__) && __GNUC__ >= 13 && !defined(__clang__))
#pragma GCC diagnostic pop
#endif

template <typename ChunkDistributorDataType>
inline bool ChunkDistributor<ChunkDistributorDataType>::pushToQueue(cxx::not_null<ChunkQueueData_t* const> queue,
                                                                    mepoo::SharedChunk chunk) noexcept
{
    return ChunkQueuePusher_t(queue).push(chunk);
}

template <typename ChunkDistributorDataType>
inline cxx::expected<ChunkDistributorError>
ChunkDistributor<ChunkDistributorDataType>::deliverToQueue(const cxx::UniqueId uniqueQueueId,
                                                           const uint32_t lastKnownQueueIndex,
                                                           mepoo::SharedChunk chunk IOX_MAYBE_UNUSED) noexcept
{
    bool retry{false};
    do
    {
        typename MemberType_t::LockGuard_t lock(*getMembers());

        auto queueIndex = getQueueIndex(uniqueQueueId, lastKnownQueueIndex);

        if (!queueIndex.has_value())
        {
            return cxx::error<ChunkDistributorError>(ChunkDistributorError::QUEUE_NOT_IN_CONTAINER);
        }

        auto& queue = getMembers()->m_queues[queueIndex.value()];

        bool willWaitForConsumer = getMembers()->m_consumerTooSlowPolicy == ConsumerTooSlowPolicy::WAIT_FOR_CONSUMER;

        bool isBlockingQueue = (willWaitForConsumer && queue->m_queueFullPolicy == QueueFullPolicy::BLOCK_PRODUCER);

        retry = false;
        if (!pushToQueue(queue.get(), chunk))
        {
            if (isBlockingQueue)
            {
                retry = true;
            }
            else
            {
                ChunkQueuePusher_t(queue.get()).lostAChunk();
            }
        }
    } while (retry);

    return cxx::success<>();
}

template <typename ChunkDistributorDataType>
inline cxx::optional<uint32_t>
ChunkDistributor<ChunkDistributorDataType>::getQueueIndex(const cxx::UniqueId uniqueQueueId,
                                                          const uint32_t lastKnownQueueIndex) const noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    auto& queues = getMembers()->m_queues;

    if (queues.size() > lastKnownQueueIndex && queues[lastKnownQueueIndex]->m_uniqueId == uniqueQueueId)
    {
        return lastKnownQueueIndex;
    }

    uint32_t index{0};
    for (auto& queue : queues)
    {
        if (queue->m_uniqueId == uniqueQueueId)
        {
            return index;
        }
        ++index;
    }
    return cxx::nullopt;
}

template <typename ChunkDistributorDataType>
inline void ChunkDistributor<ChunkDistributorDataType>::addToHistoryWithoutDelivery(mepoo::SharedChunk chunk) noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    if (0u < getMembers()->m_historyCapacity)
    {
        if (getMembers()->m_history.size() >= getMembers()->m_historyCapacity)
        {
            auto chunkToRemove = getMembers()->m_history.begin();
            chunkToRemove->releaseToSharedChunk();
            // PRQA S 3804 1 # we are not iterating here, so return value can be ignored
            getMembers()->m_history.erase(chunkToRemove);
        }
        // PRQA S 3804 1 # we ensured that there is space in the history, so return value can be ignored
        getMembers()->m_history.push_back(chunk); // PRQA S 3804
    }
}

template <typename ChunkDistributorDataType>
inline uint64_t ChunkDistributor<ChunkDistributorDataType>::getHistorySize() noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    return getMembers()->m_history.size();
}

template <typename ChunkDistributorDataType>
inline uint64_t ChunkDistributor<ChunkDistributorDataType>::getHistoryCapacity() const noexcept
{
    return getMembers()->m_historyCapacity;
}

template <typename ChunkDistributorDataType>
inline void ChunkDistributor<ChunkDistributorDataType>::clearHistory() noexcept
{
    typename MemberType_t::LockGuard_t lock(*getMembers());

    for (auto& unmanagedChunk : getMembers()->m_history)
    {
        unmanagedChunk.releaseToSharedChunk();
    }

    getMembers()->m_history.clear();
}

template <typename ChunkDistributorDataType>
inline void ChunkDistributor<ChunkDistributorDataType>::cleanup() noexcept
{
    if (getMembers()->tryLock())
    {
        clearHistory();
        getMembers()->unlock();
    }
    else
    {
        /// @todo currently we have a deadlock / mutex destroy vulnerability if the ThreadSafePolicy is used
        /// and a sending application dies when having the lock for sending. If the RouDi daemon wants to
        /// cleanup or does discovery changes we have a deadlock or an exception when destroying the mutex
        /// As long as we don't have a multi-threaded lock-free ChunkDistributor or another concept we die here
        errorHandler(Error::kPOPO__CHUNK_DISTRIBUTOR_CLEANUP_DEADLOCK_BECAUSE_BAD_APPLICATION_TERMINATION,
                     nullptr,
                     ErrorLevel::FATAL);
    }
}

} // namespace popo
} // namespace iox

#endif // IOX_POSH_POPO_BUILDING_BLOCKS_CHUNK_DISTRIBUTOR_INL
