#ifndef ZOOM_FILTER_H
#define ZOOM_FILTER_H

#include <systemc.h>


SC_MODULE(ZOOM_FILTER) {

    sc_in<bool> clk;
    sc_in<bool> reset_n;

    sc_in<bool> h_in;
    sc_in<bool> v_in;
    sc_out<bool> h_out;
    sc_out<bool> v_out;

    sc_in<unsigned char> p_in;
    sc_out<unsigned char> p_out;

    SC_CTOR(ZOOM_FILTER) {
        SC_THREAD(zoom);
        sensitive << clk.pos();
        async_reset_signal_is(reset_n, false);
        dont_initialize();

        buffer = new unsigned char[720*576];
    }

    void zoom();

private:
    unsigned char* buffer;
    virtual unsigned char computePixel(int index, unsigned char* image);
};

#endif
