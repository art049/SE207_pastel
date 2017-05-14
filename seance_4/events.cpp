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
     Pixel *p = new Pixel(std::max(R.to_uint() + other.R.to_uint(), static_cast<unsigned int> (31)),
                          std::max(G.to_uint() + other.G.to_uint(), static_cast<unsigned int> (63)),
                          std::max(B.to_uint() + other.B.to_uint(), static_cast<unsigned int> (31)));
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

int sc_main(int argc, char * argv[])
{
   sc_trace_file *trace_f;
   trace_f = sc_create_vcd_trace_file ("simu_events");
   trace_f->set_time_unit(1,SC_NS);

   sc_clock ck("ck",10,SC_NS);
   sc_trace(trace_f, ck, "ck");
   sc_signal<bool> rst("rst");
   sc_trace(trace_f, rst, "rst");
   //Pixel sat sum
   sc_signal<Pixel> p1("p1");
   p1 = Pixel(31, 63, 17);
   sc_signal<Pixel> p2("p2");
   p2 = Pixel(1, 5, 3);
   sc_signal<Pixel> p3("p3");
   sum_sat_method sum("sum");
   sum.a(p1);
   sum.b(p2);
   sum.c(p3);
   sc_trace(trace_f, sum.a, "sum.a");
   sc_trace(trace_f, sum.b, "sum.b");
   sc_trace(trace_f, sum.c, "sum.c");

   //Pixel generation
   pixel_change_method pcm("pcm");
   sc_signal<Pixel> pcm_out("pcm_out");
   pcm.ck(ck);
   pcm.rst(rst);
   pcm.out(pcm_out);
   sc_trace(trace_f, pcm_out, "pcm_out");

   //Initial reset
   rst = true;
   sc_start(10,SC_NS);
   rst = false;
   //Sim
   for(int i = 0; i <= 200; i++)
    sc_start(10,SC_NS);
   return 0;
}
