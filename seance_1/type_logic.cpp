#include <systemc.h>

int sc_main(int argc, char * argv[]) {
  sc_logic a, b, c;
  bool d, e;
  a = 1;
  b = 'Z';
  c = a ^ b;
  d = false;
  e = true;
  cout << "(a ^ b) : " << (a ^ b) << endl;
  cout << "(a & b) : " << (a & b) << endl;
  cout << "(a | e) : " << (a | e) << endl;
  cout << "(a & d) : " << (a & d) << endl;
  return 0;
}
