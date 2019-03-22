#ifndef CUBEMAP_H
#define CUBEMAP_H
// *****************************************************************************
// cube_map.h                                                      Tao3D project
// *****************************************************************************
//
// File description:
//
//      Define a cube map.
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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

#include <stdio.h>
#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "texture_mapping.h"
#include <map>

struct  TextureFace
// ----------------------------------------------------------------------------
//   Define a face of a cube map
// ----------------------------------------------------------------------------
{
    bool flip_u; // flip u-coordinates of our face
    bool flip_v; // flip v-coordinates of our face
    text name;   // name of our face

    TextureFace() : flip_u(false), flip_v(true) {}

    TextureFace & operator = (const TextureFace &o)
    {
        name   = o.name;
        flip_u = o.flip_u;
        flip_v = o.flip_v;

        return *this;
    }

    bool operator == (const TextureFace&o) const
    {
        if(o.name.compare(name))
           return false;
        if(o.flip_u != flip_u)
            return false;
        if(o.flip_v != flip_v)
            return false;
        return true;
    }

    bool operator != (const TextureFace&o) const
    {
        return ! operator ==(o);
    }

    void loadTexture(uint face, QImage &image);
};


struct  TextureCube
// ----------------------------------------------------------------------------
//   Define a cubemap texture
// ----------------------------------------------------------------------------
{
    int size; //size of texture cube
    //name of cube faces
    TextureFace left, right, bottom, top, front, back;

    TextureCube & operator = (const TextureCube &o)
    {
        size   = o.size;
        left   = o.left;
        right  = o.right;
        bottom = o.bottom;
        top    = o.top;
        front  = o.front;
        back   = o.back;

        return *this;
    }

    bool operator == (const TextureCube&o) const
    {
        if(o.size != size)
            return false;
        if(o.left != left)
           return false;
        if(o.right != right)
           return false;
        if(o.bottom != bottom)
           return false;
        if(o.top != top)
           return false;
        if(o.front != front )
           return false;
        if(o.back != back)
           return false;

        return true;
    }

    bool operator != (const TextureCube&o) const
    {
        return ! operator ==(o);
    }
};


struct CubeMap : TextureMapping
// ----------------------------------------------------------------------------
//   Apply a cubemap texture
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    CubeMap(int size = 0);
    CubeMap(text cross);
    ~CubeMap();

    // Draw cubemap
    virtual void    Draw();

    void            flip(bool u, bool v);
    bool            setTexture(text filename, uint face);
    bool            loadCubeMap();
    bool            loadCrossTexture(text cross);

protected:
    virtual void    createShaders();

private:
    TextureFace*    whichFace(uint face);
    bool            loadTexture(uint face);
    bool            loadTexture(uint face, QImage &cross);

private:
    uint            cubeMapId;
    GLuint          cubeMapUID, lightsUID;
    bool            flip_u : 1;
    bool            flip_v : 1;
    TextureCube     textureCube;
};

#endif
