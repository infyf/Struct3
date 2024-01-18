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

int binFileCreation() {//������� ��� ������� ������� ���� �� ����� ��������� �����

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_CTYPE, "ukr");

	const int l_name = 35; // ������� ���� ϲ�
	// ��������� ��� ��������� ��������� ��� ������ ����������
	struct {
		// ������� ���� ������ � ����������� ����-�������
		char name[l_name + 1];
		int birth_year;
		int pay;
	}Sotr;
	//////////// ���� 1. ³������� ����� ////////////
	FILE* fin, * fout; // ��������� �� ����� 
	fin = fopen("LR5/db_otd-kadr.txt ", "r");
	if (fin == NULL) {
		puts("������� �������� ����� db_otd-kadr.txt!");
		return 1;
	}

	// ��������� ������ ����� db_otd-kadr.bin 
	fout = fopen("LR5/db_otd-kadr.txt ", "wb");
	if (fout == NULL) {
		puts("������� �������� ����� db_otd-kadr.bin!");
		return 1;
	}
	//////////// ���� 2. ����� � ������� ���� ////////////
	// ������ ����, ���� �� ����������� ����� �����
	while (!feof(fin)) {
		fgets(Sotr.name, l_name, fin); /*������ ����� � ���������� ����� � �������� � ���������
										������ ���������� � ���������� �����, ������������ �� 
										� ������� ����� � �������� � ��������� : */
		fscanf_s(fin, "%i%i\n", &Sotr.birth_year, &Sotr.pay);
		// ������������
		printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
		// ���� � ������� ����
		fwrite(&Sotr, sizeof(Sotr), 1, fout);
	}
	fclose(fout); fclose(fin);
	puts("�������� ���� ��������!");
	return 0;
}

