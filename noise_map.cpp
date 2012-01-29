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


NoiseMap::NoiseMap(uint w, uint h, uint seed) : w(w), h(h), seed(seed)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
{
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
    uint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_3D, texId);

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

    glTexImage3D(GL_TEXTURE_3D, 0, 4, w, h, seed, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glBindTexture(GL_TEXTURE_3D, 0);
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
            glDeleteTextures(1, &(*first).second);
            textures.erase(first);
        }

        textures[key] = generateNoiseMap();
    }

    if (!tested)
    {
        if(tao->hasLicense("Mapping 1.0") || tao->hasLicense("Materials 1.0"))
            licensed = true;
        else
            licensed = tao->checkLicense("Mapping 1.0", false);
        tested = true;
    }
    if (!licensed && !tao->blink(1.0, 1.0, 300.0))
        return;

    // Set to the textures list in Tao.
    TextureMapping::tao->BindTexture(textures[key], GL_TEXTURE_3D);
}

void NoiseMap::Draw()
// ----------------------------------------------------------------------------
//   Draw noise map texture
// ----------------------------------------------------------------------------
{
    if (!licensed && !tao->blink(1.0, 1.0, 300.0))
        return;

    // Enable pixel blur
    TextureMapping::tao->HasPixelBlur(true);
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

