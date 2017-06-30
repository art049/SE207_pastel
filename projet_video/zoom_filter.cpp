#include <cstdio>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include "zoom_filter.h"

void my_copy_n(unsigned char * inp, int n, unsigned char * out){
    for(int i=0; i < n; i++)
       out[i] =  inp[i];
}


void ZOOM_FILTER::zoom()
{
    while(true) {
        unsigned i_input = 0;
        unsigned i_output = 0;

        bool receivedOnce = false;
        h_out = 0;
        v_out = 0;
        unsigned char* image = new unsigned char[720*576];
        while(1) {
            if(!reset_n) {
                i_input = 0;
                i_output = 0;
                receivedOnce = false;
            }
            else {
                if(receivedOnce) {
                    h_out = h_in;
                    v_out = v_in;
                    if(h_in) {
                        p_out = computePixel(i_output, image);
                        ++i_output;
                        if(i_output == 720*576)
                            i_output = 0;
                    }
                }
                if(h_in) {
                    buffer[i_input] = p_in;
                    ++i_input;
                    if(i_input == 720*576) {
                        i_input = 0;
                        my_copy_n(&buffer[0], 720*576, &image[0]);
                        receivedOnce = true;
                    }
                }
            }
            wait();
        }
    }
}

unsigned char ZOOM_FILTER::computePixel(int index, unsigned char* image)
{
    int line = index / 720;
    int column = index % 720;

    return image[column/2+720/4 + 720*(line/2 + 576/4)];
}
