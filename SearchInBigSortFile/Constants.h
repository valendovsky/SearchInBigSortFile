#ifndef CONSTANTS_H
#define CONSTANTS_H


// Псевдонимы используемых типов данных
// Тип массива для адресации (от 0 до 18 446 744 073 709 551 615)
using adress_t = unsigned long long int;


// Название файлов
// Файл настроек
const char* g_settings("Settings.txt");

// Файл искомых позиций
const char* g_inSearch("In.txt");

// Файл базы для поиска совпадений
const char* g_inData("Data.txt");

// Файл для сохранения найденных позиций
const char* g_outSearch("Out.txt");


// Константы для работы с массивами
// Количество позиций в файле настроек
const int g_lineSettings(2);

// Максимальная длина строки файла настроек (long long int)
const int g_sizeSettigs(20);

// Структура позиций для поиска (кратность 4: 1 - родительская позиция, 3 - дочерние позиции)
const int g_amountPosition(4);

// Максимальная длина строки одной позиции
const int g_sizeLine(100);

// Кратность прогрессии адресации
const int g_readProgress(10000000); // 10kk

// Кратность прогрессии поиска
const int g_searchProgress(1000);

#endif
