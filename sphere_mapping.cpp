// ****************************************************************************
//  sphere_mapping.cpp                                              Tao project
// ****************************************************************************
//
//   File Description:
//
//   Sphere mapping implementation.
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
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "sphere_mapping.h"

// ============================================================================
//
//   Cube Mapping
//
// ============================================================================

bool                  SphereMapping::failed = false;
QGLShaderProgram*     SphereMapping::pgm = NULL;
std::map<text, GLint> SphereMapping::uniforms;
const QGLContext*     SphereMapping::context = NULL;

SphereMapping::SphereMapping(float ratio)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : TextureMapping(&context), ratio(ratio)
{
    IFTRACE(mapping)
            debug() << "Create sphere mapping" << "\n";

    checkGLContext();

    // Get model matrix
    Matrix4 m = tao->ModelMatrix();
    std::copy(m.Data(), m.Data() + 16, model[0]);
}


SphereMapping::~SphereMapping()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}


void SphereMapping::Draw()
// ----------------------------------------------------------------------------
//   Apply sphere mapping
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = tao->checkImpressOrLicense(MAPPING_FEATURE);
        tested = true;
    }

    if (!licensed && !tao->blink(1.0, 1.0, 300.0))
        return;

    checkGLContext();

    uint prg_id = 0;
    if(pgm)
        prg_id = pgm->programId();

    if(prg_id)
    {
        IFTRACE(mapping)
                debug() << "Apply sphere mapping" << "\n";

        // Set shader
        tao->SetShader(prg_id);

        // Set uniform values
        glUniform1i(uniforms["colorMap"], 0);
        glUniform1i(uniforms["sphereMap"], 1);
        glUniform1i(uniforms["hasColorMap"], tao->HasTexture(0));
        glUniform1f(uniforms["ratio"], ratio);
        glUniformMatrix4fv(uniforms["modelMatrix"], 1, 0, &model[0][0]);

        // Get and set camera position
        Vector3 cam;
        tao->getCamera(&cam, NULL, NULL, NULL);
        GLfloat camera[3] = {cam.x, cam.y, cam.z};
        glUniform3fv(uniforms["camera"], 1, camera);

        if(tao->isGLExtensionAvailable("GL_EXT_gpu_shader4"))
        {
            GLint lightsmask = tao->EnabledLights();
            glUniform1i(uniforms["lights"], lightsmask);
        }
    }
}


void SphereMapping::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        IFTRACE(mapping)
                debug() << "Create sphere mapping shader" << "\n";

        delete pgm;

        pgm = new QGLShaderProgram(*pcontext);
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
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
                "   float m  = 2.0 * sqrt( r.x * r.x +"
                "                          r.y * r.y +"
                "                         (r.z + 1.0) * (r.z + 1.0));"

                "   /* Compute texture coordinates */"
                "   gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
                "   gl_TexCoord[1].s = r.x/m + 0.5;"
                "   gl_TexCoord[1].t = r.y/m + 0.5;"

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
                    "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
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
                    "uniform sampler2D   sphereMap;"

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

                    "    return lighting_color;"
                    "}"


                    "void main()"
                    "{"
                    "   vec4 sphereColor, mainColor, renderColor;"

                    "   /* Get sphere map */"
                    "   sphereColor = texture2D(sphereMap, gl_TexCoord[1].st);"

                    "   /* Get color map */"
                    "   mainColor = texture2D(colorMap, gl_TexCoord[0].st);"

                    "   /* Check if there is really a color map */"
                    "   if(hasColorMap)"
                    "      mainColor *= color;"
                    "   else"
                    "      mainColor  = color;"

                    "   renderColor  = mix(mainColor, sphereColor, ratio);"
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
                    ** This file is part of the Tao Presentations application, developped by Taodyne.
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
                    "   vec4 sphereColor, mainColor, renderColor;"

                    "   /* Get sphere map */"
                    "   sphereColor = texture2D(sphereMap, gl_TexCoord[1].st);"

                    "   /* Get color map */"
                    "   mainColor = texture2D(colorMap, gl_TexCoord[0].st);"

                    "   /* Check if there is really a color map */"
                    "   if(hasColorMap)"
                    "      mainColor *= color;"
                    "   else"
                    "      mainColor  = color;"

                    "   renderColor  = mix(mainColor, sphereColor, ratio);"
                    "   gl_FragColor = renderColor;"
                    "}";
        }

        if (pgm->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
        {
            if (pgm->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Error loading fragment shader code: " << "\n";
                std::cerr << pgm->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Error loading vertex shader code: " << "\n";
            std::cerr << pgm->log().toStdString();
        }
        if (!ok)
        {
            delete pgm;
            pgm = NULL;
            failed = true;
        }
        else
        {
            pgm->link();

            // Save uniform locations
            uint id = pgm->programId();

            uniforms["ratio"] = glGetUniformLocation(id, "ratio");
            uniforms["lights"] = glGetUniformLocation(id, "lights");
            uniforms["colorMap"] = glGetUniformLocation(id, "colorMap");
            uniforms["sphereMap"] = glGetUniformLocation(id, "sphereMap");
            uniforms["hasColorMap"] = glGetUniformLocation(id, "hasColorMap");
            uniforms["camera"] = glGetUniformLocation(id, "camera");
            uniforms["modelMatrix"] = glGetUniformLocation(id, "modelMatrix");
        }
    }
}
