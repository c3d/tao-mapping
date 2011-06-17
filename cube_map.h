#ifndef CUBEMAP_H
#define CUBEMAP_H
// ****************************************************************************
//  cube_map.h                                                      Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a cube Map.
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

struct  TextureCube
// ----------------------------------------------------------------------------
//   Define a cubemap texture
// ----------------------------------------------------------------------------
{
    //name of cube faces
    text left, right, bottom, top, front, back;

    TextureCube & operator = (const TextureCube &o)
    {
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
        if(o.left.compare(left))
           return false;
        if(o.right.compare(right))
           return false;
        if(o.bottom.compare(bottom))
           return false;
        if(o.top.compare(top))
           return false;
        if(o.front.compare(front))
           return false;
        if(o.back.compare(back))
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

    // Constructor and destructor
    CubeMap();
    ~CubeMap();

    // Draw cubemap
    virtual void Draw();

    void texParameters();
    bool setTexture(text filename, uint face);
    bool loadCubeMap();

private:
    uint isInclude();
    text* whichFace(uint face);
    bool loadTexture(uint face);

private:
    TextureCube        currentTexture;
    static texture_map textures;
};

#endif // CUBEMAP_H
