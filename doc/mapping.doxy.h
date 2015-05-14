/**
 * @~english
 * @taomoduledescription{Mapping, Mapping}
 * <tt>import Mapping</tt> - Define some texture mapping techniques.@n
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
 * @note Multitexturing is a particular case of texture mapping.
 *
 * The example below presents a use case of displacement mapping.
 * @include displacement.ddd
 *
 * @image html displacement.png "Displacement mapping"
 *
 * @~french
 * @taomoduledescription{Mapping, Mapping}
 * <tt>import Mapping</tt> - Techniques de mapping.@n
 *
 * Le @a mapping de textures est une méthode pour ajouter des détails sur un
 * triangle grâce à une texture (une image @a bitmap). Elle permet d'ajouter
 * des détails sans devoir définir des géométries très fines.
 *
 * Ce module permet d'appliquer les techniques suivantes :
 *       - displacement mapping
 *       - normal mapping
 *       - alpha mapping
 *       - sphere mapping
 *
 * Voici un exemple de <em>displacement mapping</em> :
 * @include displacement.ddd
 *
 * @image html displacement.png "Displacement mapping"
 *
 * @~
 * @endtaomoduledescription{Mapping}
 *
 * @{
 */



/**
 * @~english
 * Makes displacement mapping.
 *
 * Displace geometric position of points over the textured
 * surface.\n
 * The whitest points of the texture map are the most "displaced",
 * contrary to the most black which are not at all.\n
 * This displacement map have to be bound on texture unit 1.
 *
 * @image html displacementMap.png "Example of displacement map"
 *
 * The color of the mapping can be set either by @p color or by a texture bound on texture unit 0, but also both.
 *
 * @image html colorMap.png "Example of color map"
 *
 * @param ratio displacement ratio
 *
 * @attention In case where lights have been placed in the current scene, a normal map have to be bound on texture unit 2 to
 * adjust correct normals of the model. It is possible to create a normal map with @ref normal_map
 *
 * @note In order to obtain a good displacement, the used shapes must have important geometrical subdivision.
 * See @ref plane.
 *
 * @~french
 * Active le displacement mapping.
 *
 * Déplace la position des points de la surface en fonction d'une texture.
 * La luminosité des points de la texture active est utilisée pour déterminer
 * l'importance du déplacement des sommets de la géométrie. Les points qui sont
 * les plus clairs sont les plus déplacés, les points noirs ne bougent pas.
 *
 * La carte des déplacements (<em>displacement map</em>) doit étre sur l'unité
 * de texture numéro 1.
 *
 * @image html displacementMap.png "Exemple de carte des déplacements"
 *
 * La couleur d'un point de l'objet est déterminée par la couleur en cours et
 * par la texture éventuellement présente sur l'unité de texture 0 (carte des couleurs).
 *
 * @image html colorMap.png "Exemple de carte des couleurs"
 *
 * Si la scène comporte des lumières, il peut être souhaitable d'associer une
 * carte des normales (<em>normal map</em>) à l'unité 2 afin de corriger les
 * normales du modèle. La fonction @ref normal_map peut être utile pour cela.
 *
 * @note Afin d'obtenir de bons résultats, la forme géométrique doit être très
 * subdivisée. Cf. @ref plane.
 *
 * @param ratio Importance du déplacement.
 */
displacement_mapping(ratio:real);

/**
 * @~english
 * Makes alpha mapping.
 *
 * Allow to define certain area of an objet as transparent or translucent thanks to
 * the texture defined by @p alpha_map.\n
 * The most dark points of the texture map allow to define
 * a transparent area during the mapping.\n
 * This alpha map have to be bound on texture unit 1.
 * The color of the mapping can be set either by @p color or by a texture bound on texture unit 0, but also both.
 *
 * @param threshold alpha threshold
 *
 * @~french
 * Active l'alpha mapping.
 *
 * Cette technique permet de supprimer certains points d'une
 * surface, à partir d'une texture. Les pixels de la texture qui sont plus
 * sombres qu'un niveau donné (@p threshold)
 * vont correspondre à une zone transparente ou "percée" de l'objet.
 *
 * La carte alpha (<em>alpha map</em>) doit être sur l'unité de texture numéro
 * 1.
 *
 * La couleur d'un point de l'objet est déterminée par la couleur en cours et
 * par la texture éventuellement présente sur l'unité de texture 0.
 *
 * @param threshold Si les trois composantes (rouge, verte, bleue) de la
 * couleur du pixel de la texture sont inférieures à cette valeur, le point
 * n'est pas affiché (et l'objet est "percé" à cet androit).
 */
