/* image.h */
#ifndef IMAGE_H
#define IMAGE_H
#include <png.h>


// Une image est un tableau de pixels (unsigned char, pour une image en niveaux de gris)
// On ne définit pas sa taille à l'avance, cela permettra de faire des simulations avec des images plus petites...
typedef struct Image {
   int   width, height;
   unsigned char  *pixel;
} Image;


// Comme les fonctions suivantes sont appelées à partir de C++, il est nécessaire de mettre quelques
// en-têtes spéciales, pour que le linker sache linker des fonctions C pures avec du C++

#ifdef __cplusplus
extern "C" {
#endif

   void   image_read(Image * image, const char *filename);
   void   image_write(Image * image, const char *filename);

#ifdef __cplusplus
}
#endif


#endif
