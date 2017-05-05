#include <systemc.h>

// un type utilisateur
struct Pixel {
   sc_uint<5> R;
   sc_uint<6> G;
   sc_uint<5> B;
   Pixel(sc_uint<5> _R =0, sc_uint<6> _G=0, sc_uint<5> _B=0): R(_R), G(_G), B(_B) { }

   bool operator == (const Pixel &other) {
      return (R == other.R) && (G == other.G) && (B == other.B);
   }
   // On doit pouvoir imprimer la valeur d'un objet de ce type
   // l'opérateur << est un opérateur de la classe std::ostream
   friend ostream& operator << ( ostream& o, const Pixel& P ) {
      o << "{ R:" << P.R << ", G:" << P.G << ", B:" << P.B << "}" ;
      return o;
   }
};

// surcharge de la fonction sc_trace pour le type utilisateur
void sc_trace( sc_trace_file* _f, const Pixel& _foo, const std::string& _s ) {
   sc_trace( _f, _foo.R, _s + "_R" );
   sc_trace( _f, _foo.G, _s + "_G" );
   sc_trace( _f, _foo.B, _s + "_B" );
}
void print_pixel_state(const Pixel p){
   cout << "--> @ " << sc_time_stamp() << " P = " << p << endl;
}
void incr(int i, Pixel & p){
   if(i%3 == 0)
     p.R++;
   else if(i%3 == 1)
     p.G++;
   else
     p.B++;
}
// Le test
int sc_main (int argc, char * argv[])
{
   sc_trace_file *trace_f;
   trace_f = sc_create_vcd_trace_file ("simu_pixel");
   trace_f->set_time_unit(1,SC_NS);
   sc_signal<Pixel> p;
   sc_trace(trace_f, p.read(), "pixel");
   Pixel tmp;
   print_pixel_state(p.read());
   for(int i = 0; i <= 20; i++){
      incr(i, tmp);
      p = tmp;
      sc_start(10,SC_NS);
      print_pixel_state(p);
   }
   sc_close_vcd_trace_file(trace_f);
   return 0;
}
