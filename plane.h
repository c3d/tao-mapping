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

struct PlaneMesh
// ----------------------------------------------------------------------------
//   Common drawing code for mesh-based shapes
// ----------------------------------------------------------------------------
{
    PlaneMesh(int lines, int columns);

    std::vector<Point3> vertices;
    std::vector<GLuint> indices;
    std::vector<Point>  textures;
};

struct Plane : public TextureMapping
// ----------------------------------------------------------------------------
//   Define a subdivided plane
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Plane(float x, float y, float w, float h, int lines, int columns);
    ~Plane();

    // Draw plane
    virtual void Draw();
    void Draw(PlaneMesh* plane);

private:
    // Plane parameters
    Tao::Vector3 center;
    float width, height;
    int   slices, stacks;

    enum { MAX_PLANES = 10 };
    typedef std::pair<uint, uint> Key;
    typedef std::map<Key, PlaneMesh *> PlaneCache;

    static PlaneCache cache;
};

#endif
