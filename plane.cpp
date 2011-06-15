// ****************************************************************************
//  Plane.cpp                                                       Tao project
// ****************************************************************************
//
//   File Description:
//
//   Plane implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Plane.h"

// ============================================================================
//
//    Plane
//
// ============================================================================

Plane::Plane(float x, float y, float w, float h, int lines, int columns)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : center(x, y, 0), width(w), height(h), lines(lines), columns(columns)
{
    initialize();
}

Plane::~Plane()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Plane::initialize()
// ----------------------------------------------------------------------------
//   Initialize plane parameters
// ----------------------------------------------------------------------------
{
    // Subdivision steps
    float stepX = width / lines;
    float stepY = height / columns;

    // Define the start of the plan
    Vector3 start = Vector3(center.x - width/2, center.y - height/2, 0);

    // Compute vertices and textures coordinates
    for(int j = 0; j <= columns; j++)
    {
        for(int i = 0; i <= lines; i++)
        {
            vertices.push_back(start + Vector3(stepX * i, stepY * j, 0));
            textures.push_back(Vector((double) i / lines, (double) j / columns));
        }
    }

    // Compute indexes
    for(int j = 0; j < columns; j++)
    {
        for(int i = 0; i < lines; i++)
        {
            indices.push_back(j * (lines + 1) + i);
            indices.push_back(j * (lines + 1) + i + 1);
            indices.push_back((j + 1) * (lines + 1) + i + 1);
            indices.push_back((j + 1) * (lines + 1) + i);
        }
    }


    // Set texture Coordinates to all units
    TextureMapping::tao->SetTexCoords(-1, &textures[0].x);
}

void Plane::Draw()
// ----------------------------------------------------------------------------
//   Draw a subdivided plane
// ----------------------------------------------------------------------------
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glNormal3f(0., 0., 1.);

    // Set vertex coordinates
    glVertexPointer(3, GL_DOUBLE, 0, &vertices[0].x);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Set fill color defined in Tao
    if(TextureMapping::tao->SetFillColor());
        glDrawElements(GL_QUADS, columns * lines * 4 , GL_UNSIGNED_INT, &indices[0]);

    // Set fill color defined in Tao
    if(TextureMapping::tao->SetLineColor());
        glDrawElements(GL_LINES, columns * lines * 4 , GL_UNSIGNED_INT, &indices[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
}


