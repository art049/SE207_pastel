#include <systemc.h>

SC_MODULE(foo)
{
  sc_in<bool> ck;
  sc_mutex m;
  SC_CTOR(foo) {
    SC_THREAD(first_thread);
    sensitive << ck.pos();
    SC_THREAD(second_thread);
    sensitive << ck.pos();
  }
  void first_thread(){
    while(1){
      m.lock();
      cout << "First thread working @"<< sc_time_stamp() << endl;
      for(int i = 0; i < 10; i++) wait();
      m.unlock();
      wait();
    }
  }
  void second_thread(){
    while(1){
      m.lock();
      cout << "Second thread working @" << sc_time_stamp() << endl;
      for(int i = 0; i < 10; i++) wait();
      m.unlock();
      wait();
    }
  }
};


int sc_main(int argc, char * argv[]) {
  sc_clock ck("ck",1,SC_NS);
  foo foomodule("foomodule");
  foomodule.ck(ck);
  for(int i = 0; i <= 15; i++)
    sc_start(10,SC_NS);

   return 0;
}
