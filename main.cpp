#include <iostream>
#include <string>
#include <cmath>
using namespace std;

void encode(string&);
int decode(string&);
bool checkMessage(string&);

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Введите сообщение: ";
	string *message = new string();
	do
	{
		getline(cin, *message);
		if (!checkMessage(*message))
			cout << "Неверный ввод, попробуйте снова" << endl;
	} while (!checkMessage(*message));
	cout << "Выберите действие (1 - закодировать, 2 - декодировать): ";
	int choice = 0;
	cin >> choice;
	switch (choice)
	{
	case 1:
		encode(*message);
		cout << *message << endl;
		break;
	case 2:
		int result = decode(*message);
		if (result == -1)
			cout << "Ошибок не найдено!";
		else
		{
			cout << "Ошибка в элементе с номером " << result + 1 << endl;
			cout << "Исправленная строка: " << *message << endl;
		}
		break;
	}
	delete message;
	message = nullptr;
	return 0;
}

void encode(string &message)
{
	// Вставка контрольных бит
	for (int i = 0; i < message.length(); ++i)
	{
		if (floor(log2(i + 1)) == log2(i + 1))
			message.insert(i, "0");
	}
	// Установка значений контрольных бит
	for (int i = 0; i <= floor(log2(message.length())); ++i)
	{
		int bitsSum = 0;
		bool switcher = false;
		for (int j = 0; j < message.length(); ++j)
		{
			// switcher - true означает, что следующие биты зависимы
			if ((j + 1) % (int)pow(2, i) == 0)
				switcher = !switcher;
			if (switcher)
			{
				bitsSum += (int)message[j] - '0';
			}
		}
		// Если сумма нечётная, то ставим 1 в контрольный бит
		if (bitsSum % 2 != 0)
			message[pow(2, i) - 1] = '1';
		bitsSum = 0;
	}
}

int decode(string &message)
{
	int errorIndex = 0;
	bool foundError = false;
	// Проверка значений контрольных бит
	for (int i = 0; i <= floor(log2(message.length())); ++i)
	{
		int bitsSum = 0;
		bool switcher = false;
		for (int j = 0; j < message.length(); ++j)
		{
			// switcher - true означает, что следующие биты зависимы
			if ((j + 1) % (int)pow(2, i) == 0)
				switcher = !switcher;
			if (switcher)
			{
				bitsSum += (int)message[j] - '0';
			}
		}
		// Контрольный бит при нечётной сумме зависимых бит = 1, следовательно, теперь все зависимые биты вместе с контрольным битом дают чётные суммы, если они не повреждены
		if (bitsSum % 2 != 0)
		{
			errorIndex += pow(2, i);
			foundError = true;
		}
		bitsSum = 0;
	}
	if (!foundError)
		return -1;
	else
	{
		// Исправляем ошибку
		message[errorIndex - 1] == '0' ? message[errorIndex - 1] = '1' : message[errorIndex - 1] = '0';
		return errorIndex - 1;
	}
}

bool checkMessage(string &message)
{
	for (int i = 0; i < message.length(); ++i)
		if (message[i] != '1' && message[i] != '0')
			return false;
	return true;
}
