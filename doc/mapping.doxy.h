/**
 * @defgroup Mapping Mapping
 * @ingroup Modules
 *
 * Define some mapping techniques.
 *
 * Texture mapping is a method for adding detail onto a triangle,
 * thanks to a surface texture (a bitmap or raster image).
 *
 * This technique allows in this way to get fine scale details without
 * resorting to rendering tons of tiny triangles.
 *
 * The image that gets mapped onto the triangle is
 * called a texture map or texture and is usually a
 * regular color image.
 *
 * So, this module allows to apply some techniques based on this principe:
 *       - displacement mapping
 *       - normal mapping
 *       - alpha mapping
 *       - sphere mapping
 *       - etc.
 *
 * @attention All these techniques use multitexturing to obtain good results.
 * @attention Thus, color and texture map have to be bind respectively on first and second texture units before applying the effect.
 *
 * @note Multitexturing is a particular case of texture mapping.
 *
 * The example below presents a use case of displacement mapping.
 * @include displacement.ddd
 *
 * @image html displacement.png "Output from displacement.ddd"
 *
 *
 * @{
 */

/**
 * Draws a subdivided plane.
 *
 * This plane is centered at (@p x, @p y), with width @p w and height @p h,
 * and is subdivided according to its line @p l and column @p c numbers.
 *
 * @note With a convenient subdivsion, this plane can be used to make a displacement mapping.
 *
 */
plane(x:real, y:real, w:real, h:real, l:integer, c:integer);

/**
 * Makes displacement mapping.
 *
 * Displace geometric position of points over the textured
 * surface defined by @ref displacement_map and set the
 * other map specified by @ref color_map as main texture.
 * The whitest points of the texture map are the most "displaced",
 * contrary to the most black which are not at all.
 *
 * @param ratio displacement ratio
 *
 * @attention In order to obtain a good displacement, the used shapes must have important geometrical subdivision.
 *
 * @note For that reason, this module includes a new 2D shape, @ref plane , which can be easily subdivided.
 */
displacement_mapping(ratio:real);

/**
 * Makes alpha mapping.
 *
 * Allow to define certain area of an objet as transparent or translucent thanks to
 * the texture defined by @ref alpha_map and set the
 * other map specified by @ref color_map as main texture.
 * The most dark points of the texture map allow to define
 * a transparent area during the mapping.
 *
 * @param threshold alpha threshold
 *
 */
alpha_mapping(threshold:real);

/**
 * Makes normal mapping.
 *
 * Simulate 3D on an object thanks to the association of normal map defined by @ref normal_map
 * with some light and material effets.
 * Set the other map specified by @ref color_map as main texture.
 *
 * @attention As normal mapping uses a light and some materials, it is largely recommended to add it
 * to the current scene to increase the effects.
 *
 * @note This effect support only light zero.
 * @note Contrary to displacement mapping, normal mapping doesn't modify geometric positions.
 * @note This mapping use a particular texture, which allows to define normals used during mapping.
 * @note There is an example of such an image below.
 *
 * @image html normal.jpg "Example of normal map"
 *
 * @see http://en.wikipedia.org/wiki/Normal_mapping
 */
normal_mapping();

/**
 * Generates a random noise image.
 * Make a random noise texture with width @p w and height @p h.
 * This one can be use to make a noise mapping.
 *
 */
noise_map(w:integer, h:integer);

/**
 * Makes noise mapping.
 *
 * Add noise to a main texture define by @ref color_map according to the textured
 * surface defined by @ref noise_map.
 *
 * @param ratio noise ratio
 *
 * @note @ref noise_map(w:integer, h:integer) can be used to generate directly random noise map.
 * @note This texture is a particular image, which allows to define noise used during mapping.
 * @note There is an example of such an image below.
 *
 * @image html noise.png "Example of noise map"
 *
 * @see http://en.wikipedia.org/wiki/Image_noise
 */
noise_mapping(ratio:integer);

/**
 * Creates a local cube map.
 *
 * Define and evaluate the cube map tree generate thanks to @ref cube_face.
 * This texture can be used directly to create nice environment or
 * during a cube mapping to generate reflections.
 *
 * @note This cube map does not support multi-texturing without shaders.
 *
 * @param contents defines code of the current cube map.
 */
cube_map(contents:tree);

/**
 * Adds a texture to the current cube map.
 *
 * Select a texture by its @p filename and bind it to a @p face
 * of a cube map create by @ref cube_map.
 *
 * Faces of the cube map are defined in this way :
 *      - Right  = 0
 *      - Left   = 1
 *      - Top    = 2
 *      - Bottom = 3
 *      - Front  = 4
 *      - Back   = 5
 *
 * @attention Each texture must be a square (same width and height) and must have the same size for each face to generate correctly the cube map.
 *
@code
cube_map
    cube_face 0, "right.bmp"
    cube_face 1, "face.bmp"
    cube_face 2, "top.bmp"
    cube_face 3, "bottom.bmp"
    cube_face 4, "front.bmp"
    cube_face 5, "back.bmp"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 *
 */
cube_face(face:integer, filename:text);

/**
 * Makes cube mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a texture called cube map, which can be generated thanks to @ref cube_map.
 *
 * @param ratio ratio of the environment reflection.
 *
 * @note This method is more efficient than @ref sphere_mapping
 * but needs six textures to define the environment.
 *
 * @see http://en.wikipedia.org/wiki/Cube_mapping
 */
cube_mapping(ratio:integer);

/**
 * Makes sphere mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a texture called sphere map set by @ref texture.
 *
 * @param ratio ratio of the environment reflection.
 *
 * @note This texture is a particular image, which defines environment reflection.
 * @note There is an example of such an image below.
 *
 * @image html sphere.jpg "Example of sphere map"
 *
 * @see http://en.wikipedia.org/wiki/Sphere_mapping
 */
sphere_mapping(ratio:integer);

/**
 * Makes reflection mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a common texture define set by @ref texture.
 *
 * @param ratio ratio of the environment reflection.
 *
 */
reflection_mapping(ratio:integer);







