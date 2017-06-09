#include <systemc.h>

uint8_t fun_pgcd(uint8_t a, uint8_t b){
  while(a > 1 && b > 1){
    while(a <= b)
      b-=a;
    std::swap(a, b);
  }
  return b;
}

SC_MODULE(pgcd_module) {
  sc_in<bool> clk;
  sc_in<bool> valid;
  sc_in<uint8_t> a;
  sc_in<uint8_t> b;
  sc_out<bool> ready;
  sc_out<uint8_t> gcd;
  SC_CTOR(pgcd_module) {
    SC_THREAD(pgcd_thread);
    sensitive << clk;
  }
  void pgcd_thread() {
    while(1){
      if(valid){
        uint8_t min = std::min(a, b);
        uint8_t max = std::max(a, b);
        while(min != 0) {
          uint8_t d = max - min;
          max = std::max(d, min);
          min = std::min(d, min);
          wait();
        }
        gcd = max;
        ready = true;
      }
      wait();
    }
  }
};

SC_MODULE(pgcd_rtl_module) {
  sc_in<bool> clk;
  sc_in<bool> valid;
  sc_in<uint8_t> a;
  sc_in<uint8_t> b;
  sc_out<bool> ready;
  sc_out<uint8_t> gcd;

  sc_signal<uint8_t> min;
  sc_signal<uint8_t> max;
  sc_signal<bool> working;

  SC_CTOR(pgcd_rtl_module) {
    SC_METHOD(core);
    dont_initialize();
    sensitive << clk.pos();
    SC_METHOD(controller);
    dont_initialize();
    sensitive << clk.pos();
  }

  void core(){
    if(working){
      int d = max - min;
      cout << (int) min << " " << (int) max << endl;
      cout << d << endl;
      if(min == 0){
        ready = true;
        gcd = max;
      }
      else if(d > min)
        max = d;
      else {
        max = min;
        min = d;
      }
    }
    else if(valid){
      ready = false;
      max = a > b ? a : b;
      min = a <= b ? a : b;
    }
    else{
      ready = false;
    }
  }

  void controller(){
    if(ready)
      working = false;
    else if(valid)
      working = true;
    else
      working = false;
  }
};

int sc_main(int argc, char * argv[]){
  sc_trace_file *trace_f;
  trace_f = sc_create_vcd_trace_file ("trace_pgcd");
  trace_f->set_time_unit(1,SC_NS);
  sc_clock clk("clk", 10, SC_NS);

  sc_signal<uint8_t> a;
  sc_signal<uint8_t> b;
  sc_signal<bool> valid;
  sc_signal<bool> ready;
  sc_signal<uint8_t> gcd;
  a = 33;
  b = 27;
  valid = true;

  pgcd_rtl_module pgcdRtl("gcd");
  pgcdRtl.clk(clk);
  pgcdRtl.valid(valid);
  pgcdRtl.a(a);
  pgcdRtl.b(b);
  pgcdRtl.ready(ready);
  pgcdRtl.gcd(gcd);

  sc_trace(trace_f, clk, "Clock");
  sc_trace(trace_f, pgcdRtl.working, "working");
  sc_trace(trace_f, gcd, "gcd");
  sc_trace(trace_f, ready, "ready");

  sc_start(20, SC_NS);
  valid = false;
  sc_start(8000, SC_NS);
  sc_close_vcd_trace_file(trace_f);
  return 0;
}