int main() {
	binFileCreation();//������ ������� ��� ������� ������� ���� �� ����� ��������� �����
	
	const int l_name = 35; // ������� ���� ϲ�
	// ��������� ��� ��������� ��� ����������
	struct {
		char name[l_name + 1];
		int birth_year;
		int pay;
	} Sotr;

	//////////// ���� 1. ³������� ����� ////////////
	FILE* fout; // �������� �� ����
	/* �������� ����� db_otd-kadr.bin, r + - ������� � �����, b - ������� �����:*/
	fout = fopen("LR5/db_otd-kadr.txt ", "r + b");
	if (fout == NULL) {
		puts("������� �������� ����� db_otd-kadr.bin!");
		return 1;
	}
	///// ���� 2. ���������� ������� ������ � ���� /////
	fseek(fout, 0, SEEK_END); // ������� � ����� �����
	//������� ������� � ������, ������� �� ������� �����
	int n_record = ftell(fout) / sizeof(Sotr);
	// ���������� ���������
	cout << "ʳ������ ����������:" << n_record << endl;
	//////////// ���� 3. ��������� ////////////
	int num; // ����� ������, ������� ����������
	int y = 0; // �������� ����� ��� �������� ��������
	puts("������ ����� ���������� (��� ������ � �������� ������ -1):");
	while (y != 1) {
		y = scanf("%i", &num); /* ���� �������� ������, �� y = 1, ���� �, �� � = 0 */
		fflush(stdin); // ������� fflush ����� ���� ������
		if (num != -1 && (y == 0 || num < 0 || num >= n_record)) {
			puts("������ ����� �����������:");
			y = 0;
		}
		else if (num == -1) break;
		// ��������� ������� ������� � ����
		fseek(fout, num * sizeof(Sotr), SEEK_SET);
		// ���������� ������ � ��������� Sotr
		fread(&Sotr, sizeof(Sotr), 1, fout);
		// ����������� ����
		printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
		// �������� ������ ������
		cout << "����� ����� - 1 " << endl 
			<< "���� ��'� - 2" << endl 
			<< "����� ���� - 3" << endl;//������ ����������� ��������� ������ �� ��� ������ ��'�����
		int sw;
		cin >> sw;
		
		string buff_name;
		int sotr1, sotr2;
		
		switch (sw)// ��������� ������� ������ ��
		{
		case 1:
			sotr1 = Sotr.pay;// �����, �� �����'����� ������� �������� Sotr.pay
			cout << "������ ����� �����(����� ����� �� ������� ������������ ��� ���� ������ �� ��������� �� 50%): ";
			scanf("%i", &Sotr.pay); // ����� ������ ������
			sotr2 = Sotr.pay;// �����, �� �����'����� ���� �������� Sotr.pay
			if (sotr2 < (0.5 * sotr1) || sotr2 >(1.5 * sotr1))// ���������� ��� ������ ������;
			{
				Sotr.pay = sotr1; // �� ������� �����, ������� ���������� �� ������� �����
				cout << "����������� ��������(����� ����� ����� ��� ����� �� �������� �� 50%)" << endl;
				y = 0;
				cout << "������ ����� ���������� (��� ���������� ������ -1): ";
			}
			else
			{
				// ��������� ������� � ���� �� ������� ��������� ������:
				fseek(fout, num * sizeof(Sotr), SEEK_SET);
				// ����� � ���� ����� � ��������� Sotr
				fwrite(&Sotr, sizeof(Sotr), 1, fout);
				// ���� ���������� �� �����
				printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
				// ���������� ��� �������� ������ ������ ����������
				y = 0;
				cout << "������ ����� ���������� (��� ���������� ������ -1): ";
			}
			break;
		case 2:
			cin.ignore(INT_MAX, '\n');//�������� �������� ��� �����;
			cout << "������ ���� ��'�: " << endl;
			getline(cin, buff_name, '\n');//�������� � ����� ������� ��������;
			
			strcpy(Sotr.name, buff_name.c_str());//������� ��� � ������, ���������� ���� � ������ const char*, � Sotr.name
			// ��������� ������� � ���� �� ������� ��������� ������:
			
			fseek(fout, num * sizeof(Sotr), SEEK_SET);
			// ����� � ���� ����� � ��������� Sotr
			
			fwrite(&Sotr, sizeof(Sotr), 1, fout);
			// ���� ���������� �� �����
			
			printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
			// ���������� ��� �������� ������ ������ ����������
			y = 0;
			
			puts("������ ����� ���������� (��� ���������� ������ -1):");
			break;
		case 3:
			cout << "����� ���� ����������. ����� ���� ���������� �� ����!";
			cin.ignore(INT_MAX, '\n');
			// ����� ������ ����:
			cout << "������ ���� ��'�: " << endl;// �������� ������ ����
			getline(cin, buff_name, '\n');
			strcpy(Sotr.name, buff_name.c_str());
			
			cout << "������ �� ����������:  " << endl;// �������� ����� ���� ����������
			cin >> Sotr.birth_year;

			sotr1 = Sotr.pay;// �����, �� �����'����� ������� �������� Sotr.pay
			cout << "������ ����� �����(����� ����� ����� ��� ����� �� �������� �� 50%): ";//�������� ������ ������
			cin >> Sotr.pay;
			
			sotr2 = Sotr.pay;// �����, �� �����'����� ���� �������� Sotr.pay
			
			if (sotr2 < (0.5 * sotr1) || sotr2 >(1.5 * sotr1))//������������ ���������� ��� ������ ������;
			{
				Sotr.pay = sotr1;
				cout << "����������� ��������(����� ����� ����� ��� ����� �� �������� �� 50%)" << endl;
				y = 0;
				cout << "������ ����� ���������� (��� ������ � �������� ������ -1): ";
			}
			else
			{
				// ��������� ������� � ���� �� ������� ��������� ������:
				fseek(fout, num * sizeof(Sotr), SEEK_SET);
				// ����� � ���� ����� � ��������� Sotr
				fwrite(&Sotr, sizeof(Sotr), 1, fout);
				// ���� ���������� �� �����
				printf("%s\t%i\t%i\n", Sotr.name, Sotr.birth_year, Sotr.pay);
				// ���������� ��� �������� ������ ������ ����������
				y = 0;
				cout << "������ ����� ���������� (��� ������ � �������� ������ -1): ";
			}
			y = 0;
			break;
		}
		sw = 0;
	}
	fclose(fout);
	puts("�� ��������� ����������� ���������� � �����������!");
	return 0;
}
