#include <cstdio>
#include <sstream>
#include <iomanip>
#include "video_out.h"

/***************************************************************************
 *      SC_THREAD principale
 ***************************************************************************/
void VIDEO_OUT::gather_pixel()
{
   reset_done = false;
   if(reset_n == false)
   {
      // Reset : on remet tous les paramètres à  zéro
      current_image_number = 0;
      vertical_sync_done = false;

      cout << "module: " << name() << "... reset!" << endl;
      reset_done = true;
   }
   // boucle infinie
   while(1)
   {
      if(!reset_done)
         cerr << "module: " << name() << " démarré sans reset!" << endl;
      int i = 0,j = 0;
      while(j < 719 || i < 576){
         if (!vertical_sync_done && vref)
            vertical_sync_done = true;
         if(vertical_sync_done){
            if (href)
               image.pixel[i*image.width + j++] = pixel_in;
            if (j == 720){
               i++;
               j = 0;
            }
         }
         wait();
      }
      write_image();
      vertical_sync_done = false;
      current_image_number++;
   }
}

/****************************************
 *  Ecritures des images successives
 ***************************************/

void VIDEO_OUT::write_image()
{
   std::ostringstream name_s;

   // Calcul du nom de la prochaine image.
   name_s << base_name << std::setfill('0') << std::setw(2) << current_image_number << ".png";

   std::cout << name() << " Ecriture de l'image " << name_s.str() << endl;
   image_write(&image, name_s.str().c_str());
}