alpha_mapping(threshold:real);

/**
 * @~english
 * Convert current texture into a basic normal map.
 * Make a basic normal map thanks to the current texture.
 * This one can be use to make a normal mapping or a displacement mapping.
 *
 * Best results are obtained if the current texture is a grayscale picture
 * (for instance: a displacement map)
 *
 * @~french
 * Convertit la texture courante en une carte de normales simples.
 * Génère une texture qui peut servir de <em>normal map</em> pour un
 * <em>normal mapping</em> (@ref normal_mapping) ou un
 * <em>displacement mapping</em> (@ref displacement_mapping).
 *
 * Les meilleurs résultats sont obtenus lorsque la texture est en niveaux
 * de gris. Dans le cas du <em>displacement mapping</em>, la carte de
 * déplacement peut être passée telle quelle à cette fonction pour obtenir
 * une <em>normal map</em> qui pourra à son tour être utilisée dans le
 * @ref displacement_mapping.
 */
normal_map();

/**
 * @~english
 * Makes normal mapping.
 *
 * Simulate 3D on an object thanks to the association of normal map defined by @ref normal_map
 * with some light and material effets.\n
 * This normal map have to be bound on texture unit 1.
 * The color of the mapping can be set either by @p color or by a texture bound on texture unit 0, but also both.
 *
 * @attention As normal mapping uses a light and some materials, it is largely recommended to add it
 * to the current scene to increase the effects.
 *
 * @note Contrary to displacement mapping, normal mapping doesn't modify geometric positions.
 * @note This mapping use a particular texture, which allows to define normals used during mapping.
 * @note There is an example of such an image below.
 *
 * @image html normal.jpg "Example of normal map"
 *
 * @see http://en.wikipedia.org/wiki/Normal_mapping
 *
 * @~french
 * Active le normal mapping.
 *
 * Simule un effet 3D sur un objet grâce à l'utilisation d'une
 * <em>normal map</em>. Pour que l'effet soit visible, il faut que l'éclairage
 * soit activé.
 *
 * La carte de normales (<em>normal map</em>) doit être sur l'unité de texture
 * numéro 1.
 *
 * @image html normal.jpg "Exemple de normal map"
 *
 * La couleur d'un point de l'objet est déterminée par la couleur ou les
 * matériaux en cours et par la texture éventuellement présente sur l'unité de
 * texture 0. La lumière modifie localement la couleur conformément à la carte
 * de normales.
 *
 * @see http://fr.wikipedia.org/wiki/Normal_mapping
 */
normal_mapping();

/**
 * @~english
 * Generates a 2D texture with some noise.
 * Make a noise texture with width @p w and height @p h.
 * @p seed define the distribution of the noise on the image.
 * This type of texture can be use to make a noise mapping.
 *
 * @~french
 * Crée une texture de bruit à deux dimensions.
 * La texture générée a pour dimensions @p w par @p h pixels, et est
 * constituée de bruit pseudo-aléatoire. Le paramètre @p seed
 * définit la distribution du bruit.
 */
noise_map(w:integer, h:integer, seed:integer);

/**
 * @~english
 * Generates a 3D texture with some noise.
 * Make a noise texture with width @p w and height @p h.
 * @p seed defines the distribution of the noise on the image. In this case, there is limit of 200 seeds.
 * This type of texture can be use to make a noise mapping and can be set in a GLSL shader thanks to \e sampler3D.
 *
 * @~french
 * Crée une texture de bruit à trois dimensions.
 * La texture générée a pour dimensions @p w par @p h pixels, et est
 * constituée de bruit pseudo-aléatoire. Le paramètre @p seed
 * définit la distribution du bruit.
 * Ce type de texture peut être passé à un @a shader GLSL en utilisant
 * le type @e sampler3D.
 */
noise_map_3D(w:integer, h:integer, seed:integer);

