// *****************************************************************************
// noise_map.cpp                                                   Tao3D project
// *****************************************************************************
//
// File description:
//
//   noise map implementation.
//
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

#include <string.h>
#include <math.h>
#include "noise_map.h"
#include "3rdparty/fbm.h"


NoiseMap::NoiseMap(uint w, uint h, uint seed)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : w(w), h(h), seed(seed), textureID(0)
{
    IFTRACE(mapping)
        debug() << "Create noise map\n";
    loadNoiseMap();
}


NoiseMap::~NoiseMap()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
    if (textureID)
        GL.DeleteTextures(1, &textureID);
}


uint NoiseMap::generateNoiseMap()
// ----------------------------------------------------------------------------
//   Generate noise texture 3D
// ----------------------------------------------------------------------------
{
    IFTRACE(mapping)
        debug() << "Generate sphere mapping shader\n";

    GL.GenTextures(1, &textureID);
    GL.BindTexture(GL_TEXTURE_3D, textureID);

    QRgb *data = new QRgb[w * h * seed];
    memset(data, 0, w * h * seed * sizeof(QRgb));
    QRgb *p = data;
    float pos[3];
    for (uint k = 0; k < seed; ++k) {
        pos[2] = k * (0x20 / (float)seed);
        for (uint j = 0; j < h; ++j) {
            for (uint i = 0; i < w; ++i) {
                for (uint byte = 0; byte < 4; ++byte) {
                    pos[0] = (i + (byte & 1) * 16) * (0x20 / (float)w);
                    pos[1] = (j + (byte & 2) * 8) * (0x20 / (float)h);
                    *p |= (int)(128.0f * (noise3(pos) + 1.0f)) << (byte * 8);
                }
                ++p;
            }
        }
    }

    GL.TexImage3D(GL_TEXTURE_3D, 0, 4, w, h, seed, 0,
                  GL_BGRA, GL_UNSIGNED_BYTE, data);

    delete[] data;

    // Apply some default parameters to the texture
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    return textureID;
}


void NoiseMap::loadNoiseMap()
// ----------------------------------------------------------------------------
//   Load noise map in the application
// ----------------------------------------------------------------------------
{
    checkGLContext();
    if (!textureID)
        textureID = generateNoiseMap();

    IFTRACE(mapping)
        debug() << "Apply noise map\n";

    // This binding allows to get texture id in Tao
    GL.Enable(GL_TEXTURE_CUBE_MAP);
    GL.BindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}


void NoiseMap::Draw()
// ----------------------------------------------------------------------------
//   Draw noise map texture
// ----------------------------------------------------------------------------
{
    checkGLContext();

    // Enable pixel blur
    GL.HasPixelBlur(true);

    GL.Enable(GL_TEXTURE_3D);
    GL.BindTexture(GL_TEXTURE_3D, textureID);
}
