/*******************************************************************************
 * File   : video_in.h
 * Author : Alexis Polti/Tarik Graba
 * Date   : 2008-2016
 * This program is released under the GNU Public License
 * Copyright : Télécom ParisTECH
 *
 * This component models a video source
 *
 *****************************************************************************/

#include <cstdio>
#include <sstream>
#include <iomanip>
#include "video_in.h"

/***************************************************************************
 *      SC_THREAD principale
 ***************************************************************************/
void VIDEO_IN::gen_sorties()
{
   if(reset_n == false)
   {
      // Reset : on remet tous les paramètres à  zéro
      if(current_image_number !=0)
      {
         current_image_number = 0;
         read_image();
      }
      pixel_out = 0;
      href = false;
      vref = false;
      cout << "module: " << name() << "... reset!" << endl;
   }
   // boucle infinie
   while(1)
   {
      for(int i=0; i<625; i++)
         for(int j=0; j<874; j++)
         {
            // on attend le prochain coup d'horloge
            wait();
            // Si on est dans la fenêtre active, on sort le pixel courant
            // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
            if((i<576) && (j<720))
               pixel_out = image.pixel[i*image.width+j];
            else
               pixel_out = 0;

            // Génération de HREF
            // HREF est actif pendant la sortie des pixels actifs
            href = (i<576) && (j<720);

            // Génération de VREF
            // VREF est actif pendant les 3 premières lignes d'une image
            vref = (i<3);
         }

      // On a fini une image, on passe à la suivante
      current_image_number ++;
      read_image();
   }
}

/****************************************
 *  Lectures des images successives
 ***************************************/

void VIDEO_IN::read_image()
{
   std::ostringstream name_s;
   FILE *fin = NULL;

   // Calcul du nom de la prochaine image.
   name_s << base_name << std::setfill('0') << std::setw(2) << current_image_number << ".png";

   fin = fopen(name_s.str().c_str(), "rb");
   if(fin == NULL)
   {
      // Le Fichier n'existe pas, on remet l'index a zero
      current_image_number = 0;
      name_s.str("");
      name_s.clear();
      name_s << base_name << std::setfill('0') << std::setw(2) << current_image_number << ".png";

      fin = fopen(name_s.str().c_str(), "rb");
      // Problème! il n'y a aucune image avec ce nom de base
      if(fin == NULL) {
         std::cerr <<  name() << "(read_image): impossible d'ouvrir l'image source " << name_s.str() << endl;
         exit(-1);
      }
   }
   fclose(fin);

   // Lecture proprement dite de l'image PNG a l'aide de la libpng
   std::cout << name() << " Ouverture de l'image " << name_s.str() << endl;
   image_read(&image, name_s.str().c_str());

   // L'image est lue et chargée en mémoire.
   // On vérifie quand même que l'image lue a la bonne taille (720*576)
   if((image.width != 720) || (image.height != 576))
   {
      std::cerr << name() << " l'image " << name_s.str() << " n'a pas les bonnes dimensions (720*576)" << endl;
      exit(-1);
   }
}

