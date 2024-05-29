// RSA encryption

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
using namespace std;
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
// функція знаходження простих чисел
vector<int> get_p_q()
{
    vector<int> numbers;
    bool flag = true;
    for (int i = 2; i < 9974; i++)
    {
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                flag = false;
                break;
            }
        }
        if (flag == true)
        {
            numbers.push_back(i);
        }
        flag = true;
    }
    return numbers;
}
// функція знаходження відкрита експонента
int get_e(int f)
{
    vector<int> numbers_for_e;
    for (int i = 2; i < 100; i++)
    {
        for (int j = 2; j < i; j++)
        {
            if (i % j == 0)
            {
                break;
            }
        }
        numbers_for_e.push_back(i);
    }
    while (true)
    {
        int num = numbers_for_e[rand() % numbers_for_e.size()];
        if (gcd(num, f) == 1)
        {
            return num;
        }
    }
}
// функція обчислення піднесення до степення і модулювання
long long pow_mod(long long x, long long d, long long n)
{
    if (d == 0)
        return 1 % n;
    if (d % 2 == 0)
    {
        long long y = pow_mod(x, d / 2, n);
        return (y * y) % n;
    }
    else
    {
        long long y = pow_mod(x, d - 1, n);
        return (x * y) % n;
    }
}
// функція шифрування
vector<int> encryption(string letter, int e, int n)
{
    vector<int> c;
    ofstream encryptFile("encrypted_text.txt"); // Відкриття файлу для запису тексту у файл
    cout << "Encrypted text: ";
    for (int i = 0; i < letter.length(); i++)
    {
        char x = letter.at(i);
        long long cripted = pow_mod(int(x), e, n);
        char text_cripted = (cripted % 94) + 33;
        encryptFile << text_cripted;
        cout << text_cripted;
        c.push_back(cripted);
    }
    encryptFile.close(); // Закриття файлу
    return c;
}

// функція дешифрування
vector<char> decryption(vector<int> text_back, int d, int n)
{
    vector<int> result;
    vector<char> final_result;
    for (int i = 0; i < text_back.size(); i++)
    {
        long long x = text_back[i];
        final_result.push_back(pow_mod(x, d, n));
    }
    return final_result;
}

int main()
{
    srand(time(nullptr));
    // змінні для роботи програми
    int p, q, n, f_n, e, d;
    string text;
    vector<int> encrypted_text;
    // вектор у який записуються всі прості числа від 1 до 1997
    vector<int> p_q = get_p_q();

    // прості числа p і q
    p = p_q[rand() % p_q.size()];
    q = p_q[rand() % p_q.size()];
    // добуток простих чисел
    n = p * q;
    // функція Ейлера
    f_n = (p - 1) * (q - 1);
    // відкрита експонента
    e = get_e(f_n);

    // секретна експонента
    d = 1;
    // підбір закритої експоненти за допомогою циклу
    while (d * e % f_n != 1)
    {
        d++;
    }
    // збереження відкритого і закритого ключа
    vector<int> open_key = {e, n};
    vector<int> secret_key = {d, n};

    ifstream inputFile("input.txt"); // Відкриття файлу для читання
    if (!inputFile)
    {
        cerr << "Error couldn't open file!" << endl;
        return 1;
    }
    getline(inputFile, text);
    cout << "Text for encryption: " << text << endl;
    inputFile.close(); // Закриття файлу

    // шифрування повідомлення
    vector<int> cript = encryption(text, e, n);

    // дешифрування повідомлення
    vector<char> decript = decryption(cript, d, n);
    cout << endl;
    cout << "Decryption text: ";
    ofstream decryptFile("decryption_text.txt"); // Відкриття файлу для запису тексту у файл
    if (!inputFile)
    {
        cerr << "Error couldn't open file!" << endl;
        return 1;
    }

    // запис кожного дешифрованого символу у файл
    for (int i = 0; i < decript.size(); i++)
    {
        cout << decript[i];
        decryptFile << decript[i];
    }
    decryptFile.close();
    return 0;
}
