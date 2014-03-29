#include <iostream>
#include <limits>
#include <iomanip>
int main()
{
    long double x = 1.123456789L;
    std::cout << "default precision: "  << x << '\n'
              << "max precision: " << std::setprecision(std::numeric_limits<long double>::digits10) << x << '\n';
}
