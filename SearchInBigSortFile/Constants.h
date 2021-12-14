#ifndef CONSTANTS_H
#define CONSTANTS_H


// ���������� ������������ ����� ������
// ��� ������� ��� ��������� (�� 0 �� 18 446 744 073 709 551 615)
using adress_t = unsigned long long int;


// �������� ������
// ���� ��������
const char* g_settings("Settings.txt");

// ���� ������� �������
const char* g_inSearch("In.txt");

// ���� ���� ��� ������ ����������
const char* g_inData("Data.txt");

// ���� ��� ���������� ��������� �������
const char* g_outSearch("Out.txt");


// ��������� ��� ������ � ���������
// ���������� ������� � ����� ��������
const int g_lineSettings(2);

// ������������ ����� ������ ����� �������� (long long int)
const int g_sizeSettigs(20);

// ��������� ������� ��� ������ (��������� 4: 1 - ������������ �������, 3 - �������� �������)
const int g_amountPosition(4);

// ������������ ����� ������ ����� �������
const int g_sizeLine(100);

// ��������� ���������� ���������
const int g_readProgress(10000000); // 10kk

// ��������� ���������� ������
const int g_searchProgress(1000);

#endif
