#include <systemc.h>

int sc_main(int argc, char * argv[]) {
  sc_int<10> x(-40);
  sc_uint<7> y(25);
  sc_lv<5> z = "00101";
  cout << "x : " << x << endl;
  cout << "y : " << y << endl;
  cout << "x + y : "<< x + y.to_int() << endl;
  cout << "x - y : "<< x - y.to_int() << endl;
  cout << "y to lv (10 bits) : " << sc_lv<10>(y) << endl;
  cout << "sc_int value of sc_lv : " << z << " : " << sc_int<5>(z) << endl;
  return 0;
}
