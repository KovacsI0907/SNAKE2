#include <iostream>
#include "module1/module1.h"
#include "module2/module2.h"
#include "matek.h"
using namespace std;
int main() {
    int a = 3;
    int b = 4;

    int sum = add(a, b);
    int product = multiply(a, b);

    cout << "Sum: " << sum << endl;
    cout << "Product: " << product << endl;
    Mat4 m1 = Mat4();
    cout << m1 << endl;

    return 0;
}
