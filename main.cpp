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
	cout << "������� ���������: ";
	string *message = new string();
	do
	{
		getline(cin, *message);
		if (!checkMessage(*message))
			cout << "�������� ����, ���������� �����" << endl;
	} while (!checkMessage(*message));
	cout << "�������� �������� (1 - ������������, 2 - ������������): ";
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
			cout << "������ �� �������!";
		else
		{
			cout << "������ � �������� � ������� " << result + 1 << endl;
			cout << "������������ ������: " << *message << endl;
		}
		break;
	}
	delete message;
	message = nullptr;
	return 0;
}

void encode(string &message)
{
	// ������� ����������� ���
	for (int i = 0; i < message.length(); ++i)
	{
		if (floor(log2(i + 1)) == log2(i + 1))
			message.insert(i, "0");
	}
	// ��������� �������� ����������� ���
	for (int i = 0; i <= floor(log2(message.length())); ++i)
	{
		int bitsSum = 0;
		bool switcher = false;
		for (int j = 0; j < message.length(); ++j)
		{
			// switcher - true ��������, ��� ��������� ���� ��������
			if ((j + 1) % (int)pow(2, i) == 0)
				switcher = !switcher;
			if (switcher)
			{
				bitsSum += (int)message[j] - '0';
			}
		}
		// ���� ����� ��������, �� ������ 1 � ����������� ���
		if (bitsSum % 2 != 0)
			message[pow(2, i) - 1] = '1';
		bitsSum = 0;
	}
}

int decode(string &message)
{
	int errorIndex = 0;
	bool foundError = false;
	// �������� �������� ����������� ���
	for (int i = 0; i <= floor(log2(message.length())); ++i)
	{
		int bitsSum = 0;
		bool switcher = false;
		for (int j = 0; j < message.length(); ++j)
		{
			// switcher - true ��������, ��� ��������� ���� ��������
			if ((j + 1) % (int)pow(2, i) == 0)
				switcher = !switcher;
			if (switcher)
			{
				bitsSum += (int)message[j] - '0';
			}
		}
		// ����������� ��� ��� �������� ����� ��������� ��� = 1, �������������, ������ ��� ��������� ���� ������ � ����������� ����� ���� ������ �����, ���� ��� �� ����������
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
		// ���������� ������
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
