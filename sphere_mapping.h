#ifndef SPHERE_MAPPING_H
#define SPHERE_MAPPING_H
// ****************************************************************************
//  sphere_mapping.h                                                Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define sphere mapping.
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#include "tao/coords3d.h"
#include "tao/matrix.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "texture_mapping.h"
#include <QObject>

using namespace std;
using namespace Tao;


struct SphereMapping : public TextureMapping
{
    SphereMapping(float ratio);
    ~SphereMapping();

    // Draw sphere mapping
    virtual void    Draw();

protected:
    virtual void    createShaders();

private:
    float   ratio;
    GLfloat model[4][4]; // model matrix

    static bool failed;
    static QGLShaderProgram* pgm;
    static std::map<text, GLint> uniforms;
    static const QGLContext* context;
};


#endif
