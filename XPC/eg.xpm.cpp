#include <iostream>
#include <string>

// linear interpolation helper
float lerp(float min, float max, float t) {
    return min + t * (max - min);
}

int main() {
    while (true) {
        float a, b, t;

        std::cout << "Enter a number: ";
        if (!(std::cin >> a)) return 1;

        std::cout << "Enter another number: ";
        if (!(std::cin >> b)) return 1;

        std::cout << "Enter a value between 0 and 1: ";
        if (!(std::cin >> t)) return 1;

        if (t < 0.0f || t > 1.0f) {
            std::cout << "Error: t must be between 0 and 1.\n";
            return 1;
        }

        float result = lerp(a, b, t);
        std::cout << "a and b interpolated with t = " << t
                  << " is: " << result << "\n";

        std::cout << "Do you want to try again? (y/n): ";
        std::string ans;
        std::cin >> ans;
        if (ans != "y" && ans != "Y")
            break;
    }

    return 0;
}