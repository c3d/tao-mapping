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
 * @attention In case where lights have been placed in the current scene, a normal map have to be bind on texture unit 2 to
 * adjust correct normals of the model. It is possible to create a normal map with @ref normal_map
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
 * Convert current texture into a basic normal map.
 * Make a basic normal map thanks to the current texture.
 * This one can be use to make a normal mapping or a displacement mapping.
 *
 * @note A better result can be obtain if the current texture is a black and white one (for instance: a displacement map)
 */
normal_map();

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
 * @note This effect can support a maximum of 8 differents lights.
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
 * @param contents defines code of the current cube map.
 *
 * An example of use of this effect is described below :
@
cube_map
   cube_map_face 0, "right.png"
   cube_map_face 1, "face.png"
   cube_map_face 2, "top.png"
   cube_map_face 3, "bottom.png"
   cube_map_face 4, "front.png"
   cube_map_face 5, "back.png"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 *
 * @note This cube map does not support multi-texturing without shaders.
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
 * @attention Each texture has to be a square (same width and height) and has to have the same size for each face to generate correctly the cube map.
 *
 */
cube_map_face(face:integer, filename:text);

/**
 * Flips following faces of the current cube map.
 *
 * Mirror following faces of the current cube map according to uv-coordinates.
 *
 * @param u enable or disable flipping of u-coordinate. Its default value is false.
 * @param v enable or disable flipping of v-coordinate. Its default value is false.
 */
cube_map_flip(u:boolean, v:boolean);

/**
 * Makes cube mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a texture called cube map, which can be generated thanks to @ref cube_map.
 *
 * @param ratio ratio of the environment reflection.
 *
 * @attention A @ref cube_map has to be define previously in order to generate reflection/refraction effects.
 * @attention This effect has to be apply just before the choosen object in order to be subject correctly to all its transformations.

 *
 * An example of use of this effect is described below :
@code
// Define color map
texture_unit 0
texture "color_map.png"
// Define cube map for reflection
texture_unit 1
cube_map
   cube_map_face 0, "right.png"
   cube_map_face 1, "face.png"
   cube_map_face 2, "top.png"
   cube_map_face 3, "bottom.png"
   cube_map_face 4, "front.png"
   cube_map_face 5, "back.png"
rotatex time * 20
rotatey 90
// Apply effect juste before drawing object
cube_mapping 0.5
cube 0, 0, 0, 5000, 5000, 5000
@endcode
 *
 * @note This method is more efficient than @ref sphere_mapping but needs six textures to define the environment.
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
 * @attention This effect has to be apply just before the choosen object in order to be subject correctly to all its transformations.
 *
 * An example of use of this effect is described below :
@code
// Define color map
texture_unit 0
texture "color_map.png"
// Define sphere map for reflection
texture "sphere_map.png"
rotatex time * 20
rotatey 90
// Apply effect juste before drawing object
sphere_mapping 0.5
sphere 0, 0, 0, 500, 500, 500, 50, 50
@endcode
 *
 * @note The sphere map is a particular image, which defines environment reflection.
 * @note There is an example of such an image below.
 * @note It is also possible to use a regular image as sphere map with a suitable scaling to generate static reflection.
 *
 * @image html sphere.jpg "Example of sphere map"
 *
 * @see http://en.wikipedia.org/wiki/Sphere_mapping
 *
 */
sphere_mapping(ratio:integer);
