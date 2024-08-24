#include <iostream>
#include <cmath>
#include "Field.cpp"

using namespace std;

class MiMC {
private:
    unsigned int modulus;
    unsigned int e;
    unsigned int r=1;
    unsigned int d;
    unsigned int* round_constants = new unsigned int [r];

    // Function to compute the GCD of two numbers
    static unsigned int compute_gcd(unsigned int a, unsigned int b) {
        while (b != 0) {
            unsigned int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Function to find the smallest e such that gcd(modulus-1, e) = 1
    static unsigned int compute_encryption_exponent(unsigned int p) {
        unsigned int m1 = p - 1;
        for (unsigned int e = 3; e < m1; ++e) {
            if (compute_gcd(m1, e) == 1) {
                return e;
            }
        }
        return m1; // Return modulus-1 if no such e is found (though this should never happen)
    }

    static unsigned int compute_log_base_e(unsigned int p, unsigned int e) {
        if (e <= 1) {
            throw invalid_argument("Base e must be greater than 1.");
        }

        double log2p = log2(p);
        double log2e = log2(e);
        unsigned int result = floor(log2p / log2e);

        return result;
    }

    static unsigned int compute_smallest_factor(unsigned int p, unsigned int e) {
        if (e == 0) {
            throw invalid_argument("Modulus e must be greater than 0.");
        }

        unsigned int p_minus_1 = p - 1;
        unsigned int target = e - 1;

        for (unsigned int z = 0; ; ++z) {
            if ((z * p_minus_1) % e == target) {
                return z;
            }
        }
    }

    static unsigned int compute_decryption_exponent(unsigned int p, unsigned int e) {
        unsigned int z = compute_smallest_factor(p, e);
        unsigned int numerator = 1 + z * (p - 1);

        if (numerator % e != 0) {
            throw logic_error("Numerator is not divisible by e.");
        }

        unsigned int d = numerator / e;
        return d;
    }

public:
    MiMC (unsigned int modulus) {
        if (modulus == 0) {
            throw invalid_argument("Modulus must be a positive integer.");
        }
        this->modulus = modulus;
        this->e = compute_encryption_exponent(modulus);
        this->r= compute_log_base_e(modulus, e);
        this->d= compute_decryption_exponent(modulus, e);

//         create round constants dynamically
        this->round_constants[0] = 0;
        for(int i=1; i<r; i++){
            this->round_constants[i] = rand() % modulus;
        }
//        this->round_constants[0] = 0;
//        this->round_constants[1] = 14;
//        this->round_constants[2] = 23;
    }

    void print() const {
        cout << "MiMC config generated with below parameters" << endl;
        cout << "Prime Field: " << this->modulus << endl;
        cout << "Encryption Exponent: " << this->e << endl;
        cout << "Number of Rounds: " << this->r << endl;
        cout << "Decryption Exponent: " << this->d << endl;
        cout << "Round Constants: " << endl;
        for (size_t i = 0; i < this->r; ++i) {
            cout << "round_constants[" << i << "] = " << this->round_constants[i] << endl;
        }
    }

    Field encrypt(Field plain, Field key) {
        cout << "Encryption Started-----------"<< endl;
        Field exp(this->e, this->modulus);

        cout << "Round 0: ";
        Field cipher = (plain.add(key)).pow(exp); // first round
        cipher.print();

        for(int i=1;i<this->r;i++){
            cout << "Round " << i << ": ";
            Field curr_constant(this->round_constants[i], this->modulus);
            cipher = (cipher.add(key).add(curr_constant)).pow(exp);
            cipher.print();
        }

        cout << "Round " << this->r+1 << ": ";
        cipher = cipher.add(key);
        cipher.print();
        cout << "Encryption Ended-----------"<< endl;
        return cipher;
    }

    Field decrypt(Field cipher, Field key) {
        cout << "Decryption Started-----------"<< endl;

        Field dxp(this->d, this->modulus);

        cout << "Round " << this->r << ":";
        Field plain = cipher.sub(key);
        plain.print();

        for(int i=(this->r)-1;i>0;i--){
            cout << "Round " << i << ":";
            Field curr_constant(this->round_constants[i], this->modulus);
            plain = (plain.pow(dxp)).sub(key.add(curr_constant));
            plain.print();
        }

       cout << "Round 0:";
        plain = (plain.pow(dxp)).sub(key);
        plain.print();
        cout << "Decryption Ended-----------"<< endl;
        return plain;
    }

};
