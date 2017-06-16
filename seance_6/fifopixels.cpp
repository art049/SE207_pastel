#include <systemc.h>
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
void incr(Pixel & p){
   if(++p.R == 0)
      if(++p.G == 0) p.B++;
}

SC_MODULE(Producer) {
   sc_in<bool>  ck;
   sc_fifo_out<Pixel> out;
   Pixel p;
   int n;
   void loop() {
      while(1){
         for(int i = 0; i < 100 ; i++ ){
            out.write(p);
            incr(p);
            wait();
         }
         n = rand()%33;
         if(n) wait(n);
     }
   }
   SC_CTOR(Producer) { SC_CTHREAD(loop, ck.pos()); }
};
SC_MODULE(Consumer) {
   sc_in<bool>  ck;
   sc_fifo_in<Pixel> in;
   Pixel p;
   int sum;
   void loop() {
      wait();
      for(;;){
         p = in.read();
         sum = p.R + p.G + p.B;
         cout << "Lecture : " << p << "@ " << sc_time_stamp() << endl;
         wait((int)(sum%2 ? 3: 2));
      }
   }
   SC_CTOR(Consumer) { SC_CTHREAD(loop, ck.pos()); }
};

int sc_main(int, char **) {
   sc_clock ck("ck",1,SC_NS);
   sc_fifo<Pixel> fifo(10);

   Producer prod("prod");
   prod.ck(ck);
   prod.out(fifo);

   Consumer cons("cons");
   cons.ck(ck);
   cons.in(fifo);

   sc_start(300,SC_NS);
   cout
      << "contenu de la fifo @" << sc_time_stamp() << endl
      << fifo << endl;

   sc_start(10,SC_US);
   cout
      << "contenu de la fifo @" << sc_time_stamp() << endl
      << fifo << endl;

   return 0;
}
