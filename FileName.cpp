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
	cout << "\n\n1. Сформувати список(стек)" << endl;
	cout << "2. Оновити файл" << endl;
	cout << "3. Отримати дані з файлу" << endl;
	cout << "4. Почистити список" << endl;
	cout << "5. Вставити новий елемент в список" << endl;
	cout << "6. Текст і файл" << endl;
	cout << "7. Заміна елемента в файлі" << endl;
	cout << "0. Вихід" << endl;
	cout << endl;
}
void Print(Steck* begin) {
	Steck* print = begin;
	cout << "Поточний стек: ";
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

void InitStack(Steck** begin) { // вказівник на вказівник означає, що цей обєкт буде змінюватись
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
	//побайтовий запис стеку в файл, з адресами кожнного елемент(чило -8 щоб без сміття)
	//while (saveToFile) {
	//	fout.write(reinterpret_cast<const char*>(saveToFile), sizeof(Steck)-8);  
	//	saveToFile = saveToFile->next;
	//}
	// запис конкретно значень стеку в файл 
	while (saveToFile) {
		fout.write(reinterpret_cast<const char*>(&(saveToFile->data)), sizeof(int));  //побайтовий запис стеку в файл
		saveToFile = saveToFile->next;
	}
	fout.close();
}
int FileToStack(ifstream& fin, Steck** begin) {
	fin.open("binarystack.bin", ios::binary);
	if (!fin.is_open()) { cout << "File is not exist!"; _getch(); return -1; }

	// визначити к-кість байтів у файлі
	fin.seekg(0, ios::end); // без першого аргумента працює не вірно
	int fileSize = fin.tellg();  // повертає число позицію в даний момент в файлі 
	fin.seekg(0, ios::beg); // повертає на початок, як було

	if (fileSize == 0) { cout << "Файл порожній" << endl; }
	cout << "В файлі 'binarystack.bin' - " << fileSize << " байта" << endl;

	while (*begin != NULL) { // чистимо стек
		Steck* t = new Steck;
		t = *begin;
		*begin = t->next;
		delete t;
	}

    // допоміжний масив, який записує інфу с файлу, та переда її в стек в зворотньому(правильному) порядку
	int arrNum = fileSize / 4; // взначення к-сті елементів масива з розміру файла(в байтах)
	int* arr = new int[arrNum];
	int el = 0;
	while (!fin.eof()) // поки не досягнемо кінця файла
	{
		if (fin.tellg() == fileSize) { break; } // щоб останнього проходу циклу та в стек не записалось сміттяі
		fin.read(reinterpret_cast<char*>(&(arr[el])), sizeof(int)); // куди записати інфу?(1-й аргумент). скільки байтів зчитати?(2-й аргумент).
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
void Free(Steck** begin) { // звільнення памяті, видалення стеку 
	while (*begin != NULL) {
		Steck* t;
		t = *begin;
		*begin = t->next;
		delete t;
	}
}
void Insert(Steck** begin) {

	// заміна елемента по обраній позиції, якщо такої позиції немає - вставака нового елемента в стек на позицію по порядку 
	int data,pos;
	Steck* ins = new Steck; // новий елемент стеку, якщо елемн. не буде замінено по позиції, то буде створено новий. якщо ні память під еього чиститься 
	cout << "Введіть значення нового елемента: " << endl;
	cin >> data;
	ins->data = data;
	cout << "Введіть позицію в стекі для заміни елемента: " << endl;
	cin >> pos;


	if (*begin == NULL) { // якщо в спискі жодного елементів. створення введеного елемент, він буде першим
		ins->next = NULL;
		*begin = ins;
		return;
	}


	// шукаємо елмент стеку по позиції та міняємо його значення
	Steck* position = *begin;
	int i = 0;
	while (position)
	{
		if (pos == i) { position->data = data; delete ins; return; }
		position = position->next;
		i++;
	}


	// яйкщо обрраної позиції немає - вставка в позицію по порядку 
	Steck* t = *begin; // створюємо допоміжний елемент, який пройде список
	if ((*begin)->data >= ins->data) { // вставка в початок 
		ins->next = t;
		*begin = ins;
		return;
	}
	// пройти весь список і дізнитись куди вставити 
	Steck* t1 = t->next; // елемент точно не буде першим. 
	while (t1)
	{
		if (t->data < ins->data && ins->data <= t1->data) { // треба знайти наступний і попередній (t та t1) і вставити між ними.
			t->next = ins;
			ins->next = t1;
			return;
		}
		// якщо умова не виконана - нічого не буде вставлено
		t = t1;
		t1 = t1->next;
	}
	t->next = ins; // надання останньому елементу 
	ins->next = NULL;
}
void StringFile(ofstream& fout, ifstream& fin) {

	char cha;
	//string str; // string - динамічний масив з char
	char* str1 = new char[100];
	int var = 10;
	double arr[3];

	cout << "Запис в файл:" << endl;
	fout.open("string.txt");
	cin.getline(str1,100); fout << str1; // запис рядка
	//fout.write(str.c_str(), str.length()); // побайтовий запис (c_str() - адреса внутрішнього масиву). до пробіла
	//string line2 = "sdcds sd sd sd sd"; // запис рядка
	//fout.put(); // аналог get 
	fout.close();

	//cout << "Запис НЕ тексту в файл(бінарний, побайтовий):" << endl;
	//fout.open("binary.bin", ios::app | ios::binary);
	//fout.write(reinterpret_cast<const char*>(&var), sizeof(int)); // адреса типу char(1-й аргумент). ромір змінної(2-й аргумент).
	//fout.write(reinterpret_cast<const char*>(arr), sizeof(double) * 3); // запис масива
	////fout.write(reinterpret_cast<const char*>(&myPoint), sizeof(Point)); // запис структури(класа).  екземпляр класа(1-й). назва(2-й).
	//fout.close();

	cout << "Читання з файлу:" << endl;
	fin.open("string.txt");
	while (fin.get(cha)) { cout << cha;} // посимвольне зчитування (get() - читає посимвольно(повертає символ з потоку))
	//while (!fin.eof()) { fin >> str; cout << str << endl; } // зчитує по слову за цикл. eof() повертає тру, якщо кінець. 
	//while (!fin.eof()) { getline(fin, str); cout << str << endl; } // зчитує по рядку за цикл.  
	//while (fin.getline(str1, 100, '\r')) { cout << str1; }; delete[] str1; // всі дані з файлу в масив(3-й параметр - коли припинити зчитувати рядок)
	//while (fin >> cha) { cout << cha; }// пропускає пробіли та переноси рядків, все зліплює
	fin.close();
	

	//cout << "Читання НЕ тексту в файл(бінарний, побайтовий):" << endl;
	//fin.open("binary.bin", ios::binary);
	//fin.read(reinterpret_cast<char*>(&var), sizeof(int)); // куди записати інфу?(1-й аргумент). скільки байтів зчитати?(2-й аргумент).
	//fin.read(reinterpret_cast<char*>(arr), sizeof(double) * 3); // читання масива
	//////fin.read(reinterpret_cast<char*>(&myPoint), sizeof(Point)); 
	//fin.close();
	//// вивід записаних в змінні даних з файлу 
	//cout << "var: " << var << endl;
	//for (size_t i = 0; i < 3; i++){ cout << arr[i] << " "; }

	delete[] str1;
	_getch();

}
void ChangeElemInFile(ofstream& fout, ifstream& fin, int count, Steck* begin) {
	int newElem = 0;
	bool isNumberFound = false;
	int countElementinFile = 0;
	
	// дізнаємось к-сть байтів файла
	fout.open("binarystack.bin", ios::app | ios::binary);
	fout.seekp(0, ios::end); // без першого аргумента працює не вірно
	int fileSize = fout.tellp();  // повертає число позицію в даний момент в файлі 
	fout.seekp(0, ios::beg); // повертає на початок, як було

	// якщо нуль байт - вихід з функції
	if (fileSize == 0) {
		cout << "Файл порожній" << endl;  _getch();
		return; }
	fout.close();


	// створення допоміжного масива і запис елемент з файла в ного
	cout << "Який елемент оновити?" << endl;
	cin >> newElem;
	countElementinFile = fileSize / sizeof(int);
	int* arr = new int[countElementinFile];
	fin.open("binarystack.bin", ios::binary);
	fin.read(reinterpret_cast<char*>(arr), fileSize); // читання масива
    fin.close();

	// читання даних з файла відразу в структуру(стек)
	//fin.open("binarystack.bin", ios::binary);
	//Steck* saveToFile = begin;
	//while (saveToFile) {
	//	fin.read(reinterpret_cast<char*>(saveToFile), sizeof(Steck)-8); // читання структури
	//	saveToFile = saveToFile->next;
	//}
	//fin.close();


	// пошук в масиві обраного елемент для заміни
	for (size_t i = 0; i < countElementinFile; i++){
		if (arr[i] == newElem) {
			cout << " буде замінено на: " << endl;
			cin >> arr[i];
			isNumberFound = true;
			break;
		}
	}

	////// якщо число знайдене - відкриваємо файл для запису, перезаписуэмо дані
if (isNumberFound) {
	fout.open("binarystack.bin", ios::out | ios::binary);
	fout.write(reinterpret_cast<const char*>(arr), fileSize);
	fout.close();
}	else { cout << "Такого елемента не було й не буде!" << endl;  _getch();}

//  видалення допоміжного масиву
delete[] arr;
}






int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

 
	// створення списку(стеком). запис зі стеку в файл. зчитування з файлу в стек.
		Steck* begin = NULL;
		ofstream fout;
		ifstream fin;
		char c;

	do {

		system("cls");
		Menu();
		c = (char)_getch(); // обробка нажаття без Enter 
		switch (c) {
		case '1': system("cls"); InitStack(&begin); Print(begin); break; //  заповнення стеку ранд значеннями
		case '2': system("cls"); StackToFile(fout,begin); Print(begin); break; //  запис стеку в файл
		case '3': system("cls"); FileToStack(fin, &begin); Print(begin); break; // читання з файлу та запис в стек
		case '4': system("cls"); Free(&begin); break; // видалення стеку
		case '5': system("cls"); Insert(&begin); Print(begin); break; // заміна обраного елемента в стекі, вставка по-порядку
		case '6': system("cls"); StringFile(fout, fin); break; // запис рядків в файл та читання 
		case '7': system("cls"); ChangeElemInFile(fout, fin, NumberElemStack(begin), begin); break; // заміна обраного елемент в файлі
		case '0': break;
		default: cout << "\nНевірний пункт меню!\n\n"; Sleep(200);
		}
	} while (c != '0');




	

	return 0;
}