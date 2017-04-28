#include <systemc.h>

int sc_main(int argc, char * argv[]) {
  sc_logic a, b, c;
  bool d, e;
  a = 1;
  b = 'Z';
  c = a ^ b;
  d = false;
  e = true;
  cout << (a ^ b) << endl;
  cout << (a & b) << endl;
  cout << (a | e) << endl;
  cout << (a & d) << endl;
  return 0;
}
