#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H
// ****************************************************************************
//  displacement.h                                                  Tao project
// ****************************************************************************
//
//   File Description:
//
//      Draw a lens flare at a specific location and heading toward a
//      defined target.
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
