/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Kevin DuBois <kevin.dubois@canonical.com>
 */

#include "mir/compositor/buffer_swapper_multi.h"
#include "mir/compositor/buffer.h"

namespace mc = mir::compositor;

mc::BufferSwapperMulti::BufferSwapperMulti(std::vector<std::unique_ptr<Buffer>>&& buffer_list)
    : buffers{std::move(buffer_list)}
{
    for (auto& buffer : buffers)
    {
        client_queue.push_back(buffer.get());
    }
}

mc::BufferSwapperMulti::BufferSwapperMulti(std::unique_ptr<Buffer> buf_a,
                                           std::unique_ptr<Buffer> buf_b)
{
    buffers.push_back(std::move(buf_a));
    buffers.push_back(std::move(buf_b));

    for (auto& buffer : buffers)
    {
        client_queue.push_back(buffer.get());
    }
}

mc::BufferSwapperMulti::BufferSwapperMulti(std::unique_ptr<Buffer> buf_a,
                                           std::unique_ptr<Buffer> buf_b,
                                           std::unique_ptr<Buffer> buf_c)
{
    buffers.push_back(std::move(buf_a));
    buffers.push_back(std::move(buf_b));
    buffers.push_back(std::move(buf_c));

    for (auto& buffer : buffers)
    {
        client_queue.push_back(buffer.get());
    }
}

mc::Buffer* mc::BufferSwapperMulti::client_acquire()
{
    std::unique_lock<std::mutex> lk(swapper_mutex);

    while (client_queue.empty())
    {
        client_available_cv.wait(lk);
    }

    Buffer* dequeued_buffer = client_queue.front();
    client_queue.pop_front();
    return dequeued_buffer;
}

void mc::BufferSwapperMulti::client_release(mc::Buffer* queued_buffer)
{
    std::unique_lock<std::mutex> lk(swapper_mutex);

    compositor_queue.push_back(queued_buffer);
}

mc::Buffer* mc::BufferSwapperMulti::compositor_acquire()
{
    std::unique_lock<std::mutex> lk(swapper_mutex);
    Buffer* dequeued_buffer{nullptr};

    if (compositor_queue.empty())
    {
        dequeued_buffer = client_queue.back();
        client_queue.pop_back();
    }
    else
    {
        dequeued_buffer = compositor_queue.front();
        compositor_queue.pop_front();
    }

    return dequeued_buffer;
}

void mc::BufferSwapperMulti::compositor_release(mc::Buffer *released_buffer)
{
    std::unique_lock<std::mutex> lk(swapper_mutex);
    client_queue.push_back(released_buffer);
    client_available_cv.notify_one();
}

void mc::BufferSwapperMulti::shutdown()
{
    std::unique_lock<std::mutex> lk(swapper_mutex);

    if (client_queue.empty())
    {
        Buffer* dequeued_buffer = compositor_queue.front();
        compositor_queue.pop_front();
        client_queue.push_back(dequeued_buffer);
    }

    client_available_cv.notify_all();
}
