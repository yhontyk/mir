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
#ifndef MIR_CLIENT_MIR_SURFACE_H_
#define MIR_CLIENT_MIR_SURFACE_H_

#include "mir_protobuf.pb.h"

#include "mir/geometry/pixel_format.h"
#include "mir/geometry/dimensions.h"
#include "mir_client/mir_client_library.h"
#include "mir_wait_handle.h"

#include <memory>
#include <map>

namespace mir
{
namespace client
{
class ClientBufferFactory;
class ClientBuffer;
class MemoryRegion;
}
}

class MirSurface
{
public:
    MirSurface(MirSurface const &) = delete;
    MirSurface& operator=(MirSurface const &) = delete;

    MirSurface(
        mir::protobuf::DisplayServer::Stub & server,
        const std::shared_ptr<mir::client::ClientBufferFactory>& factory, 
        MirSurfaceParameters const & params,
        mir_surface_lifecycle_callback callback, void * context);

    MirWaitHandle* release(mir_surface_lifecycle_callback callback, void * context);
    MirSurfaceParameters get_parameters() const;
    char const * get_error_message();
    int id() const;
    bool is_valid() const;
    void populate(MirBufferPackage& buffer_package);
    MirWaitHandle* next_buffer(mir_surface_lifecycle_callback callback, void * context);
    MirWaitHandle* get_create_wait_handle();

    void get_cpu_region(MirGraphicsRegion& region);
    void release_cpu_region();

private:
    void released(mir_surface_lifecycle_callback callback, void * context);
    void created(mir_surface_lifecycle_callback callback, void * context);
    void new_buffer(mir_surface_lifecycle_callback callback, void * context);

    void save_buffer_dimensions();

    mir::protobuf::DisplayServer::Stub & server;
    mir::protobuf::Void void_response;
    mir::protobuf::Surface surface;
    std::string error_message;

    MirWaitHandle create_wait_handle;
    MirWaitHandle release_wait_handle;
    MirWaitHandle next_buffer_wait_handle;

    int last_buffer_id;
    std::map<int, std::shared_ptr<mir::client::ClientBuffer>> buffer_cache;
    mir::geometry::Width surface_width;
    mir::geometry::Height surface_height;
    mir::geometry::PixelFormat surface_pf;

    std::shared_ptr<mir::client::MemoryRegion> secured_region;
    std::shared_ptr<mir::client::ClientBufferFactory> buffer_factory;
};

#endif /* MIR_CLIENT_PRIVATE_MIR_WAIT_HANDLE_H_ */
