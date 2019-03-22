// *****************************************************************************
// texture_mapping.cpp                                             Tao3D project
// *****************************************************************************
//
// File description:
//
//   Texture Mapping implementation.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2011-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2011-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2011-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
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
