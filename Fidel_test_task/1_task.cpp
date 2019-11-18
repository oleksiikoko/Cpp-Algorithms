#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std::chrono; 

double fRand(double fMin, double fMax);

int main()
{
    auto start = high_resolution_clock::now(); 
    std::ofstream file("random_float.txt");

    if (file.is_open())
    {
        for (int i = 0; i < 125000000; ++i)
        {
            file << fRand(__DBL_MIN__, __DBL_MAX__) << '\n';
        }
        file.close();
    }

    std::ifstream file_read("random_float.txt");
    
    int file_name = 0;
    std::string digit_line;
    while (file_read.good())
    {
        std::vector<double> temp_vector;
        for (int i = 0; i < 10000000; ++i)
        {
            if (std::getline(file_read, digit_line))
            {
                std::istringstream sin(digit_line);
                double digit;
                sin >> digit;
                temp_vector.push_back(digit);
            }
        }

        std::ofstream tempFile(std::to_string(file_name)); 
        std::sort(temp_vector.begin(), temp_vector.end());
        for (auto i : temp_vector)
        {
            tempFile << i << '\n';
        }
        tempFile.close();
        ++file_name;
    }

    std::vector<std::ifstream *> file_vector;
    for (int i = 0; i < file_name; ++i)
    {
        std::ifstream *tempFile = new std::ifstream(std::to_string(i));
        file_vector.push_back(tempFile);
    }
    
    std::vector<double> first_elem_file;
    for (std::vector<std::ifstream *>::iterator i = file_vector.begin(); i < file_vector.end(); ++i)
    {
        double temp;
        if (std::getline(*(*i), digit_line))
        {
            std::istringstream sin(digit_line);
            double digit;
            sin >> digit;
            first_elem_file.push_back(digit);
        }
    }

    std::ofstream output("output.txt");
    while (true) {
        std::vector<double>::iterator minElem = std::min_element(first_elem_file.begin(), first_elem_file.end()) ;
        output << *minElem << '\n';

        int pos = std::distance(first_elem_file.begin(), minElem);
        if (std::getline(*file_vector.at(pos), digit_line))
        {
            std::istringstream sin(digit_line);
            double digit;
            sin >> digit;
            first_elem_file.at(pos) = digit;
        } else {
            delete *(file_vector.begin() + pos);
            file_vector.erase(file_vector.begin() + pos);
            first_elem_file.erase(first_elem_file.begin() + pos);
        }
        if (first_elem_file.empty())
        break;
    }
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    std::cout << duration.count() << std::endl; 

}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}