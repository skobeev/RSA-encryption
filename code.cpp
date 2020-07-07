#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>//rand
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstring>//
#include <cmath>
#include <cstdint>//корректная работа типов
#include <boost\multiprecision\cpp_int.hpp>
#include <windows.h>
using namespace std;
using boost::multiprecision::cpp_int;
enum command{NewKey,ConS,ConD,FileS,FileD,Exit};

/*
В программе были сделаны ограничения на q и p от 1 до 100
									 на e до 300
									 */
cpp_int gen()
{
	cpp_int i;
	bool f = false;
	srand(time(NULL));
	while (!f)
	{
		f = true;
		i =2+rand() % 99; // от 2 до 99, попалась 1, поэтому от 2
		//i =rand();
			for (int j = 2;j < i;j++)
			{
				if ((long long int)i%j == 0)
				{
						f = false;
						break;
				}
			}

	}
	return i;
}

void NewKeys(cpp_int &p, cpp_int &q, cpp_int &mulsub, cpp_int &n, cpp_int &e,cpp_int &d)
{
	p = gen();
	q = gen();
	while (q == p)
	{
		q = gen();
	}
	mulsub = (p - 1)*(q - 1);
	n = p*q;
	std::cout << "Генерирование коэффициента p: p=" << p << endl;
	std::cout << "Генерирование коэффициента q: q=" << q << endl;
	std::cout << "Произведение n=p*q: n=" << n << endl;
	std::cout << "(p-1)*(q-1): " << mulsub << endl; // 
	bool f;
	std::cout << endl << "\t\t\t\t\t\tСПИСОК ДОСТУПНЫХ ОТКРЫТЫХ КЛЮЧЕЙ" << endl;
	for (int i = 2;i < 300;i++) // число меньше mulsub
	{
		f = true;
		if (i == 2 && (long long int)mulsub % 2 == 0) // сделал данную проверку, потому что при i=2 не попадем в следующий цикл
		{
			continue; // следующая итерация
		}
		for (int j = 2; j < i;j++) // цикл не проверяет 2. если поставить j<=i, то все простые числа не пройдут проверку
		{
			if (i%j == 0 || ((long long int)mulsub%i) == 0)  // число не простое или не взаимопростое с mulsub
			{
				f = false;
				break;
			}
		}
		if (!f)
			continue;
		std::cout << setw(6) << i;
	}
	std::cout << endl<<endl << "Введите любое значение коэффициента е из таблицы: e=";
	std::cin >> e;
	std::cout << endl;
	bool flag = false;
	while (!flag)
	{
		bool a = false, c = true;
		d =rand();
		if(powm(d*e, 1, mulsub)==1)   // (((long long  int)d*(long long int)e) % (long long int)mulsub == 1) powm(d*e, 1, mulsub);
			a = true;
		//for (int i = 2;i < (long long int)d;i++)
		//{
			//if ((long long int)d%i == 0)
			//{
			//	c = false;
			//	break;
		//	}
	//	}

		if (a && c)
		{
			flag = true;
			break;
		}
		else
			flag = false;
	}
	std::cout << "Генерирование закрытого ключа d: d=" << d << endl;
}
void Shifr(cpp_int &n, cpp_int &e, int  *&mes2,string message)
{
	//string message;
	//std::cout << "Введите сообщение: ";
	//cin.ignore();
	//getline(cin, message);
	const char *mes = message.c_str();
	int a = message.length();
	mes2=new int[a];
	std::cout << "Символы в ASCII: ";
	for (int i = 0;i < (int)message.length();i++)
		std::cout <<(cpp_int)mes[i] << " ";//(long long int)static_cast<int>
	std::cout << endl << endl<<"Зашифрованное сообщение: ";
		for (int i = 0;i < (int)message.length();i++)
		{
			cpp_int a = powm((cpp_int)mes[i], e,n);
			mes2[i] = (int)a;
			cout << mes2[i]<<" ";
		}
		
		cout << endl << "Чтобы сохранить сообщение в файл нажмите 1" << endl << "Чтобы продолжить нажмите любую кнопку" << endl;
		cout << "Команда: ";
		char c;
		cin >> c;
		if (c == '1')
		{
			cout << "Введите название файла: ";
			string file;
			cin.ignore();
			getline(cin, file);
			ofstream fout(file);
			for (int i = 0;i < message.length();i++)
			{
				fout << mes2[i];
				fout << " ";
			}
			fout.close();
		}
		
		
}

void DShifr(cpp_int &n, cpp_int &d, cpp_int *&m, string &message, int &x)
{
	std::istringstream s(message);
	int *i=new int[1024];
	int count=0;
	int c;
	while (s >> c)
	{
		i[count] = c;
		count++;
	}
	cpp_int *mes2 = new  cpp_int[count];
	for (int b = 0;b < count;b++)
	{
	mes2[b] = i[b];
	//std::cout << mes2[b];
	}
	cout << endl;
	m=new  cpp_int [count];
	cout << "Расшифрованное сообщение: ";
	for (int a = 0;a < count;a++)
	{
		m[a] = powm((cpp_int)mes2[a], d, n);
		//setlocale(LC_ALL, "Rus");
		std::cout << (char)m[a];	
	}
	x = count;
	cout << endl;	
}


