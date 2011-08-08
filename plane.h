#ifndef PLANE_H
#define PLANE_H
// ****************************************************************************
// plane.h                                                          Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a subdivided plane.
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
#include <vector>

struct Plane : public TextureMapping
// ----------------------------------------------------------------------------
//   Define a subdivided plane
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Plane(float x, float y, float w, float h, int lines, int columns);
    ~Plane();

    void initialize();

    // Draw plane
    virtual void Draw();

private:
    // Plane parameters
    Tao::Vector3 center;
    float width, height;
    int   lines, columns;

    //Plane buffers
    std::vector<Vector3> vertices;
    std::vector<Vector>  textures;
    std::vector<GLuint>  indices;
};

#endif
