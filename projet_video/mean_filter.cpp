#include <cstdio>
#include <sstream>
#include <iomanip>
#include "mean_filter.h"


void MEAN_FILTER::gather_pixel()
{
   reset_done = false;
   if(reset_n == false)
   {
      // Reset : on remet tous les paramètres à  zéro

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
         if (!vertical_sync_done && v_in)
            vertical_sync_done = true;
         if(vertical_sync_done){
            if (h_in)
               pixels.write(p_in);
            if (j == 720){
               i++;
               j = 0;
            }
         }
         wait();
      }
      vertical_sync_done = false;
   }
}

void MEAN_FILTER::gen_pixel()
{
   reset_done = false;
   if(reset_n == false)
   {
      // Reset : on remet tous les paramètres à  zéro
      p_out = 0;
      h_out = false;
      v_out = false;
      output_line = 0;
      cout << "module: " << name() << "... reset!" << endl;
      reset_done = true;

   }

   if(!reset_done)
      cerr << "module: " << name() << " démarré sans reset!" << endl;

   // boucle infinie
   while(1)
   {
      //Wait for enough pixels in the FIFO
      while( pixels.num_available() < (720 * 3) -1 )
         wait();
      for(int i = 0; i < 720 * 3; i++)
         temp_buffer[i] = pixels.read();

      for(output_line = 0; output_line < 625; output_line++){
         if(output_line == 0){
            for(int i = 0; i < 720; i++ )
               output_buffer[i] = temp_buffer[i];
         }
         else if(output_line == 575){
            for(int i = 0; i < 720; i++ )
               output_buffer[i] = temp_buffer[2*720+i];
         }
         else {
            //Apply filter
            for(int i = 1; i < 719; i++ ){
               int sum = 0;
               sum += temp_buffer[720+i]; //Center pixel
               sum += temp_buffer[720+i-1];//Left
               sum += temp_buffer[720+i+1];//Right
               sum += temp_buffer[i];//Up
               sum += temp_buffer[2*720+i];//Down
               sum += temp_buffer[i-1];//Up Left
               sum += temp_buffer[2*720+i-1];//Down Left
               sum += temp_buffer[i+1];//Up Right
               sum += temp_buffer[2*720+i+1];//Down Right
               output_buffer[i] = sum / 9;
            }
            //Special case for line begining and ending
            output_buffer[0] = temp_buffer[720];
            output_buffer[719] = temp_buffer[720+719];
         }

         for(int i = 0; i < 864; i++){
            v_out = output_line < 3;
            h_out = (output_line<576) && (i<720);
            p_out = output_buffer[i%720];
            wait();
         }
         v_out = output_line < 3;
         h_out = false;

         //Shift temp_buffer and include a new line
         if(output_line != 0 && output_line < 574){
            for(int i = 0; i < 2*720; i++)
               temp_buffer[i] = temp_buffer[i+720];
            for(int i = 0; i < 720; i++)
               temp_buffer[2*720+i] = pixels.read();
         }
      }
   }
}
