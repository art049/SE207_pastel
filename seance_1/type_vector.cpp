#include <systemc.h>

int sc_main(int argc, char * argv[]) {
  sc_lv<5> x = "001XZ";
  sc_lv<3> y = false;
  sc_lv<2> z = true;
  z[1] = 1;
  cout << "x : " << x << endl;
  cout << "y : " << y << endl;
  cout << "z : " << z << endl;
  cout << "x^x : " << (x ^ x) << endl;
  cout << "x or reduce : " << x.or_reduce() << endl;
  cout << "x and reduce : " << x.and_reduce() << endl;
  cout << "y xor reduce : " << y.xor_reduce() << endl;
  cout << "Binary : "<< z << " : "<< "Decimal (uint): " << z.to_uint() << endl;
  cout << "Binary : "<< z << " : "<< "Decimal (int): " << z.to_int() << endl;
  cout << "Initial : " << x << endl;
  (y,z) = x;
  x = (z,y);
  cout << "Switched : " << x << endl;
  return 0;
}
