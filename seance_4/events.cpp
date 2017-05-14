#include <systemc.h>

struct Pixel {
   sc_uint<5> R;
   sc_uint<6> G;
   sc_uint<5> B;
   Pixel(sc_uint<5> _R =0, sc_uint<6> _G=0, sc_uint<5> _B=0): R(_R), G(_G), B(_B) { }

   bool operator == (const Pixel &other) {
      return (R == other.R) && (G == other.G) && (B == other.B);
   }
   Pixel operator + (const Pixel &other){
     Pixel *p = new Pixel(std::min(R.to_uint() + other.R.to_uint(), static_cast<unsigned int> (31)),
                          std::min(G.to_uint() + other.G.to_uint(), static_cast<unsigned int> (63)),
                          std::min(B.to_uint() + other.B.to_uint(), static_cast<unsigned int> (31)));
    return *p;
   }
   //Method to get every different pixels
   Pixel change() {
     unsigned int tR = R, tG = G, tB = B;
     if(R + 1 < 31)
       tR++;
     else if (G + 1 < 63){
       tG++;
       tR = 0;
     }
     else if (B + 1 < 31){
       tB++;
       tR = tG = 0;
     }
     else
       tR = tG = tB = 0;
     return Pixel(tR, tG, tB);
   }

   friend ostream& operator << ( ostream& o, const Pixel& P ) {
      o << "{ R:" << P.R << ", G:" << P.G << ", B:" << P.B << "}" ;
      return o;
   }
};

void sc_trace( sc_trace_file* _f, const Pixel& _foo, const std::string& _s ) {
   sc_trace( _f, _foo.R, _s + "_R" );
   sc_trace( _f, _foo.G, _s + "_G" );
   sc_trace( _f, _foo.B, _s + "_B" );
}

SC_MODULE(sum_sat_method)
{
   sc_in<Pixel>  a;
   sc_in<Pixel>  b;
   sc_out<Pixel> c;

   SC_CTOR(sum_sat_method)
   {
      SC_METHOD(sum);
      sensitive << a << b ;
      dont_initialize();
   }

   void sum()
   {
     Pixel pa = a.read();
     Pixel pb = b.read();
     c = pa + pb;
   }
};

SC_MODULE(sum_sat_thread)
{
   sc_in<Pixel>  a;
   sc_in<Pixel>  b;
   sc_out<Pixel> c;

   SC_CTOR(sum_sat_thread)
   {
      SC_THREAD(sum);
      sensitive << a << b ;
      dont_initialize();
   }

   void sum()
   {
     for(;;){
       Pixel pa = a.read();
       Pixel pb = b.read();
       c = pa + pb;
       wait();
     }
   }
};

SC_MODULE(pixel_change_method)
{
   sc_in<bool>  ck;
   sc_in<bool> rst;
   sc_out<Pixel> out;

   SC_CTOR(pixel_change_method)
   {
      SC_METHOD(generate);
      sensitive << ck.pos();
      async_reset_signal_is(rst, true);
      dont_initialize();
   }

   void generate()
   {
     if (rst) {
       out = Pixel();
     }
     else {
       Pixel tmp = out;
       out = tmp.change();
     }
   }
};

SC_MODULE(pixel_change_thread)
{
   sc_in<bool>  ck;
   sc_in<bool> rst;
   sc_out<Pixel> out;

   SC_CTOR(pixel_change_thread)
   {
      SC_THREAD(generate);
      sensitive << ck.pos();
      async_reset_signal_is(rst, true);
      dont_initialize();
   }

   void generate()
   {
     out = Pixel();
     wait();
     for(;;) {
       Pixel tmp = out;
       out = tmp.change();
       wait();
     }
   }
};

SC_MODULE(pixel_change_cthread)
{
   sc_in<bool>  ck;
   sc_in<bool> rst;
   sc_out<Pixel> out;

   SC_CTOR(pixel_change_cthread)
   {
      SC_CTHREAD(generate, ck.pos());
      async_reset_signal_is(rst, true);
   }

   void generate()
   {
     out = Pixel();
     wait();
     for(;;) {
       Pixel tmp = out;
       out = tmp.change();
       wait();
     }
   }
};

int sc_main(int argc, char * argv[])
{
   sc_trace_file *trace_f;
   trace_f = sc_create_vcd_trace_file ("simu_events");
   trace_f->set_time_unit(1,SC_NS);

   sc_clock ck("ck",10,SC_NS);
   sc_trace(trace_f, ck, "ck");
   sc_signal<bool> rst("rst");
   sc_trace(trace_f, rst, "rst");
   //Pixel generation out signals
   sc_signal<Pixel> pcm_out("pcm_out");
   sc_signal<Pixel> pct_out("pct_out");
   sc_signal<Pixel> pcct_out("pcct_out");

   //Pixel saturated sum test
   sc_signal<Pixel> p1("p1");
   p1 = Pixel(17, 63, 31);
   sc_signal<Pixel> p2("p2");
   p2 = Pixel(1, 5, 3);
   sc_signal<Pixel> p3("p3");
   sc_signal<Pixel> p4("p4");
   //Using SC_METHOD
   sum_sat_method ssm("ssm");
   ssm.a(p1);
   ssm.b(pct_out);
   ssm.c(p3);
   sc_trace(trace_f, p1, "ssm.a");
   sc_trace(trace_f, pct_out, "ssm.b");
   sc_trace(trace_f, p3, "ssm.c");
   //Using SC_THREAD
   sum_sat_thread sst("sst");
   sst.a(pcm_out);
   sst.b(p2);
   sst.c(p4);
   sc_trace(trace_f, pcm_out, "sst.a");
   sc_trace(trace_f, p2, "sst.b");
   sc_trace(trace_f, p4, "sst.c");
   //Pixel generation
   //Using SC_METHOD
   pixel_change_method pcm("pcm");
   pcm.ck(ck);
   pcm.rst(rst);
   pcm.out(pcm_out);
   sc_trace(trace_f, pcm_out, "pcm_out");
   //Using SC_THREAD
   pixel_change_thread pct("pct");
   pct.ck(ck);
   pct.rst(rst);
   pct.out(pct_out);
   sc_trace(trace_f, pct_out, "pct_out");
   //Using SC_CTHREAD
   pixel_change_cthread pcct("pcct");
   pcct.ck(ck);
   pcct.rst(rst);
   pcct.out(pcct_out);
   sc_trace(trace_f, pcct_out, "pcct_out");

   //Initial reset
   rst = true;
   sc_start(10,SC_NS);
   rst = false;
   //Simulation
   for(int i = 0; i <= 200; i++)
    sc_start(10,SC_NS);

   return 0;
}
