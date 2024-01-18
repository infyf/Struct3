#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <cstring>
#include <cstdio> 
#include <cstdlib>
#include <Windows.h> 
#pragma warning(disable: 4996)
using namespace std;

int binFileCreation() {//функція яка створює бінарний файл на основі текстовго файлу

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_CTYPE, "ukr");

	const int l_name = 35; // довжини поля ПІБ
	// структура для зберігання відомостей про одного працівника
	struct {
		// довжина поля задана з урахуванням нуль-символу
		char name[l_name + 1];
		int birth_year;
		int pay;
	}Sotr;
	//////////// Крок 1. Відкриття файлів ////////////
	FILE* fin, * fout; // вказівники на файли 
	fin = fopen("LR5/db_otd-kadr.txt ", "r");
	if (fin == NULL) {
		puts("Помилка відкриття файлу db_otd-kadr.txt!");
		return 1;
	}

	// створення нового файлу db_otd-kadr.bin 
	fout = fopen("LR5/db_otd-kadr.txt ", "wb");
	if (fout == NULL) {
		puts("Помилка відкриття файлу db_otd-kadr.bin!");
		return 1;
	}
	//////////// Крок 2. Запис у бінарний файл ////////////
	// читаємо файл, поки не зустрінеться кінець файлу
	while (!feof(fin)) {
		fgets(Sotr.name, l_name, fin); /*Читаємо рядок з текстового файлу і записуємо в структуру
										читаємо інформацію з текстового файлу, перетворюємо її 
										в числову форму і записуємо в структуру : */
		fscanf_s(fin, "%i%i\n", &Sotr.birth_year, &Sotr.pay);
		// налагодження
		printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
		// вивід у бінарний файл
		fwrite(&Sotr, sizeof(Sotr), 1, fout);
	}
	fclose(fout); fclose(fin);
	puts("Бінарний файл записано!");
	return 0;
}

