#include <iostream>
#include <vector>
#include <regex>

using namespace std;

string::size_type KMP(const string& S, int begin, const string& pattern);

vector<size_t> prefix_function (string s);

void FU(size_t** D, size_t V);

int main()
{
    std::string str = "aabcdeffgjklnn";
    std::string str1 = "a*n";
    // std::string str = "abcde";
    // std::string str1 = "a*d";

    while (str1.find("?*") != std::string::npos)
        str1.replace(str1.find("?*"), 2,"*");
    while (str1.find("*?") != std::string::npos)
        str1.replace(str1.find("*?"), 2,"*");
    while (str1.find("**") != std::string::npos)
        str1.replace(str1.find("**"), 2,"*");
    
    std::vector<string> words;
    std::vector<string> delimiters;
    std::string newWords;
    std::string newDelimiter;
    std::string mask = str1;
    for (int k = 0; k <= mask.size(); ++k)
    {
        char i = mask[k];
        if (i == '*')
        {
            delimiters.push_back("*");
            if (newWords.size() != 0)
            {
                words.push_back(newWords);
                newWords = "";
            }
        } else if (i == '?') {
            newDelimiter += i;
            if (newWords.size() != 0)
            {
                words.push_back(newWords);
                newWords = "";
            }
        } else if (i == '\0')
        {
            words.push_back(newWords);
        } else {
            if (newDelimiter.size() != 0)
            {
                delimiters.push_back(newDelimiter);
                newDelimiter = "";
            }
            newWords += i;
        }
    }

    for (auto i : words)
        std::cout << i << '\n';
    for (auto i : delimiters)
        std::cout << i << '\n';

    std::vector<std::vector<size_t> *> positionVector;
    for (auto i : words)
    {
        vector<size_t> vec;
        
        size_t pos = 0;
        while (true)
        {
            pos = str.find(i, pos);
            if (pos == std::string::npos)
                break;
            std::cout << pos;
            vec.push_back(pos);
            ++pos;
        }

        std::vector<size_t> *newPositionVector = new std::vector<size_t>;
        for (vector<size_t>::iterator j = vec.begin(); j < vec.end(); ++j)
        {
            if (*j == i.size())
            {
                std::cout << *j << " eqwr ";
                newPositionVector->push_back(distance(vec.begin(), j));
            }
        }
        if (newPositionVector->empty())
            {
                std::cout << "NULL";
                // return 0;
            }
        positionVector.push_back(newPositionVector);
    }

    for (auto i : positionVector)
    {
        for (auto j : *i)
        {
            std::cout << j << " ";
        }
        std::cout << '\n';
    }

    // vector<size_t> vec = prefix_function(str1 + str);
    // for (auto i : vec)
    // {
    //     cout << i;
    // }
    size_t matrix_size = 0;
    for (auto i : positionVector)
        matrix_size += i->size();

    size_t matrix[matrix_size][matrix_size];
    for (size_t i = 0; i < matrix_size; ++i)
    {
        for (size_t j = 0; j < matrix_size; ++j)
        {
            matrix[i][j] = 0;
        }
    }

    vector<std::string>::iterator maskIterator = delimiters.begin();
    vector<std::string>::iterator wordIterator = words.begin();
    size_t pos_x(0), pos_y((*positionVector.begin())->size());
    for (vector<vector<size_t> *>::iterator i = positionVector.begin(); i < positionVector.end() - 1 ; ++i)
    {
        for (vector<size_t>::iterator j = (*i)->begin(); j < (*i)->end(); ++j )
        {
            for (vector<size_t>::iterator k = (*(i+1))->begin(); k < (*(i+1))->end(); ++k)
            {
                if (*maskIterator == "*")
                {
                    if (*k >= *j + wordIterator->size())
                    {
                        matrix[pos_x][pos_y] = matrix[pos_y][pos_x] = 1;
                    }
                } else {
                    std::cout << "qwer " << *k << " " << *j  << " " <<  maskIterator->size()  << '\n';
                    if ((*k - *j - wordIterator->size()) == maskIterator->size())
                    {
                        matrix[pos_x][pos_y] = matrix[pos_y][pos_x] = 1;
                    }
                }
                ++pos_y;
            }

            ++pos_x;
        }
        ++maskIterator;
        ++wordIterator;
    }

    for (size_t i = 0; i < matrix_size; ++i)
    {
        for (size_t j = 0; j < matrix_size; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    for (size_t i=0; i<matrix_size; i++) matrix[i][i]=0;
 
    for (size_t k=0; k<matrix_size; k++)
        for (size_t i=0; i<matrix_size; i++)
            for (size_t j=0; j<matrix_size; j++)
                if (matrix[i][k] && matrix[k][j] && i!=j)
                    if (matrix[i][k]+matrix[k][j]<matrix[i][j] || matrix[i][j]==0)
                        matrix[i][j]=matrix[i][k]+matrix[k][j];
                    
    for (size_t i=0; i<matrix_size; i++)
    {
        for (size_t j=0; j<matrix_size; j++) cout<<matrix[i][j]<<" ";
        cout<<endl;
    }   

}

vector<size_t> prefix_function (string s) 
{
    size_t n =  s.length();
    vector<size_t> pi(n); // в i-м элементе (его индекс i-1) количество совпавших символов в начале и конце для подстроки длины i. 
			 // p[0]=0 всегда, p[1]=1, если начинается с двух одинаковых 
    for (size_t i=1; i<n; ++i) 
    {
       // ищем, какой префикс-суффикс можно расширить
        size_t j = pi[i-1]; // длина предыдущего префикса-суффикса, возможно нулевая
        while ((j > 0) && (s[i] != s[j])) // этот нельзя расширить,
            j = pi[j-1];   // берем длину меньшего префикса-суффикса

        if (s[i] == s[j]) 
            ++j;  // расширяем найденный (возможно пустой) префикс-суффикс
        pi[i] = j;
     }
     return pi;
} 

string::size_type KMP(const string& S, int begin, const string& pattern) 
{
	vector<int> pf (pattern.length());

	pf[0] = 0;
	for (int k = 0, i = 1; i < pattern.length(); ++i) 
	{
		while ((k > 0) && (pattern[i] != pattern[k]))
			k = pf[k-1];

		if (pattern[i] == pattern[k])
			k++;

		pf[i] = k;
	}

	for (int k = 0, i = begin; i < S.length(); ++i) 
	{
		while ((k > 0) && (pattern[k] != S[i]))
			k = pf[k-1];

		if (pattern[k] == S[i])
			k++;

		if (k == pattern.length())
			return (i - pattern.length() + 1); //либо продолжаем поиск следующих вхождений
	}

	return (string::npos);
}

void FU(size_t** D, size_t V)
{
int k;
for (size_t i=0; i<V; i++) D[i][i]=0;
 
for (size_t k=0; k<V; k++)
for (size_t i=0; i<V; i++)
for (size_t j=0; j<V; j++)
if (D[i][k] && D[k][j] && i!=j)
if (D[i][k]+D[k][j]<D[i][j] || D[i][j]==0)
D[i][j]=D[i][k]+D[k][j];
 
for (size_t i=0; i<V; i++)
{
for (size_t j=0; j<V; j++) cout<<D[i][j]<<"\t";
cout<<endl;
}
}