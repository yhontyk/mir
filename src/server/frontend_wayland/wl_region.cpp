/*
 * Copyright © 2018 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3 as
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
 * Authored by:
 *   William Wold <william.wold@canonical.com>
 */

#include "wl_region.h"

namespace mf = mir::frontend;

mf::WlRegion::WlRegion(struct wl_client* client, struct wl_resource* parent, uint32_t id)
    : wayland::Region(client, parent, id)
{}

mf::WlRegion::~WlRegion()
{}

void mf::WlRegion::destroy()
{
    wl_resource_destroy(resource);
}

void mf::WlRegion::add(int32_t x, int32_t y, int32_t width, int32_t height)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}

void mf::WlRegion::subtract(int32_t x, int32_t y, int32_t width, int32_t height)
{
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}
