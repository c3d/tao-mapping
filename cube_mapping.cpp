// *****************************************************************************
// cube_mapping.cpp                                                Tao3D project
// *****************************************************************************
//
// File description:
//
//   Cube mapping implementation.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2012-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

#include "cube_mapping.h"

// ============================================================================
//
//   Cube Mapping
//
// ============================================================================

CubeMapping::CubeMapping(float ratio)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : TextureMapping(), ratio(ratio),
      ratioUID(0), lightsUID(0), colorMapUID(0), cubeMapUID(0),
      hasColorMapUID(0), cameraUID(0), modelMatrixUID(0)
{
    IFTRACE(mapping)
        debug() << "Create cube mapping\n";

    checkGLContext();

    // Get model matrix
    Matrix4 m = tao->ModelMatrix();
    std::copy(m.Data(), m.Data() + 16, model[0]);
}


CubeMapping::~CubeMapping()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{}


void CubeMapping::Draw()
// ----------------------------------------------------------------------------
//   Apply cube mapping
// ----------------------------------------------------------------------------
{
    checkGLContext();

    uint prg_id = 0;
    if(program)
    {
        prg_id = program->programId();
        IFTRACE(mapping)
            debug() << "Apply cube mapping" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Check if unit 0 has a texture or not
        bool hasTexture = GL.ActiveTextureUnits() & 1;

        // Set uniform values
        GL.Uniform(colorMapUID, 0);
        GL.Uniform(cubeMapUID, 1);
        GL.Uniform(hasColorMapUID, hasTexture);
        GL.Uniform(ratioUID, ratio);
        GL.UniformMatrix4fv(modelMatrixUID, 1, 0, &model[0][0]);

        // Get and set camera position
        Vector3 cam;
        tao->getCamera(&cam, NULL, NULL, NULL);
        GLfloat camera[3] = {(float) cam.x, (float) cam.y, (float) cam.z};
        GL.Uniform3fv(cameraUID, 1, camera);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask =  GL.LightsMask();
            GL.Uniform(lightsUID, lightsmask);
        }
    }
}


