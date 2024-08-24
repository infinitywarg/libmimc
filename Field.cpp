#include <iostream>

using namespace std;

class Field {
private:
    unsigned int element;
    unsigned int modulus;

    // Function to compute the modular inverse using the Extended Euclidean Algorithm
    static unsigned int modInverse(unsigned int a, unsigned int m) {
        int m0 = m, t, q;
        int x0 = 0, x1 = 1;

        if (m == 1) return 0;

        // Apply extended Euclidean Algorithm
        while (a > 1) {
            // q is quotient
            q = a / m;
            t = m;

            // m is remainder now, process same as Euclid's algo
            m = a % m;
            a = t;

            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }

        // Make x1 positive
        if (x1 < 0) x1 += m0;

        return x1;
    }

    // Exponentiation by Squaring
    unsigned int expMod(unsigned int base, unsigned int exp, unsigned int mod) const {
        unsigned int result = 1;
        base = base % mod;

        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * base) % mod;
            }
            exp = exp >> 1; // exp = exp / 2
            base = (base * base) % mod;
        }

        return result;
    }

public:
    // Constructor to initialize element and modulus
    Field(unsigned int el, unsigned int mod) : element(el % mod), modulus(mod) {
        if (modulus == 0) {
            throw invalid_argument("Modulus must be a positive integer.");
        }
    }

    // Addition modulo
    Field add(const Field& other) const {
        if (modulus != other.modulus) {
            throw invalid_argument("Moduli must be the same for addition.");
        }
        return {(element + other.element) % modulus, modulus};
    }

    // Subtraction modulo
    Field sub(const Field& other) const {
        if (modulus != other.modulus) {
            throw invalid_argument("Moduli must be the same for subtraction.");
        }
        return {((element + modulus) - other.element) % modulus, modulus};
    }

    // Multiplication modulo
    Field mul(const Field& other) const {
        if (modulus != other.modulus) {
            throw invalid_argument("Moduli must be the same for multiplication.");
        }
        return {(element * other.element) % modulus, modulus};
    }

    // Division modulo (using modular inverse)
    Field div(const Field& other) const {
        if (modulus != other.modulus) {
            throw invalid_argument("Moduli must be the same for division.");
        }
        unsigned int inverse = modInverse(other.element, modulus);
        return {(element * inverse) % modulus, modulus};
    }


    // Exponentiation
    Field pow(const Field& exponent) const {
        if (exponent.modulus != modulus) {
            throw invalid_argument("Exponent modulus must be the same as the base modulus.");
        }
        return {expMod(element, exponent.element, modulus), modulus};
    }

    // Print the field element as "element mod modulus"
    void print() const {
        cout << element << " mod " << modulus << endl;
    }
};