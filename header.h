
#ifndef C___PROGRAMS_HEADER_H
#define C___PROGRAMS_HEADER_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <random>

std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
std::string myhash;
auto elapsed = (std::chrono::duration<double>(start - end).count());

using std::cout, std::endl, std::cin;
double timer = 0;


void starttimer() {
    start = std::chrono::high_resolution_clock::now();
}

void endtimer() {
    end = std::chrono::high_resolution_clock::now();
    elapsed = (std::chrono::duration<double>(end - start).count());
    timer = timer + elapsed;

}

char decToHexa(int n) {
    char hexaDeciNum;
    int temp = n%16;

    //int temp =std::fmod(n,16);
    if (temp < 10) {
        hexaDeciNum = temp + 48;
    } else {
        hexaDeciNum = temp + 55;
    }

    return hexaDeciNum;
}
int counter=0;
long sum_the_decimal_digits_of_a_value(long double &rootvalue, long digitsum, int end) {



        rootvalue *= pow(10,end);
        digitsum += floor(rootvalue);
        rootvalue -= floor(rootvalue);

    return digitsum;

}




std::string inputhashing(std::string input) {

    starttimer();
    unsigned char *hash = new unsigned char[32];
    for(int i=0; i<32; i++)
    {
        hash[i]=0;
    }
    long digitsum = 0;

    for (char character : input) {
        int ascii = abs((int) character);
        long double rootvalue = std::pow(ascii, 1.0 / 727.0);


        for (int i = 0; i < 32; i++) {

            digitsum = sum_the_decimal_digits_of_a_value(rootvalue, digitsum, 5);

            digitsum=digitsum ^ 1;

            digitsum%=16;
            hash[i] = decToHexa(digitsum + int(hash[i]));


            if (i==7 )
            {
                rootvalue = std::pow(ascii, 1.0/947.0);

            }
            else if(i==15 )
            {
                rootvalue = std::pow(ascii, 1.0/319.0);

            }
            else if(i==23 )
            {
                rootvalue = std::pow(ascii, 1.0/683.0);

            }



        }

    }
    endtimer();
//cout<<std::string(reinterpret_cast<char *>(hash)).substr(0,32)<<endl;
    return std::string(reinterpret_cast<char *>(hash)).substr(0,32);
}

std::string filehashing(std::string file_name) {
    std::ifstream in(file_name);
    unsigned char *hash = new unsigned char[32];
    std::string line;
    starttimer();
    int digitsum = 0;
     char ch;
     while (in >> std::skipws >>ch) {
         int ascii = abs((int)ch);
         long double rootvalue = std::pow(ascii, 1.0 / 319.0);
         int end = 2;
         for (int i = 0; i < 32; i++) {
             digitsum = sum_the_decimal_digits_of_a_value(rootvalue, digitsum, end);
             digitsum %= 16;
             hash[i] = decToHexa(digitsum + int(hash[i]));
           //   end = 3;
             /*   if (i == 7) {
                    rootvalue = std::pow(ascii, 1.0 / 947.0);
                    end = 3;
                }*/
              if (i == 15) {
                   rootvalue = std::pow(ascii, 1.0 / 727.0);
                    end = 2;}


         /* else if (i == 23) {
                    rootvalue = std::pow(ascii, 1.0 / 683.0);
                    end = 3;
                }*/
          }
      }


    endtimer();
      in.close();
   /*
      std::ifstream On(file_name);
     while (std::getline(On, line)) {
       myhash = inputhashing(line);

      }
     cout<<myhash<<endl;
 */
    return reinterpret_cast<char *>(hash);
   // return myhash;
}

auto goodChars = const_cast<char *>("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
const int goodCharsLength = 62;

std::string random_string(size_t length) {
    thread_local static std::mt19937 rg{
            static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
    thread_local static std::uniform_int_distribution<int> pick(0, goodCharsLength - 1);
    auto randchar = []() -> char { return goodChars[pick(rg)]; };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

void test() {
    int same = 0;
    for (int i = 0; i < 1000000; i++) {
        std::string a = inputhashing(random_string(5));
        std::string b = inputhashing(random_string(5));
        if (a.compare(b) == 0) same++;

    }

    cout << same;


}

void test2() {
    int same = 0;
    for (int i = 0; i < 1000000; i++) {
        std::string a = random_string(4);
        std::string b = a;
        a = a + "a";
        b = b + "b";
        a = inputhashing(a);
        b = inputhashing(b);
        if (a.compare(b) == 0) same++;

    }
    cout << same;
}

void menu()
{
    cout << "Press 1 for v0.1\n";
    cout << "Press 2 for v0.2+v0.3\n";

}
#endif //C___PROGRAMS_HEADER_H
