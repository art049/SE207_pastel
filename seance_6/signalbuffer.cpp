#include <systemc.h>

SC_MODULE(foo)
{
  sc_in<bool> clk;
  sc_signal<bool> bsignal;
  sc_buffer<bool> bbuffer;
  SC_CTOR(foo) {
    SC_THREAD(affect_thread);
    sensitive << clk;
    SC_THREAD(buffer_thread);
    sensitive << bbuffer;
    SC_THREAD(signal_thread);
    sensitive << bsignal;
  }
  void affect_thread(){
    wait();
    bsignal = true;
    bbuffer = true;
    while(1){
      wait();
      bsignal = false;
      bbuffer = false;
    }
  }
  void buffer_thread(){
    while(1){
      wait();
      cout << "Buffer event" << endl;
    }
  }
  void signal_thread(){
    while(1){
      wait();
      cout << "Signal event" << endl;
    }
  }
};


int sc_main(int argc, char * argv[]) {
  sc_clock clk("clk",10,SC_NS);
  foo foomodule("foomodule");
  foomodule.clk(clk);


  for(int i = 0; i <= 5; i++)
    sc_start(10,SC_NS);

   return 0;
}
