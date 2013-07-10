// ****************************************************************************
//  noise_map.cpp                                                   Tao project
// ****************************************************************************
//
//   File Description:
//
//   noise map implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <string.h>
#include <math.h>
#include "noise_map.h"
#include "3rdparty/fbm.h"

NoiseMap::context_to_textures NoiseMap::texture_maps;

#define GL (*graphic_state)

NoiseMap::NoiseMap(uint w, uint h, uint seed) : w(w), h(h), seed(seed)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
{
    IFTRACE(mapping)
            debug() << "Create noise map" << "\n";

    loadNoiseMap();
}


NoiseMap::~NoiseMap()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


uint NoiseMap::generateNoiseMap()
// ----------------------------------------------------------------------------
//   Generate noise texture 3D
// ----------------------------------------------------------------------------
{
    IFTRACE(mapping)
            debug() << "Generate sphere mapping shader" << "\n";

    uint texId = 0;
    GL.GenTextures(1, &texId);
    GL.BindTexture(GL_TEXTURE_3D, texId);

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

    return texId;
}


void NoiseMap::loadNoiseMap()
// ----------------------------------------------------------------------------
//   Load noise map in the application
// ----------------------------------------------------------------------------
{
    checkGLContext();
    Key key(w, h, seed);
    noise_map::iterator found = textures.find(key);

    if(found == textures.end())
    {
        // Prune the map if it gets too big
        while (textures.size() > MAX_TEXTURES)
        {
            noise_map::iterator first = textures.begin();
            GL.DeleteTextures(1, &(*first).second);
            textures.erase(first);
        }

        textures[key] = generateNoiseMap();
    }

    IFTRACE(mapping)
            debug() << "Apply noise map" << "\n";

    // Set to the textures list in Tao.
    //TextureMapping::tao->BindTexture(textures[key], GL_TEXTURE_3D);
}


void NoiseMap::Draw()
// ----------------------------------------------------------------------------
//   Draw noise map texture
// ----------------------------------------------------------------------------
{
    checkGLContext();

    // Enable pixel blur
    GL.HasPixelBlur(true);

    Key key(w, h, seed);
    GL.Enable(GL_TEXTURE_3D);
    GL.BindTexture(GL_TEXTURE_3D, textures[key]);

    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    GL.TexParameter(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
}


void NoiseMap::checkGLContext()
// ----------------------------------------------------------------------------
//   Make sure a texture_map has been allocated for the current GL context
// ----------------------------------------------------------------------------
{
    if (!texture_maps.count(QGLContext::currentContext()))
    {
        noise_map m;
        texture_maps[QGLContext::currentContext()] = m;
    }
}

