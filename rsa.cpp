#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>

using namespace std;
typedef unsigned int uint;

class RSA
{
  private:
    const uint gcd(uint a, uint b)
    {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    const uint extEuclid(uint x, uint y, int *d, int *k)
    {
        uint sa = 1, ta = 0, sb = 0, tb = 1, q, tempA, tempB;
        int a = x, b = y;
        while (b > 0)
        {
            q = a / b;
            tempA = a;
            a = b;
            b = tempA - q * b;
            tempA = sa;
            tempB = ta;
            sa = sb;
            ta = tb;
            sb = tempA - q * sb;
            tb = tempB - q * tb;
        }
        *d = sa;
        *k = ta;
        return a;
    }

    uint Exp_n_Mod(int base, int exp, uint n)
    {
        while (base < 0)
            base += n;
        if (exp < 0)
        {
            int temp = 0;
            extEuclid(base, n, &base, &temp);
            return Exp_n_Mod(base, -1 * exp, n);
        }
        uint res = 1;
        base = base % n;
        while (exp > 0)
        {
            if (exp & 1)
                res = (res * base) % n;
            exp = exp >> 1;
            base = (base * base) % n;
        }
        return res;
    }

  public:
    void genkeys_and_n(int &e, int &d, int &n)
    {
        uint p = 239, q = 251; // Two Large primes
        n = p * q;
        uint phi = (p - 1) * (q - 1); // Euler Totient
        uint pubKey;
        do
        {
            pubKey = rand() % phi;
        } while (gcd(pubKey, phi) != 1);
        e = pubKey;

        int privKey, k;
        extEuclid(pubKey, phi, &privKey, &k);
        d = privKey;
    }

    vector<uint> rsa_encrypt(string msg, int pubKey, int rem)
    {
        vector<uint> crypt_arr(msg.size());
        for (int i = 0; i < msg.size(); i++)
        {
            uint temp = Exp_n_Mod((uint)msg[i], pubKey, rem);
            crypt_arr.at(i) = temp;
        }
        return crypt_arr;
    }

    string rsa_decrypt(vector<uint> crypt_arr, int privKey, int rem)
    {
        string dec_msg = "";
        int len = crypt_arr.size();
        for (int i = 0; i < len; i++)
        {
            dec_msg += (char)(Exp_n_Mod(crypt_arr[i], privKey, rem));
        }
        return dec_msg;
    }

    void write_to_file(const string &fname, string &data)
    {
        ofstream file(fname);
        if (file.is_open())
        {
            for (int i = 0; i < data.size(); i++)
            {
                file << data.at(i);
            }
        }
        file.close();
    }

    void write_to_cryptic_file(const string &fname, vector<uint> &data)
    {
        ofstream file(fname);
        if (file.is_open())
        {
            for (int i = 0; i < data.size(); i++)
            {
                file << data.at(i);
                file << " ";
            }
        }
        file.close();
    }

    string read_from_file(const string &fname)
    {
        string word;
        string msg = "";
        vector<uint> vec;
        ifstream file(fname);
        if (file.is_open())
        {
            while (file >> word)
            {
                msg += word;
                msg += " ";
            }
        }
        file.close();
        return msg;
    }

    vector<uint> read_from_cryptic_file(const string &fname)
    {
        string word;
        vector<uint> vec;
        ifstream file(fname);
        if (file.is_open())
        {
            while (file >> word)
            {
                vec.push_back(stoi(word));
            }
        }
        file.close();
        return vec;
    }
};

int main()
{
    srand(time(NULL));
    int public_key, private_key, n;
    public_key = private_key = 0;
    RSA r1;
    r1.genkeys_and_n(public_key, private_key, n);
    // string msg = "Finally this Worked yay!!!";
    // r1.write_to_file("plaintxt.txt", msg);
    string read_data = r1.read_from_file("plaintxt.txt");
    vector<uint> vec = r1.rsa_encrypt(read_data, public_key, n);
    r1.write_to_cryptic_file("cryptfile.txt", vec);
    vector<uint> vec1 = r1.read_from_cryptic_file("cryptfile.txt");
    string dec_msg = r1.rsa_decrypt(vec1, private_key, n);
    r1.write_to_file("decrypt.txt", dec_msg);
}