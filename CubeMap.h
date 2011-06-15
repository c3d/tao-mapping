#ifndef CUBEMAP_H
#define CUBEMAP_H
// ****************************************************************************
//  plane.h                                                  Tao project
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
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <stdio.h>

#include "texture_mapping.h"

struct CubeMap : public TextureMapping
// ----------------------------------------------------------------------------
//   Define a subdivided plane
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    CubeMap();
    ~CubeMap();

    // Draw plane
    virtual void Draw();

    void texParameters();
    bool setTexture(const char* filename, GLuint face);
    bool loadCubeMap();

private:
    bool loadTexture(const char* filename, GLenum target);

private:
    GLuint  cubeMapId;
    std::vector<const char*> textures;
};

#endif // CUBEMAP_H
