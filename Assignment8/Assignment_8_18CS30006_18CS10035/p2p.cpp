#include <bits/stdc++.h>
using namespace std;

class Base {
  protected:
   int a, b;

  public:
   Base(int _a, int _b) {
      a = _a;
      b = _b;
   }
   void func() { cout << "Inside Parent Class" << endl; }
};

class Derived1 : public Base {
  public:
   Derived1(int a, int b) : Base(a, b) {}

   void func() { cout << "Inside Derived1 Class" << endl; }
};

class Derived2 : public Base {
  public:
   Derived2(int a, int b) : Base(a, b) {}

   void func() { cout << "Inside Derived2 Class" << endl; }
};

// Main function for the program
int main() {
   Base *b;
   Derived1 d1(10, 7);
   Derived2 d2(10, 5);

   // Store the address of Derived1
   b = &d1;

   // Call Derived1 Func.
   b->func();

   // Store the address of Derived2
   b = &d2;

   // Call Derived2 Func.
   b->func();

   return 0;
}