/**
 * @~english
 * Makes noise mapping.
 *
 * Add noise to a main texture bind on the unit 0 according to the textured
 * surface defined by a particular image named noise_map.\n
 * The texture unit 1 allows to specify a simple noise texture, which can be generated by @ref noise_map.
 * The texture unit 2 can also be used to set 3D noise, generated for instance by @ref noise_map_3D.
 *
 * @param ratio noise ratio
 *
 * @note There is an example of noise map below.
 *
 * @image html noise.png "Example of noise map"
 *
 * @see http://en.wikipedia.org/wiki/Image_noise
 *
 * @~french
 * Active le noise mapping.
 *
 * Ajoute du bruit à une texture principale, présente sur l'unité de texture
 * 0, en fonction d'une texture de bruit (<em>noise map</em>), qui peut être :
 * - soit une texture 2D, générée par exemple par @ref noise_map,
 * - soit une texture 3D, générée par exmple par @ref noise_map_3D.
 *
 * @image html noise.png "Example de noise map"
 */
noise_mapping(ratio:integer);

/**
 * @~english
 * Creates a local cube map.
 *
 * Define and evaluate the cube map tree generate thanks to @ref cube_map_face according to the defined size.
 * This texture can be used directly to create nice environment or
 * during a cube mapping to generate reflections.
 *
 * @param size     allows to rescale directly the images as squares. If 0 then there is no rescaling.
 * @param contents defines code of the current cube map.
 *
 * An example of use of this effect is described below:
@code
cube_map 1024,
   cube_map_face 0, "right.png"
   cube_map_face 1, "left.png"
   cube_map_face 2, "top.png"
   cube_map_face 3, "bottom.png"
   cube_map_face 4, "front.png"
   cube_map_face 5, "back.png"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 *
 * @note This texture have a higher priority than a common texture.
 * @note The cube map does not support multi-texturing without shaders.
 *
 * @~french
 * Définit une cube map.
 *
 * Une <em>cube map</em> se construit en utilisant @ref cube_map_face.
 * Le résultat peut être ensuite utilisé par @ref cube_mapping pour générer
 * un environnement (un très grand cube qui englobe la caméra) et sur un objet
 * pour simuler des réflexions de l'environnement sur l'objet.
 *
 * @param size Permet de redimensionner les images sous forme de carrés. Si 0,
 * il n'y a pas de redimensionnement.
 * @param contents Définit les faces du cube.
 *
 * Voici un exemple :
@code
cube_map 1024,
   cube_map_face 0, "droite.png"
   cube_map_face 1, "gauche.png"
   cube_map_face 2, "dessus.png"
   cube_map_face 3, "dessous.png"
   cube_map_face 4, "avant.png"
   cube_map_face 5, "arrière.png"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 */
cube_map(size:integer, contents:tree);

/**
 * @~english
 * Creates a local cube map from a cross-shaped image.
 *
 * Define a cube map using a cross-shaped image.
 * This texture can be used directly to create nice environment or
 * during a cube mapping to generate reflections.
 *
 * @param cross Specify the source image
 *
 * An example of use of this effect is described below:
@code
cube_map_cross "cross.png"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 *
 * @note This texture have a higher priority than a common texture.
 * @note The cube map does not support multi-texturing without shaders.
 *
 * @~french
 * Définit une cube map à partir d'une image en croix.
 * Le résultat peut être ensuite utilisé par @ref cube_mapping pour générer
 * un environnement (un très grand cube qui englobe la caméra) et sur un objet
 * pour simuler des réflexions de l'environnement sur l'objet.
 *
 * @param cross Image source à utiliser
 *
 * Voici un exemple :
@code
cube_map_cross "cross.png"
cube 0, 0, 0, 30000, 30000, 30000
@endcode
 */
cube_map_cross(cross:text);

/**
 * @~english
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
 * @~french
 * Ajoute une texture à une cube map.
 * Charge le fichier @p filename et l'associe à une face du cube. Cette
 * fonction s'utilise dans le cadre de @ref cube_map.
 *
 * Le paramètre @p face définit la position de la face sur le cube :
 *  - 0 : droite
 *  - 1 : gauche
 *  - 2 : dessus
 *  - 3 : dessous
 *  - 4 : avant
 *  - 5 : arrière
 */
