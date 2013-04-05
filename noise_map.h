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
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
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
            return (((seed < o.seed) || (w < o.w)) || (h < o.h));
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
