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
 *
 * The example below presents a use case of displacement mapping.
 * @include displacement.ddd
 *
 * @image html displacement.png "Output from displacement.ddd"
 * 
 * @note Multitexturing is a particular case of texture mapping.
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
 * Creates a local mapping.
 *
 * Define and evaluate the mapping tree while
 * preserving the current graphical state like a @ref locally.
 *
 * @param contents defines code applied on the current mapping
 */
mapping(contents:tree);

/**
 * Selects the color map.
 * Build a color texture out of image file @p filename like @ref texture
 * to use it during a mapping.
 *
 */
color_map(filename:text);

/**
 * Selects the displacement map.
 * Build a displacement texture out of image file @p filename like @ref texture
 * to use it during a displacement mapping.
 * The whitest points of the map are the most "displaced",
 * contrary to the most black which are not at all.
 *
 */
displacement_map(filename:text);

/**
 * Makes displacement mapping.
 *
 * Displace geometric position of points over the textured
 * surface defined by @ref displacement_map and set the
 * other map specified by @ref color_map as main texture.
 *
 *
 * @param offset displacement offset
 *
 * @note In order to obtain a good displacement, the used shapes must have important geometrical subdivision.
 * @note For that reason, this module includes a new 2D shape, @ref plane , which can be easily subdivided.
 */
displacement_mapping(offset:real);

/**
 * Selects the alpha map.
 * Build an alpha texture out of image file @p filename like @ref texture
 * to use it during an alpha mapping.
 * The most dark points of the texture allow to define
 * a transparent area during the mapping.
 *
 */
alpha_map(filename:text);

/**
 * Makes alpha mapping.
 *
 * Allow to define certain area of an objet as transparent or translucent thanks to
 * the texture defined by @ref alpha_map and set the
 * other map specified by @ref color_map as main texture.
 *
 *
 * @param threshold alpha threshold
 *
 */
alpha_mapping(threshold:real);

/**
 * Selects the normal map.
 * Build an normal texture out of image file @p filename like @ref texture
 * to use it during a normal mapping.
 *
 * @note This texture is a particular image, which allows to define normals in a texture.
 * @note There is an example of such an image below.
 *
 * @image html normal.jpg "Example of normal map"
 *
 */
normal_map(filename:text);

/**
 * Makes normal mapping.
 *
 * Simulate 3D on an object thanks to the association of normal map defined by @ref normal_map
 * with some light and material effets.
 * Set the other map specified by @ref color_map as main texture.
 *
 * @attention As normal mapping uses lights and materials, it is largely recommended to add some
 * of them to the current scene to increase its effect.
 *
 * @note This effect support only light zero.
 * @note Contrary to displacement mapping, normal mapping doesn't modify geometric positions.
 *
 * @see http://en.wikipedia.org/wiki/Normal_mapping
 */
normal_mapping();