cube_map_face(face:integer, filename:text);

/**
 * @~english
 * Flips following faces of the current cube map.
 *
 * Mirror following faces of the current cube map according to uv-coordinates.
 *
 * @param u enable or disable flipping of u-coordinate. Its default value is false.
 * @param v enable or disable flipping of v-coordinate. Its default value is false.
 *
 * @~french
 * Inverse les images d'une ou plusieurs faces de cube map.
 *
 * Toutes les faces déclarées par @ref cube_map_face qui suivent cet
 * appel sont modifiées. Pour chaque axe @c u et @c v, @c true active le mirroir,
 * @c false le désactive.
 *
 * @param u Miroir selon l'axe @c u (abscisses sur la texture).
 * @param v Miroir selon l'axe @c v (ordonnées sur la texture).
 */
cube_map_flip(u:boolean, v:boolean);

/**
 * @~english
 * Makes cube mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a texture called cube map, which can be generated thanks to @ref cube_map.\n
 * This cube map have to be bound on texture unit 1.
 * The color of the mapping can be set either by @p color or by a usual texture bound on texture unit 0, but also both.
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
cube_map 1024,
   cube_map_face 0, "right.png"
   cube_map_face 1, "left.png"
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
 *
 * @~french
 * Active le cube mapping.
 *
 * Permet de simuler des effets de réflexion d'un environnement.
 * Après création d'une <em>cube map</em> par la primitive @ref cube_map,
 * cette primitive en active l'utilisation sur les objets qui suivent.
 *
 * Cette technique est plus efficace que @ref sphere_mapping mais demande six
 * textures pour définir l'environnement.
 *
 * @param ratio Valeur entre 0.0 et 1.0 qui spécifie la contribution du
 * <em>cube map</em> à la couleur de l'objet.
 *
 * Exemple :
@code
// Texture pour colorer l'objet (color map)
texture_unit 0
texture "color_map.png"
// Définition d'une cube map pour simuler une réflexion
texture_unit 1
cube_map 1024,
   cube_map_face 0, "droite.png"
   cube_map_face 1, "gauche.png"
   cube_map_face 2, "dessus.png"
   cube_map_face 3, "dessous.png"
   cube_map_face 4, "avant.png"
   cube_map_face 5, "arrière.png"
rotatex time * 20
rotatey 90
// On applique l'effet juste avant de dessiner l'objet
cube_mapping 0.5
cube 0, 0, 0, 5000, 5000, 5000
@endcode
 */
cube_mapping(ratio:integer);

/**
 * @~english
 * Makes sphere mapping.
 *
 * Allow to simulate easily the reflection of an environment on an object thanks
 * to a texture called sphere map set by @p texture.\n
 * This sphere map have to be bound on texture unit 1.
 * The color of the mapping can be set either by @p color or by a usual texture bound on texture unit 0, but also both.
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
texture_unit 1
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
 * @~french
 * Active le sphere mapping.
 *
 * Comme le <em>cube mapping</em>, le <em>sphere mapping</em> permet de simuler
 * des réflexions sur un objet. Il utilise seulement une texture appelée
 * <em>sphere map</em>, qui doit être associée à l'unité de texture 1.
 *
 * @image html sphere.jpg "Exemple de sphere map"
 *
 * En dehors de la contribution du <em>sphere mapping</em>, la couleur d'un
 * point de l'objet est déterminée par la couleur en cours et
 * par la texture éventuellement présente sur l'unité de texture 0.
 *
 * @param ratio Valeur entre 0.0 et 1.0 qui spécifie la contribution du
 * <em>sphere map</em> à la couleur de l'objet.
 *
 * Exemple :
@code
// Texture pour colorer l'objet (color map)
texture_unit 0
texture "color_map.png"
// Définition d'une sphere map pour simuler une réflexion
texture_unit 1
texture "sphere_map.png"
rotatex time * 20
rotatey 90
// On applique l'effet juste avant de dessiner l'objet
sphere_mapping 0.5
sphere 0, 0, 0, 500, 500, 500, 50, 50
@endcode
 */
sphere_mapping(ratio:integer);

/**
 * @}
 */
