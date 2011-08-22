#ifndef CUBEMAP_H
#define CUBEMAP_H
// ****************************************************************************
//  cube_map.h                                                      Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a cube map.
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

#include <stdio.h>
#include "texture_mapping.h"

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

struct CubeMap : public TextureMapping
// ----------------------------------------------------------------------------
//   Apply a cubemap texture
// ----------------------------------------------------------------------------
{
    typedef std::map<uint, TextureCube> texture_map;
    typedef std::map<const QGLContext *, texture_map> context_to_textures;
    enum { MAX_TEXTURES = 20 };

    // Constructor and destructor
    CubeMap(int size = 0);
    ~CubeMap();

    // Draw cubemap
    virtual void Draw();

    void flip(bool u, bool v);
    bool setTexture(text filename, uint face);
    bool loadCubeMap();

private:
    uint         isInclude();
    TextureFace* whichFace(uint face);
    bool         loadTexture(uint face);
    void         checkGLContext();

private:
    int                size;
    bool               flip_u : 1;
    bool               flip_v : 1;
    TextureCube        currentTexture;

    // Textures cache
    static context_to_textures texture_maps;
#   define textures texture_maps[QGLContext::currentContext()]

};

#endif // CUBEMAP_H