int main()
{
	SetConsoleCP(1251); //устанавливает кодировку ввода из консоли и из редактора кода; для корректного вывода кириллицы
	SetConsoleOutputCP(1251); //устанавливает кодировку вывода на консоль;
	setlocale(LC_ALL, "Rus");
	int start,x;
	string message;
	cpp_int *m=new  cpp_int[1024];
	const char *mes = message.c_str();
	int  *mes2 = new  int[message.length()];
	cpp_int  p, q, mulsub, e, n, d;
	//NewKeys(p, q, mulsub, e, n, d);
	bool exit = false;
	while(!exit)
	{
		cout << endl << endl;
		cout << "Чтобы сгенирировать ключ введите  0" << endl; //команду NewKey
		cout << "Для шифрования сообщения с консоли введите 1" << endl;//команду ConS
		cout << "Для дешифровки сообщения с консоли введите 2" << endl;//команду ConD
		cout << "Для шифрования сообщения из файла введите 3" << endl;//команду FileS
		cout << "Для дешифровки сообщения из файла введите 4" << endl;//команду FileD
		cout << "Для выхода из программы введите 5" << endl << endl; //Exit
		cout << "Команда: ";
		cin >> start;
		cout << endl;
		switch (start)
		{
			case NewKey:
				cout << endl << endl;
				NewKeys(p, q, mulsub, e, n, d);
				exit = false;
				break;
			case ConS:
				cout << endl << endl;
				std::cout << "Для кодирования введите открытый ключ e: e=";
				std::cin >> e;
				std::cout << "Для кодирования ввдеите коэффициент n: n=";
				std::cin >> n;
				cout << endl;
				std::cout << "Введите сообщение: ";
				cin.ignore();
				getline(cin, message);
				Shifr(n, e, mes2,message);
				exit = false;
				break;
			case ConD:
			{
				cout << endl << endl;
				std::cout << "Введите зашифрованное сообщение: ";
				cin.ignore();
				std::getline(cin, message);
				std::cout << "Для расшифровки сообщения введите закрытый ключ d: d=";
				std::cin >> d;
				std::cout << "Для расщифровки введите коэффициент n: n= ";
				std::cin >> n;
				DShifr(n, d, m, message,x);
				cout << endl;
				cout << endl << "Чтобы сохранить сообщение в файл нажмите 1" << endl << "Чтобы продолжить нажмите любую кнопку" << endl;
				cout << "Команда: ";
				char c;
				cin >> c;
				if (c == '1')
				{
					cout << "Введите название файла: ";
					string file;
					cin.ignore();
					getline(cin, file);
					ofstream fout(file);
					for (int i = 0;i <x;i++)
					{
						fout << (char)m[i];
					}
					fout.close();
				}
				exit = false;
				break;
			}
			case FileS:
			{
				cout << endl << endl;
				cout << "Введите название файла: ";
				cin.ignore();
				getline(cin, message);
				ifstream fin;
				fin.open(message);
				if (!fin.is_open())
				{
					cout << "Файл не открыт";
					exit = false;
					break;
				}
				message = "";
				string s;
				while (!fin.eof())
				{
					fin >> s;
					s += " ";
					message += s;
				}

				cout << endl << "Текст для шифрования: " << message << endl;
				std::cout << "Для кодирования введите открытый ключ e: e=";
				std::cin >> e;
				std::cout << "Для кодирования ввдеите коэффициент n: n=";
				std::cin >> n;
				cout << endl;
				Shifr(n, e, mes2, message);
				exit = false;
				fin.close();
				break;
			}
			case FileD:
			{
				std::cout << "Введите название файла: ";
				cin.ignore();
				getline(cin, message);
				ifstream fin;
				fin.open(message);
				if (!fin.is_open())
				{
					cout << "Файл не открыт";
					exit = false;
					break;
				}
				std::cout << "Для расшифровки сообщения введите закрытый ключ d: d=";
				std::cin >> d;
				std::cout << "Для расщифровки введите коэффициент n: n= ";
				std::cin >> n;
				cout << endl << "Текст для расшифровки: ";
				int s;
				int count=0;
				while (!fin.eof())
				{
					//m = new cpp_int[count];
					fin >> s;
					//c[count] = (cpp_int)s;
					cout << s << " ";
					m[count] = powm((cpp_int)s, d, n);
					count++;
				}
				cout << endl << "Расшифрованный текст: ";
				
				for (int i = 0;i < count;i++)
				{
					cout << (char)m[i];
				}
				cout << endl << "Чтобы сохранить сообщение в файл нажмите 1" << endl << "Чтобы продолжить нажмите любую кнопку" << endl;
				cout << "Команда: ";
				char c;
				cin >> c;
				if (c == '1')
				{
					cout << "Введите название файла: ";
					string file;
					cin.ignore();
					getline(cin, file);
					ofstream fout(file);
					for (int i = 0;i < count;i++)
					{
						fout << (char)m[i];
					}
					fout.close();
				}
				break;
			}
			case Exit: exit = true;
				break;

		}
	}	
	system("pause");
    return 0;
}

