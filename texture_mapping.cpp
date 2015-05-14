// ****************************************************************************
//  texture_mapping.cpp                                             Tao project
// ****************************************************************************
//
//   File Description:
//
//   Texture Mapping implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is licensed under the GNU General Public License v3.
// See file COPYING for details.
//  (C) 1992-2015 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include "texture_mapping.h"

const Tao::ModuleApi *TextureMapping::tao = NULL;


DLL_PUBLIC Tao::GraphicState * graphic_state = NULL;


// ============================================================================
//
//    Material
//
// ============================================================================

TextureMapping::TextureMapping()
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : context(NULL), program(NULL)
{}


TextureMapping::~TextureMapping()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
    delete program;
}


void TextureMapping::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((TextureMapping *)arg)->Draw();
}


void TextureMapping::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void TextureMapping::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (TextureMapping *)arg;
}


void TextureMapping::Draw()
// ----------------------------------------------------------------------------
//   Draw texture mapping
// ----------------------------------------------------------------------------
{
}


void TextureMapping::checkGLContext()
// ----------------------------------------------------------------------------
//   Re-create context-dependent resources if GL context has changed
// ----------------------------------------------------------------------------
{
    tao->makeGLContextCurrent();
    const QGLContext *currentContext = QGLContext::currentContext();
    if (context != currentContext)
    {
        IFTRACE(mapping)
            debug() << "Context has changed\n";
        delete program;
        program = NULL;
        context = currentContext;
        createShaders();
    }
}


void TextureMapping::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs for the material
// ----------------------------------------------------------------------------
{}


std::ostream & TextureMapping::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[Mapping] " << (void*)this << " ";
    return std::cerr;
}
