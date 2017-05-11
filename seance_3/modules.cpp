#include <systemc.h>

SC_MODULE(foo) {
   sc_in <bool> i;
   sc_out<bool> o;
   SC_CTOR(foo):i("i"),o("o") { }
};

SC_MODULE(bar) {
   sc_in <bool> i;
   sc_out<bool> o;
   SC_CTOR(bar):i("i"),o("o") { }
};

SC_MODULE(foobar)
{
   // entrée/sortie
   sc_in <bool> i;
   sc_out<bool> o;
   // interne
   sc_signal<bool> s;
   // sous modules
   foo foo_i;
   bar bar_i;

   SC_CTOR(foobar)
      :i("i"),o("o"),s("s"),
      foo_i("foo"), bar_i("bar")
   {
      // connexions aux I/O
      foo_i.i(i);
      bar_i.o(o);
      // connexion interne
      foo_i.o(s);
      bar_i.i(s);
   }
};

int sc_main(int argc, char * argv[])
{
   sc_trace_file *trace_f;
   trace_f = sc_create_vcd_trace_file ("simu_modules");
   trace_f->set_time_unit(1,SC_NS);
   sc_signal<bool> i("i");
   sc_signal<bool> o("o");
   sc_clock ck("ck",10,SC_NS);
   foobar uut("foobar");

   uut.i(ck);
   uut.o(o);
   sc_trace(trace_f, ck, "ck");
   sc_trace(trace_f, o, "o");
   sc_trace(trace_f, uut.i, "foobar.i");
   sc_trace(trace_f, uut.o, "foobar.o");
   sc_trace(trace_f, uut.s, "foobar.s");
   sc_trace(trace_f, uut.foo_i.i, "foobar.foo_i.i");
   sc_trace(trace_f, uut.foo_i.o, "foobar.foo_i.o");
   sc_trace(trace_f, uut.bar_i.i, "foobar.bar_i.i");
   sc_trace(trace_f, uut.bar_i.o, "foobar.bar_i.o");
   for(int i = 0; i <= 20; i++)
    sc_start(10,SC_NS);
   return 0;
}
