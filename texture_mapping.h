#ifndef TEXTURE_MAPPING_H
#define TEXTURE_MAPPING_H

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include <QObject>

using namespace std;
using namespace Tao;

struct TextureMapping : public QObject
{
    TextureMapping();
    ~TextureMapping();

    // Draw mapping
    virtual void    Draw();

    static void     render_callback(void *arg);
    static void     delete_callback(void *arg);

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;
};


#endif // TEXTURE_MAPPING_H
