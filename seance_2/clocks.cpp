#include <systemc.h>

int sc_main(int argc, char * argv[]) {
   sc_trace_file *trace_f;
   trace_f = sc_create_vcd_trace_file ("simu_clocks");
   trace_f->set_time_unit(1,SC_NS);
   // une horloge de période 10ns
   sc_clock ck1("ck1",10,SC_NS);
   sc_trace(trace_f, ck1, "ck1");
   sc_time T(25,SC_NS);
   // une horloge de période T
   sc_clock ck2("ck2",T);
   sc_trace(trace_f, ck2, "ck2");
   // une horloge de période 10ns avec un rapport cyclique de 25%
   // qui commence à l'instant 150ns par un front descendant
   sc_clock ck3("ck3",10,SC_NS,0.25,150,SC_NS,false);
   sc_trace(trace_f, ck3, "ck3");
   double D = 0.4;
   sc_time ST(200,SC_NS);
   bool first_edge = true;
   // une horloge de période T avec un rapport cyclique D
   // qui commence à l'instant ST par un front montant
   sc_clock ck4("ck4",T,D,ST,first_edge);
   sc_trace(trace_f, ck4, "ck4");
   for(int i = 0; i <= 20; i++)
      sc_start(10,SC_NS);

   sc_close_vcd_trace_file(trace_f);
   return 0;
}