int main() {
	binFileCreation();//Виклик функції яка створює бінарний файл на основі текстовго файлу
	
	const int l_name = 35; // довжини поля ПІБ
	// структура для відомостей про працівника
	struct {
		char name[l_name + 1];
		int birth_year;
		int pay;
	} Sotr;

	//////////// Крок 1. Відкриття файлу ////////////
	FILE* fout; // вказівник на файл
	/* відкриття файлу db_otd-kadr.bin, r + - читання і запис, b - бінарний режим:*/
	fout = fopen("LR5/db_otd-kadr.txt ", "r + b");
	if (fout == NULL) {
		puts("Помилка відкриття файлу db_otd-kadr.bin!");
		return 1;
	}
	///// Крок 2. Визначення кількості записів у файлі /////
	fseek(fout, 0, SEEK_END); // перейти в кінець файлу
	//поточна позиція в байтах, виміряна від початку файлу
	int n_record = ftell(fout) / sizeof(Sotr);
	// контрольне виведення
	cout << "Кількість працівників:" << n_record << endl;
	//////////// Крок 3. Оновлення ////////////
	int num; // номер запису, вводить користувач
	int y = 0; // допоміжна змінна для перевірки введення
	puts("Введіть номер працівника (для виходу з програми введіть -1):");
	while (y != 1) {
		y = scanf("%i", &num); /* якщо введення успішне, то y = 1, якщо ні, то у = 0 */
		fflush(stdin); // функція fflush очищає вміст буфера
		if (num != -1 && (y == 0 || num < 0 || num >= n_record)) {
			puts("Введіть номер співробітника:");
			y = 0;
		}
		else if (num == -1) break;
		// установка поточної позиції у файлі
		fseek(fout, num * sizeof(Sotr), SEEK_SET);
		// зчитування запису в структуру Sotr
		fread(&Sotr, sizeof(Sotr), 1, fout);
		// контрольний вивід
		printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
		// введення нового окладу
		cout << "Новий оклад - 1 " << endl 
			<< "Нове ім'я - 2" << endl 
			<< "Повна зміна - 3" << endl;//Надаємо користувачу можливість вибору дій над обранім об'єктом
		int sw;
		cin >> sw;
		
		string buff_name;
		int sotr1, sotr2;
		
		switch (sw)// Створюємо варіанти вибору дій
		{
		case 1:
			sotr1 = Sotr.pay;// Буфер, що запам'ятовує існуюче значення Sotr.pay
			cout << "Введіть новий оклад(новий окдад не повинен перевищувати або бути меншим ніж попередній на 50%): ";
			scanf("%i", &Sotr.pay); // Запис нового окладу
			sotr2 = Sotr.pay;// Буфер, що запам'ятовує нове значення Sotr.pay
			if (sotr2 < (0.5 * sotr1) || sotr2 >(1.5 * sotr1))// Обмеженння для нового окладу;
			{
				Sotr.pay = sotr1; // Не змінюємо оклад, оскільки користувач не виконав умову
				cout << "Недопустиме значення(новий окдад більше або менше за існуючий на 50%)" << endl;
				y = 0;
				cout << "Введіть номер працівника (для завершення введіть -1): ";
			}
			else
			{
				// установка позиції у файлі на початок зчитаного запису:
				fseek(fout, num * sizeof(Sotr), SEEK_SET);
				// запис в файл даних зі структури Sotr
				fwrite(&Sotr, sizeof(Sotr), 1, fout);
				// вивід результату на екран
				printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
				// повернення для введення нового номера працівника
				y = 0;
				cout << "Введіть номер працівника (для завершення введіть -1): ";
			}
			break;
		case 2:
			cin.ignore(INT_MAX, '\n');//Ігноруємо попередні дані вводу;
			cout << "Введіть нове имі'я: " << endl;
			getline(cin, buff_name, '\n');//Записуємо в буфер введене значення;
			
			strcpy(Sotr.name, buff_name.c_str());//Копіюємо дані з буфера, повертаючи його у форматі const char*, в Sotr.name
			// установка позиції у файлі на початок зчитаного запису:
			
			fseek(fout, num * sizeof(Sotr), SEEK_SET);
			// запис в файл даних зі структури Sotr
			
			fwrite(&Sotr, sizeof(Sotr), 1, fout);
			// вивід результату на екран
			
			printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
			// повернення для введення нового номера працівника
			y = 0;
			
			puts("Введіть номер працівника (для завершення введіть -1):");
			break;
		case 3:
			cout << "Повне зміна інформації. Кожне поле редагується по черзі!";
			cin.ignore(INT_MAX, '\n');
			// Запис нового імені:
			cout << "Введіть нове имі'я: " << endl;// Введення нового імені
			getline(cin, buff_name, '\n');
			strcpy(Sotr.name, buff_name.c_str());
			
			cout << "Введіть рік народження:  " << endl;// Введення новго року народження
			cin >> Sotr.birth_year;

			sotr1 = Sotr.pay;// Буфер, що запам'ятовує існуюче значення Sotr.pay
			cout << "Введіть новий оклад(новий окдад більше або менше за існуючий на 50%): ";//Введення нового окладу
			cin >> Sotr.pay;
			
			sotr2 = Sotr.pay;// Буфер, що запам'ятовує нове значення Sotr.pay
			
			if (sotr2 < (0.5 * sotr1) || sotr2 >(1.5 * sotr1))//Встановлення обмеженння для нового окладу;
			{
				Sotr.pay = sotr1;
				cout << "Недопустиме значення(новий окдад більше або менше за існуючий на 50%)" << endl;
				y = 0;
				cout << "Введіть номер працівника (для виходу з програми введіть -1): ";
			}
			else
			{
				// установка позиції у файлі на початок зчитаного запису:
				fseek(fout, num * sizeof(Sotr), SEEK_SET);
				// запис в файл даних зі структури Sotr
				fwrite(&Sotr, sizeof(Sotr), 1, fout);
				// вивід результату на екран
				printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
				// повернення для введення нового номера працівника
				y = 0;
				cout << "Введіть номер працівника (для виходу з програми введіть -1): ";
			}
			y = 0;
			break;
		}
		sw = 0;
	}
	fclose(fout);
	puts("Ви завершили редагування інформацію о працівниках!");
	return 0;
}
