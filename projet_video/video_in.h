/*******************************************************************************
 * File   : video_in.h
 * Author : Alexis Polti/Tarik Graba
 * Date   : 2008-2016
 * This program is released under the GNU Public License
 * Copyright : Télécom ParisTECH
 *
 * Ce module modélise le fonctionnement d'une source vidéo.
 *
 * On lui donne un nom de base (ex:"img_src_"), et il transmet successivement
 * img_src_0.png, img_src_1.png, etc. S'il ne trouve pas une image, il
 * recommence à l'index 0.
 *
 ******************************************************************************/

#ifndef VIDEO_IN_H
#define VIDEO_IN_H

#include <systemc.h>
#include "image.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(VIDEO_IN) {

   // IO PORTS
   sc_in<bool>         clk;
   sc_in<bool>         reset_n;

   sc_out<bool>        href;
   sc_out<bool>        vref;

   sc_out<unsigned char> pixel_out;

   /***************************************************
    *  constructeur
    **************************************************/
   SC_CTOR(VIDEO_IN):base_name("wallace")
   {
      cout << "Instanciation de " << name() <<" ..." ;

      SC_THREAD (gen_sorties);
      sensitive << clk.pos();
      async_reset_signal_is(reset_n,false);
      dont_initialize();

      current_image_number = 0;
      image.pixel = NULL;
      read_image();

      cout << "... réussie" << endl;
   }

   /***************************************************
    *  méthodes et champs internes
    **************************************************/
   private:

   void gen_sorties();
   void read_image();

   const std::string   base_name;              // nom de base des images d'entrée
   int                 current_image_number;   // numéro de l'image courante

   Image               image;
};

#endif

