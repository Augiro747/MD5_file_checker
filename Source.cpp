#include <cstdio>
#include<iostream>
#include <cstring>
#include <Windows.h>
#include <Wincrypt.h>
#include <vector>
#include<string>
#include<fstream>

unsigned char SymbolToHex(unsigned char c)
{
	switch (c)
	{
	case 10:
		c = 'A';
		break;
	case 11:
		c = 'B';
		break;
	case 12:
		c = 'C';
		break;
	case 13:
		c = 'D';
		break;
	case 14:
		c = 'E';
		break;
	case 15:
		c = 'F';
		break;
	default:
		c += '0';
	}
	return c;
}

std::string CharToHex(unsigned char c)
{
	unsigned char l, r;
	std::string result;
	l = c >> 4;
	r = c << 4;
	r = r >> 4;
	result += SymbolToHex(l);
	result += SymbolToHex(r);
	return result;
}

std::string GetMD5HexHash(std::vector<char> buf)
{
	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;
	std::string md5 = "", test = "";
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
	{
		std::cout << "Acquisition of context failed.\n";
		exit(1);
	}
	if (!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		std::cout << "Error during CryptBeginHash!\n";
		exit(1);
	}
	std::string s(buf.begin(), buf.end());
	char* data = &s[0];

	if (!CryptHashData(hHash, (BYTE*)data, strlen(data), 0))
	{
		std::cout << "Error during CryptHashData!\n";
		exit(1);
	}
	DWORD count = 0;
	if (!CryptGetHashParam(hHash, HP_HASHVAL, NULL, &count, 0))
	{
		std::cout << "Error during CryptGetHashParam!\n";
		exit(1);
	}
	unsigned char* hash_value = static_cast<unsigned char*>(malloc(count + 1));
	ZeroMemory(hash_value, count + 1);

	if (!CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)hash_value, &count, 0))
	{
		std::cout << "Error during CryptGetHashParam!\n";
		exit(1);
	}
	// Вывод полученного хеш-значения
	for (unsigned char const* p = hash_value; *p; ++p)
	{
		test += CharToHex(unsigned(*p));
	}
	if (hHash)
		CryptDestroyHash(hHash);
	if (hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	return test;
}

std::string GetFileMD5Hash(std::string input_name)
{
	// Загружаем файл в бинарном виде
	std::ifstream input(input_name, std::ios::binary);

	if (!input.is_open()) 
	{ 
		std::cout << "File does not exist."; 
		exit(1); 
	}
	// Копируем данные в буфер
	std::vector<char> buffer(std::istreambuf_iterator<char>(input), {});
	std::string md5_hash = GetMD5HexHash(buffer);
	return md5_hash;
}

void SaveHashTable(std::string output_name, std::vector<std::string> data)
{
	std::ofstream output(output_name);
	for (int i = 0; i < data.size(); i++)
	{
		output << data[i] << std::endl;
	}
}

std::vector<std::string> ReadHashTable(std::string output_name)
{
	std::vector<std::string> data;
	std::string temp;
	std::ifstream input(output_name);
	while (getline(input, temp))
	{
		data.push_back(temp);
	}
	return data;
}

int CheckHash(std::string input_name, std::string output_name)
{
	input_name += ' ';
	std::vector<std::string> data = ReadHashTable(output_name);
	int n;
	for (int i = 0; i < data.size(); i++)
	{
		n = data[i].find("\""+input_name+"\"");
		if (std::string::npos != n)
		{
			n = data[i].find("'"+GetFileMD5Hash(input_name) + "'");
			if (std::string::npos != n)
			{
				return 1;
			}
			else
			{
				char answer;
				std::cout << "File was changed, rewrite hash?(y/n)" << std::endl;
				std::cin >> answer;
				if (answer == 'y')
				{
					data[i] = "\"" + input_name + "\" " + "'" + GetFileMD5Hash(input_name) + "'";
					SaveHashTable(output_name, data);
				}
				return -1;
			}
		}
	}
	data.push_back("\"" + input_name + "\" " + "'"+GetFileMD5Hash(input_name)+"'");
	SaveHashTable(output_name, data);
	return 0;
}

std::string Answer(std::string input_name, std::string output_name)
{
	std::string s;
	switch (CheckHash(input_name, output_name))
	{
	case 0:  s = "File added to hash table!"; break;
	case 1:  s = "File not changed."; break;
	case -1: s = "File was changed!"; break;
	}
	std::cout << s << std::endl;
}

int main(int argc, char* argv[])
{
	std::string output_name = "md5_hash_table.txt";
	std::string input_name = "C:\\Games\\hello.txt";

	if (argc == 1)
	{
		char cont;
		// Программа запустилась без параметров из командной строки, предлагаем ввести их пользователю
		do {
			std::cout << "Input file path:";
			std::getline(std::cin, input_name);
			Answer(input_name, output_name);
			std::cout << "Continue?(y/n)";
			std::cin >> cont;
			std::getline(std::cin, input_name);
		} while (cont != 'n');
	}
	else
	{
		// Присваиваем значениям путей аргументы командной строки
		std::string input_name = argv[1];
		Answer(input_name, output_name);
	}
	return 0;
}
