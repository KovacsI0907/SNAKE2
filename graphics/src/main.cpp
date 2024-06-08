#include <iostream>
#include "module1/module1.h"
#include "module2/module2.h"

int main() {
    int a = 3;
    int b = 4;

    int sum = add(a, b);
    int product = multiply(a, b);

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;

    return 0;
}