void CubeMapping::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    IFTRACE(mapping)
        debug() << "Create cube mapping shader" << "\n";

    delete program;

    program = new QGLShaderProgram(context);
    bool ok = false;

    // Basic vertex shader
    static string vSrc =
        "/********************************************************************************\n"
        "**                                                                               \n"
        "** Copyright (C) 2011 Taodyne.                                                   \n"
        "** All rights reserved.                                                          \n"
        "** Contact: Taodyne (contact@taodyne.com)                                        \n"
        "**                                                                               \n"
        "** This file is part of the Tao3D application, developped by Taodyne.\n"
        "** It can be only used in the software and these modules.                        \n"
        "**                                                                               \n"
        "** If you have questions regarding the use of this file, please contact          \n"
        "** Taodyne at contact@taodyne.com.                                               \n"
        "**                                                                               \n"
        "********************************************************************************/\n"
        "uniform mat4 modelMatrix;"
        "uniform vec3 camera;"

        "varying vec3 viewDir;"
        "varying vec3 normal;"
        "varying vec4 color;"

        "void main()"
        "{"
        "   mat3 normalMatrix;"

        "   /* First column */"
        "   normalMatrix[0][0] = modelMatrix[0][0];"
        "   normalMatrix[0][1] = modelMatrix[0][1];"
        "   normalMatrix[0][2] = modelMatrix[0][2];"

        "   /* Second column */"
        "   normalMatrix[1][0] = modelMatrix[1][0];"
        "   normalMatrix[1][1] = modelMatrix[1][1];"
        "   normalMatrix[1][2] = modelMatrix[1][2];"

        "   /* Third column */"
        "   normalMatrix[2][0] = modelMatrix[2][0];"
        "   normalMatrix[2][1] = modelMatrix[2][1];"
        "   normalMatrix[2][2] = modelMatrix[2][2];"

        "   /* Compute world position and normal */"
        "   vec4 worldPos = modelMatrix * gl_Vertex;"
        "   viewDir  = normalize(worldPos.xyz - camera);"
        "   normal   = normalize(normalMatrix * gl_Normal);"

        "   /* Compute reflection vector */"
        "   vec3 r = reflect (viewDir, normal);"

        "   /* Compute texture coordinates */"
        "   gl_TexCoord[1] = gl_TextureMatrix[1] * vec4 (r, 1.0);"
        "   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"

        "   /* Compute position */"
        "   gl_Position = ftransform();"

        "   /* Compute new values for lighting */"
        "   normal   =  normalize(gl_NormalMatrix * gl_Normal);"
        "   viewDir  = -normalize((gl_ModelViewMatrix * gl_Vertex).xyz);"

        "   color = gl_Color;"
        "}";

    static string fSrc;
    if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
    {
        // If the extension is available, use this shader
        // to handle multiple lights
        fSrc =
            "/********************************************************************************\n"
            "**                                                                               \n"
            "** Copyright (C) 2011 Taodyne.                                                   \n"
            "** All rights reserved.                                                          \n"
            "** Contact: Taodyne (contact@taodyne.com)                                        \n"
            "**                                                                               \n"
            "** This file is part of the Tao3D application, developped by Taodyne.\n"
            "** It can be only used in the software and these modules.                        \n"
            "**                                                                               \n"
            "** If you have questions regarding the use of this file, please contact          \n"
            "** Taodyne at contact@taodyne.com.                                               \n"
            "**                                                                               \n"
            "********************************************************************************/\n"
            "#extension GL_EXT_gpu_shader4 : require\n"

            "/* Mapping parameters */"
            "uniform int         lights;"
            "uniform bool        hasColorMap;"
            "uniform float       ratio;"
            "uniform sampler2D   colorMap;"
            "uniform samplerCube cubeMap;"

            "varying vec3 viewDir;"
            "varying vec3 normal;"
            "varying vec4 color;"

            "/**"
            "* Compute render color according to materials,"
            "* lights and colors parameters which are"
            "* set in the current scene."
            "**/"
            "vec4 computeRenderColor(vec4 renderColor)"
            "{"
            "    vec4 lighting_color = renderColor;"
            "    if(lights > 0)"
            "    {"
            "        /* Define a maximum of lights supported */"
            "        int MAX_LIGHTS = 8;"

            "        vec3 N = normalize(normal);"
            "        vec3 V = normalize(viewDir);"

            "        vec4 ambient  = vec4(0.0);"
            "        vec4 diffuse  = vec4(0.0);"
            "        vec4 specular = vec4(0.0);"
            "        ambient = gl_FrontLightModelProduct.sceneColor"
            "                * gl_FrontMaterial.ambient;"

            "        for(int i = 0; i < MAX_LIGHTS; i++)"
            "        {"
            "            if(bool(lights & (1 << i)))"
            "            {"
            "                vec3 L = normalize(gl_LightSource[i].position.xyz);"

            "                /* Compute ambient part */"
            "                ambient += gl_LightSource[i].ambient * gl_FrontMaterial.ambient;"

            "                /* Diffuse coefficient */"
            "                float nDotL = max(clamp(dot(L, N), 0.0, 1.0), 0.0);"
            "                if (nDotL > 0.0)"
            "                {"
            "                    /* Compute diffuse part */"
            "                    diffuse += gl_LightSource[i].diffuse"
            "                             * gl_FrontMaterial.diffuse"
            "                             * nDotL;"

            "                    /* Compute specular coefficient */"
            "                    float nDotV = clamp(dot(reflect(-L, N), V), 0.0, 1.0);"
            "                    if (nDotV > 0.0)"
            "                    {"
            "                        /* Compute specular part */"
            "                        specular += gl_LightSource[i].specular"
            "                                  * gl_FrontMaterial.specular"
            "                                  * pow(nDotV, gl_FrontMaterial.shininess);"
            "                    }"
            "                }"
            "            }"
            "        }"

            "         /* Define new render color */"
            "        lighting_color  = (ambient + diffuse) * renderColor + specular;"
            "    }"
            "    else"
            "    {"
            "        lighting_color = renderColor * color;"
            "    }"

            "    return lighting_color;"
            "}"


            "void main()"
            "{"
            "   vec4 cubeColor, mainColor, renderColor;"

            "   /* Get sphere map */"
            "   cubeColor = textureCube(cubeMap, gl_TexCoord[1].xyz);"

            "   /* Get color map */"
            "   mainColor = vec4(1.0);"

            "   /* Check if there is really a color map */"
            "   if(hasColorMap)"
            "       mainColor = texture2D(colorMap, gl_TexCoord[0].st);"

            "   renderColor  = mix(mainColor, cubeColor, ratio);"
            "   gl_FragColor = computeRenderColor(renderColor);"
            "}";

    }
    else
    {
        // If the extension is not available, use this shader
        // to handle an unique light.
        fSrc =
            /********************************************************************************
             **
             ** Copyright (C) 2011 Taodyne.
             ** All rights reserved.
             ** Contact: Taodyne (contact@taodyne.com)
             **
             ** This file is part of the Tao3D application, developped by Taodyne.
             ** It can be only used in the software and these modules.
             **
             ** If you have questions regarding the use of this file, please contact
             ** Taodyne at contact@taodyne.com.
             **
             ********************************************************************************/
            "/* Mapping parameters */"
            "uniform bool        hasColorMap;"
            "uniform float       ratio;"
            "uniform sampler2D   colorMap;"
            "uniform samplerCube cubeMap;"

            "varying vec4 color;"
            "void main()"
            "{"
            "    vec4 cubeColor, mainColor, renderColor;"

            "    /* Get cube map */"
            "    cubeColor = textureCube(cubeMap, gl_TexCoord[1].xyz);"

            "    /* Get color map */"
            "    mainColor = texture2D(colorMap, gl_TexCoord[0].st);"

            "    /* Check if there is really a color map */"
            "    if(hasColorMap)"
            "       mainColor *= color;"
            "    else"
            "       mainColor  = color;"

            "    renderColor  = mix(mainColor, cubeColor, ratio);"
            "    gl_FragColor = renderColor;"
            "}";
    }

    if (program->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
    {
        if (program->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
        {
            ok = true;
        }
        else
        {
            std::cerr << "Error loading fragment shader code:\n";
            std::cerr << program->log().toStdString();
        }
    }
    else
    {
        std::cerr << "Error loading vertex shader code:\n";
        std::cerr << program->log().toStdString();
    }
    if (!ok)
    {
        delete program;
        program = NULL;
    }
    else
    {
        program->link();

        // Save uniform locations
        uint id = program->programId();
        ratioUID = GL.GetUniformLocation(id, "ratio");
        lightsUID = GL.GetUniformLocation(id, "lights");
        colorMapUID = GL.GetUniformLocation(id, "colorMap");
        cubeMapUID = GL.GetUniformLocation(id, "cubeMap");
        hasColorMapUID = GL.GetUniformLocation(id, "hasColorMap");
        cameraUID = GL.GetUniformLocation(id, "camera");
        modelMatrixUID = GL.GetUniformLocation(id, "modelMatrix");
    }
}
