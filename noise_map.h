#ifndef NOISE_MAP_H
#define NOISE_MAP_H
// ****************************************************************************
//  noise_map.h                                                    Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a noise map.
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************

#include "texture_mapping.h"

struct NoiseMap : public TextureMapping
// ----------------------------------------------------------------------------
//    Create a noise map
// ----------------------------------------------------------------------------
{
    NoiseMap(uint w, uint h, uint seed);
    ~NoiseMap();

    // Draw noise map
    virtual void Draw();

private:
    uint generateNoiseMap();
    void loadNoiseMap();

private:
    uint w, h, seed, textureID;
};

#endif // NOISE_MAP_H
