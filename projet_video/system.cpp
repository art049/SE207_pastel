/**********************************************************************
 * File : system.cpp
 * Date : 12/03/2008
 * Author :  Alexis Polti
 * 
 * This program is released under the GNU public license
 * Télécom ParisTECH
 *
 * Testbench pour video_in
 **********************************************************************/

#include "systemc.h"
#include "video_in.h"

#include <cstdio>
#include <cstdlib>


/***************************************************
 *	MAIN
 **************************************************/

int sc_main (int argc, char *argv[])
{

    int	ncycles;

    /* La periode de l'horloge du signal video */
    sc_time pix_period(74, SC_NS);


    if (argc == 2) {
        ncycles = atoi(argv[1]);
    } else{
        printf("\n");
        printf("The number of simulation cycles must be defined in the command line (-1 for unlimited)\n\n");
        exit(1);
    }



    /******************************************************
     *      Declaration des signaux
     *****************************************************/

    sc_clock                        signal_clk("Clock", pix_period);
    sc_signal<bool>                 signal_resetn;

    sc_signal<bool>                 signal_vref, signal_href;
    sc_signal<unsigned char>        signal_pixel;



    /********************************************************
     *	Instanciation des composants
     *******************************************************/

    VIDEO_IN video_in("VIDEO_GEN");

    /*********************************************************
     *	Connexion des composants
     ********************************************************/

    video_in.clk        (signal_clk);
    video_in.reset_n    (signal_resetn);
    video_in.href       (signal_href);
    video_in.vref       (signal_vref);
    video_in.pixel_out  (signal_pixel);

    /*********************************************************
     *	Traces
     ********************************************************/

    /* open trace file */
    sc_trace_file * my_trace_file;
    my_trace_file = sc_create_vcd_trace_file ("simulation_trace");
    my_trace_file->set_time_unit(1,SC_NS);

    /* chronogrammes signaux CLK et NRESET */
    sc_trace(my_trace_file, signal_clk,         "clk");
    sc_trace(my_trace_file, signal_resetn,      "reset_n");

    /* chronogrammes video */
    sc_trace(my_trace_file, signal_href,        "href");
    sc_trace(my_trace_file, signal_vref,        "vref");
    sc_trace(my_trace_file, signal_pixel,       "pixel");


    /*********************************************************
     *	Simulation
     ********************************************************/

    /* Initialisation de la simulation */
    signal_resetn = true;
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());

    /* Generation d'un reset */
    signal_resetn = false;
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());
    signal_resetn = true;

    /* Lancement de la simulation */
    int i=0;
    while(1) {

        /* Si on a atteint le nombre de cycles demandés, on s'arrête */
        if(ncycles > 0)
            if(i >= ncycles)
                break;
        sc_start(signal_clk.period());
        i++;

    } // end simulation loop

    cout << "End of simulation @ " << sc_time_stamp() << endl;

    /* Close trace file */
    sc_close_vcd_trace_file (my_trace_file);


    return EXIT_SUCCESS;

}
