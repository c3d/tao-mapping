#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H
// *****************************************************************************
// displacement.h                                                  Tao3D project
// *****************************************************************************
//
// File description:
//
//      Draw a lens flare at a specific location and heading toward a
//      defined target.
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011, Soulisse Baptiste <soulisse@polytech.unice.fr>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************


#include "texture_mapping.h"

struct Displacement : public TextureMapping
// ----------------------------------------------------------------------------
//   Define a complete lens flare
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Displacement(float x, float y, float z, float w, float h, int lines, int columns);
    ~Displacement();

    // Draw lens flare
    virtual void Draw();

private:
    Tao::Vector3 center;
    float width, height;
    int   lines, columns;

    //Plane parameters
    Tao::Vector3 *vertices, normals;
    Tao::Vector  *textures;
    GLuint  *indices;
};

#endif
