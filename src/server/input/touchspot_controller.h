/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Robert Carr <robert.carr@canonical.com>
 */

#ifndef MIR_INPUT_TOUCHSPOT_CONTROLLER_H_
#define MIR_INPUT_TOUCHSPOT_CONTROLLER_H_

#include "mir/input/touch_visualizer.h"

#include <memory>
#include <mutex>

namespace mir
{
namespace graphics
{
class GraphicBufferAllocator;
class Buffer;
class BufferWriter;
class Renderable;
}
namespace input
{
class Scene;
class TouchspotRenderable;

/// Receives touchspot events out of the input stack and manages appearance
/// of touchspot renderables for visualization.
class TouchspotController : public TouchVisualizer
{
public:
    TouchspotController(std::shared_ptr<graphics::GraphicBufferAllocator> const& allocator,
        std::shared_ptr<graphics::BufferWriter> const& writer,
        std::shared_ptr<input::Scene> const& scene);

    virtual ~TouchspotController() = default;
    
    void visualize_touches(std::vector<Spot> const& touches);

protected:
    TouchspotController(TouchspotController const&) = delete;
    TouchspotController& operator=(TouchspotController const&) = delete;

private:
    std::shared_ptr<graphics::Buffer> touchspot_buffer;
    std::shared_ptr<Scene> scene;
    
    std::mutex guard;

    unsigned int renderables_in_use;
    std::vector<std::shared_ptr<TouchspotRenderable>> touchspot_renderables;
};

}
}

#endif // MIR_INPUT_TOUCHSPOT_CONTROLLER_H_
