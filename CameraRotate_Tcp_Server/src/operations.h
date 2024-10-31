#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string.h>
#include <sstream>
#include <vector>
#include <iostream>



class Operations
{
public:
    static std::vector<std::string> split(std::string s, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (pos_start));
        return res;
    }

   static std::vector<std::string> split (const std::string &s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss (s);
        std::string item;

        while (getline (ss, item, delim)) {
            result.push_back (item);
        }

        return result;
    }

   static void example()
   {
       std::string str = "adsf-+qwret-+nvfkbdsj-+orthdfjgh-+dfjrleih";
       std::string delimiter = "-+";
       std::vector<std::string> v = split (str, delimiter);

       for (auto i : v) std::cout << i << std::endl;

       std::string str2 = "32a324dsf 3424qwer 234234poui 243fdgh";
       std::vector<std::string> v2 = split (str2, ' ');

       for (auto i : v2) std::cout << i << std::endl;
   }

};

#endif // OPERATIONS_H
