// ****************************************************************************
//  plane.cpp                                                       Tao project
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
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "plane.h"


PlaneMesh::PlaneMesh(int lines, int columns)
// ----------------------------------------------------------------------------
//   Initialize plane parameters
// ----------------------------------------------------------------------------
{
    // Subdivision steps
    float stepX = 1.0 / lines;
    float stepY = 1.0 / columns;

    // Compute vertices and textures coordinates
    for(int j = 0; j <= columns; j++)
    {
        for(int i = 0; i <= lines; i++)
        {
            vertices.push_back(Vector3(stepX * i - 0.5, stepY * j - 0.5, 0));
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
}

// ============================================================================
//
//    Plane
//
// ============================================================================

Plane::PlaneCache Plane::cache;


Plane::Plane(float x, float y, float w, float h, int slices, int stacks)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : center(x, y, 0), width(w), height(h), slices(slices), stacks(stacks)
{}

Plane::~Plane()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Plane::Draw()
{
    PlaneMesh * plane = NULL;
    Key key(slices, stacks);
    PlaneCache::iterator found = cache.find(key);
    if (found == cache.end())
    {
        // Prune the map if it gets too big
        while (cache.size() > MAX_PLANES)
        {
            PlaneCache::iterator first = cache.begin();
            delete (*first).second;
            cache.erase(first);
        }
        plane = new PlaneMesh(slices, stacks);
        cache[key] = plane;
    }
    else
    {
        plane = (*found).second;
    }

    Draw(plane);
}

void Plane::Draw(PlaneMesh* plane)
// ----------------------------------------------------------------------------
//   Draw a subdivided plane
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = tao->checkImpressOrLicense(MAPPING_FEATURE);
        tested = true;
    }
    if (!licensed && !tao->blink(1.0, 1.0, 300.0))
        return;

    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_NORMALIZE);
    glTranslatef(center.x, center.y, 0.0);
    glScalef(width, height, 1.0);
    glNormal3f(0., 0., 1.);

    // Set vertex coordinates
    glVertexPointer(3, GL_DOUBLE, 0, &plane->vertices[0].x);
    glEnableClientState(GL_VERTEX_ARRAY);

    TextureMapping::tao->EnableTexCoords(&plane->textures[0].x);
    TextureMapping::tao->SetTextures();

    GLuint size = stacks * slices * 4;

    // Set fill color defined in Tao
    if(TextureMapping::tao->SetFillColor())
        glDrawElements(GL_QUADS, size, GL_UNSIGNED_INT, &plane->indices[0]);

    // Set line color defined in Tao
    if(TextureMapping::tao->SetLineColor())
        for(GLuint i = 0; i < size; i+= 4)
            glDrawElements(GL_LINE_LOOP, 4 , GL_UNSIGNED_INT, &plane->indices[0] + i);

    TextureMapping::tao->DisableTexCoords();

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopAttrib();
    glPopMatrix();
}


