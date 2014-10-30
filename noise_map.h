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
{
    struct Key
    {
        Key(uint w, uint h, uint seed): w(w), h(h), seed(seed) {}
        uint w, h, seed;

        bool operator==(const Key &o) const
        {
            return ((seed == o.seed) && (w == o.w) && (h == o.h));
        }

        bool operator<(const Key &o) const
        {
            if (seed < o.seed)
                return true;
            if (seed > o.seed)
                return false;
            if (w < o.w)
                return true;
            if (w > o.w)
                return false;
            if (h < o.h)
                return true;
            return false;
        }
    };

    typedef std::map<Key, uint>  noise_map;
    typedef std::map<const QGLContext *, noise_map> context_to_textures;
    enum { MAX_TEXTURES = 20 };

public:
    NoiseMap(uint w, uint h, uint seed);
    ~NoiseMap();

    // Draw noise map
    virtual void Draw();

private:
    uint generateNoiseMap();
    void loadNoiseMap();
    void checkGLContext();

private:
    uint w, h, seed;

    // Textures cache
    static context_to_textures texture_maps;
#   define textures texture_maps[QGLContext::currentContext()]

};

#endif // NOISE_MAP_H
