#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstring>
#include <fstream>
#include <ctime> 
#include <random>
#include <string>

using namespace std;

struct Steck
{
	int data;
	Steck* next;
};


void Menu() {
	cout << "\n\n1. ���������� ������(����)" << endl;
	cout << "2. ������� ����" << endl;
	cout << "3. �������� ��� � �����" << endl;
	cout << "4. ��������� ������" << endl;
	cout << "5. �������� ����� ������� � ������" << endl;
	cout << "6. ����� � ����" << endl;
	cout << "7. ����� �������� � ����" << endl;
	cout << "0. �����" << endl;
	cout << endl;
}
void Print(Steck* begin) {
	Steck* print = begin;
	cout << "�������� ����: ";
	while (print)
	{
		cout << " | " << (print)->data;;
		print = print->next;
	}
	cout << endl;
	_getch();
}
int NumberElemStack(Steck* begin) {
	Steck* t = begin;
	int count = 0;
	if (t != NULL) {
		while (t)
		{
			t = t->next;
			count++;
		}
	}
	return count;
}

void InitStack(Steck** begin) { // �������� �� �������� ������, �� ��� ���� ���� ����������
	srand(time(NULL));

	while(*begin != NULL) {
		Steck* t = new Steck;
			t = *begin;
			*begin = t->next;
			delete t;
		}

		for (int i(0); i < 5; i++) {
			Steck* t = new Steck;
			t->data = rand() % 20;
			t->next = *begin;
			*begin = t;
		}

}
void StackToFile(ofstream& fout, Steck* begin){
	fout.open("binarystack.bin",ios::binary);
	Steck* saveToFile = begin;
	//���������� ����� ����� � ����, � �������� �������� �������(���� -8 ��� ��� �����)
	//while (saveToFile) {
	//	fout.write(reinterpret_cast<const char*>(saveToFile), sizeof(Steck)-8);  
	//	saveToFile = saveToFile->next;
	//}
	// ����� ��������� ������� ����� � ���� 
	while (saveToFile) {
		fout.write(reinterpret_cast<const char*>(&(saveToFile->data)), sizeof(int));  //���������� ����� ����� � ����
		saveToFile = saveToFile->next;
	}
	fout.close();
}
int FileToStack(ifstream& fin, Steck** begin) {
	fin.open("binarystack.bin", ios::binary);
	if (!fin.is_open()) { cout << "File is not exist!"; _getch(); return -1; }

	// ��������� �-���� ����� � ����
	fin.seekg(0, ios::end); // ��� ������� ��������� ������ �� ����
	int fileSize = fin.tellg();  // ������� ����� ������� � ����� ������ � ���� 
	fin.seekg(0, ios::beg); // ������� �� �������, �� ����

	if (fileSize == 0) { cout << "���� �������" << endl; }
	cout << "� ���� 'binarystack.bin' - " << fileSize << " �����" << endl;

	while (*begin != NULL) { // ������� ����
		Steck* t = new Steck;
		t = *begin;
		*begin = t->next;
		delete t;
	}

    // ��������� �����, ���� ������ ���� � �����, �� ������ �� � ���� � �����������(�����������) �������
	int arrNum = fileSize / 4; // ��������� �-�� �������� ������ � ������ �����(� ������)
	int* arr = new int[arrNum];
	int el = 0;
	while (!fin.eof()) // ���� �� ��������� ���� �����
	{
		if (fin.tellg() == fileSize) { break; } // ��� ���������� ������� ����� �� � ���� �� ���������� ������
		fin.read(reinterpret_cast<char*>(&(arr[el])), sizeof(int)); // ���� �������� ����?(1-� ��������). ������ ����� �������?(2-� ��������).
		el++;
	}
	for (int i = arrNum-1; i >= 0; i--) 
	{
		Steck* t = new Steck;
		t->data = arr[i];
	    t->next = *begin;
	    *begin = t;
	}

	delete[] arr;
	fin.close();
	_getch();
}
void Free(Steck** begin) { // ��������� �����, ��������� ����� 
	while (*begin != NULL) {
		Steck* t;
		t = *begin;
		*begin = t->next;
		delete t;
	}
}
void Insert(Steck** begin) {

	// ����� �������� �� ������ �������, ���� ���� ������� ���� - �������� ������ �������� � ���� �� ������� �� ������� 
	int data,pos;
	Steck* ins = new Steck; // ����� ������� �����, ���� �����. �� ���� ������� �� �������, �� ���� �������� �����. ���� � ������ �� ����� ��������� 
	cout << "������ �������� ������ ��������: " << endl;
	cin >> data;
	ins->data = data;
	cout << "������ ������� � ���� ��� ����� ��������: " << endl;
	cin >> pos;


	if (*begin == NULL) { // ���� � ����� ������� ��������. ��������� ��������� �������, �� ���� ������
		ins->next = NULL;
		*begin = ins;
		return;
	}


	// ������ ������ ����� �� ������� �� ������ ���� ��������
	Steck* position = *begin;
	int i = 0;
	while (position)
	{
		if (pos == i) { position->data = data; delete ins; return; }
		position = position->next;
		i++;
	}


	// ����� ������� ������� ���� - ������� � ������� �� ������� 
	Steck* t = *begin; // ��������� ��������� �������, ���� ������ ������
	if ((*begin)->data >= ins->data) { // ������� � ������� 
		ins->next = t;
		*begin = ins;
		return;
	}
	// ������ ���� ������ � �������� ���� �������� 
	Steck* t1 = t->next; // ������� ����� �� ���� ������. 
	while (t1)
	{
		if (t->data < ins->data && ins->data <= t1->data) { // ����� ������ ��������� � ��������� (t �� t1) � �������� �� ����.
			t->next = ins;
			ins->next = t1;
			return;
		}
		// ���� ����� �� �������� - ����� �� ���� ���������
		t = t1;
		t1 = t1->next;
	}
	t->next = ins; // ������� ���������� �������� 
	ins->next = NULL;
}
void StringFile(ofstream& fout, ifstream& fin) {

	char cha;
	//string str; // string - ��������� ����� � char
	char* str1 = new char[100];
	int var = 10;
	double arr[3];

	cout << "����� � ����:" << endl;
	fout.open("string.txt");
	cin.getline(str1,100); fout << str1; // ����� �����
	//fout.write(str.c_str(), str.length()); // ���������� ����� (c_str() - ������ ����������� ������). �� ������
	//string line2 = "sdcds sd sd sd sd"; // ����� �����
	//fout.put(); // ������ get 
	fout.close();

	//cout << "����� �� ������ � ����(�������, ����������):" << endl;
	//fout.open("binary.bin", ios::app | ios::binary);
	//fout.write(reinterpret_cast<const char*>(&var), sizeof(int)); // ������ ���� char(1-� ��������). ���� �����(2-� ��������).
	//fout.write(reinterpret_cast<const char*>(arr), sizeof(double) * 3); // ����� ������
	////fout.write(reinterpret_cast<const char*>(&myPoint), sizeof(Point)); // ����� ���������(�����).  ��������� �����(1-�). �����(2-�).
	//fout.close();

	cout << "������� � �����:" << endl;
	fin.open("string.txt");
	while (fin.get(cha)) { cout << cha;} // ����������� ���������� (get() - ���� �����������(������� ������ � ������))
	//while (!fin.eof()) { fin >> str; cout << str << endl; } // ����� �� ����� �� ����. eof() ������� ���, ���� �����. 
	//while (!fin.eof()) { getline(fin, str); cout << str << endl; } // ����� �� ����� �� ����.  
	//while (fin.getline(str1, 100, '\r')) { cout << str1; }; delete[] str1; // �� ��� � ����� � �����(3-� �������� - ���� ��������� ��������� �����)
	//while (fin >> cha) { cout << cha; }// �������� ������ �� �������� �����, ��� ������
	fin.close();
	

	//cout << "������� �� ������ � ����(�������, ����������):" << endl;
	//fin.open("binary.bin", ios::binary);
	//fin.read(reinterpret_cast<char*>(&var), sizeof(int)); // ���� �������� ����?(1-� ��������). ������ ����� �������?(2-� ��������).
	//fin.read(reinterpret_cast<char*>(arr), sizeof(double) * 3); // ������� ������
	//////fin.read(reinterpret_cast<char*>(&myPoint), sizeof(Point)); 
	//fin.close();
	//// ���� ��������� � ���� ����� � ����� 
	//cout << "var: " << var << endl;
	//for (size_t i = 0; i < 3; i++){ cout << arr[i] << " "; }

	delete[] str1;
	_getch();

}
void ChangeElemInFile(ofstream& fout, ifstream& fin, int count, Steck* begin) {
	int newElem = 0;
	bool isNumberFound = false;
	int countElementinFile = 0;
	
	// �������� �-��� ����� �����
	fout.open("binarystack.bin", ios::app | ios::binary);
	fout.seekp(0, ios::end); // ��� ������� ��������� ������ �� ����
	int fileSize = fout.tellp();  // ������� ����� ������� � ����� ������ � ���� 
	fout.seekp(0, ios::beg); // ������� �� �������, �� ����

	// ���� ���� ���� - ����� � �������
	if (fileSize == 0) {
		cout << "���� �������" << endl;  _getch();
		return; }
	fout.close();


	// ��������� ���������� ������ � ����� ������� � ����� � ����
	cout << "���� ������� �������?" << endl;
	cin >> newElem;
	countElementinFile = fileSize / sizeof(int);
	int* arr = new int[countElementinFile];
	fin.open("binarystack.bin", ios::binary);
	fin.read(reinterpret_cast<char*>(arr), fileSize); // ������� ������
    fin.close();

	// ������� ����� � ����� ������ � ���������(����)
	//fin.open("binarystack.bin", ios::binary);
	//Steck* saveToFile = begin;
	//while (saveToFile) {
	//	fin.read(reinterpret_cast<char*>(saveToFile), sizeof(Steck)-8); // ������� ���������
	//	saveToFile = saveToFile->next;
	//}
	//fin.close();


	// ����� � ����� �������� ������� ��� �����
	for (size_t i = 0; i < countElementinFile; i++){
		if (arr[i] == newElem) {
			cout << " ���� ������� ��: " << endl;
			cin >> arr[i];
			isNumberFound = true;
			break;
		}
	}

	////// ���� ����� �������� - ��������� ���� ��� ������, ������������� ���
if (isNumberFound) {
	fout.open("binarystack.bin", ios::out | ios::binary);
	fout.write(reinterpret_cast<const char*>(arr), fileSize);
	fout.close();
}	else { cout << "������ �������� �� ���� � �� ����!" << endl;  _getch();}

//  ��������� ���������� ������
delete[] arr;
}






int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

 
	// ��������� ������(������). ����� � ����� � ����. ���������� � ����� � ����.
		Steck* begin = NULL;
		ofstream fout;
		ifstream fin;
		char c;

	do {

		system("cls");
		Menu();
		c = (char)_getch(); // ������� ������� ��� Enter 
		switch (c) {
		case '1': system("cls"); InitStack(&begin); Print(begin); break; //  ���������� ����� ���� ����������
		case '2': system("cls"); StackToFile(fout,begin); Print(begin); break; //  ����� ����� � ����
		case '3': system("cls"); FileToStack(fin, &begin); Print(begin); break; // ������� � ����� �� ����� � ����
		case '4': system("cls"); Free(&begin); break; // ��������� �����
		case '5': system("cls"); Insert(&begin); Print(begin); break; // ����� �������� �������� � ����, ������� ��-�������
		case '6': system("cls"); StringFile(fout, fin); break; // ����� ����� � ���� �� ������� 
		case '7': system("cls"); ChangeElemInFile(fout, fin, NumberElemStack(begin), begin); break; // ����� �������� ������� � ����
		case '0': break;
		default: cout << "\n������� ����� ����!\n\n"; Sleep(200);
		}
	} while (c != '0');




	

	return 0;
}