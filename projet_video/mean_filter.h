#ifndef MEAN_FILTER_H
#define MEAN_FILTER_H

#include <systemc.h>
#include "mean_filter.h"

/***************************************
 *  définition du module
 **************************************/
SC_MODULE(MEAN_FILTER) {

   // IO PORTS
   sc_in<bool>         clk;
   sc_in<bool>         reset_n;

   sc_in<unsigned char> p_in;
   sc_in<bool>          h_in;
   sc_in<bool>          v_in;

   sc_out<unsigned char> p_out;
   sc_out<bool>          h_out;
   sc_out<bool>          v_out;


   /***************************************************
    *  constructeur
    **************************************************/
   SC_CTOR(MEAN_FILTER):pixels(2880)
   {
      cout << "Instanciation de " << name() <<" ..." ;

      SC_THREAD (gather_pixel);
      sensitive << clk.pos();
      SC_THREAD (gen_pixel);
      sensitive << clk.pos();

      async_reset_signal_is(reset_n,false);
      dont_initialize();

      reset_done  = false;

      const int width = 720;
      temp_buffer = (unsigned char *)malloc(width * 3 * sizeof(unsigned char));
      output_buffer = (unsigned char *)malloc(width * sizeof(unsigned char));
      output_line = 0;
      cout << "... réussie" << endl;
   }

   /***************************************************
    *  méthodes et champs internes
    **************************************************/
   private:

   void gather_pixel();
   void gen_pixel();

   bool                reset_done;
   bool                vertical_sync_done;
   sc_fifo<unsigned char>   pixels;
   unsigned char *     temp_buffer;
   unsigned char *     output_buffer;
   int                 output_line;
};

#endif
