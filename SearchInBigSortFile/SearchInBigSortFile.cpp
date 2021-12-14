// SearchInBigSortFile.cpp : Поиск бинарным методом через создание адресации файла с базой данных
// Есть файл с настройками - Settings.txt:
// первая строка количество искомых позиций, вторая строка количество позиций в словаре для поиска
// Файл с позициями для проверки - In.txt, имеет следующую структуру:
// первая строка - название раздела, 2-4 строки - значения для поиска
// Файл с отсортированными по возрастанию данными для поиска - Data.txt
// Файл с результатами поиска (найденные позиции) - Out.txt:
// первая строка - название раздела найденной позиции, вторая строка - само найденное значение

#include <iostream>
#include <chrono>		// Для таймера std::chrono
#include <fstream>		// Для работы с std::ifstream и std::ofstream
#include <sstream>		// Для работы с std::stringstream
#include <cstring>      // Для работы со строками C-style
#include <exception>	// Для работы с std::exception

#include "Constants.h"


// Ошибки поиска
namespace EV
{
	enum ErrorValue
	{
		ErrorInFileOpen = -1,
		ErrorLengthInSearch = -2,
		ErrorLengthArray = -3,
		ErrorLengthInFile = -4,
		ErrorFileSettings = -5
	};
}

// Таймер
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	// Сброс секундомера
	void reset()
	{
		m_beg = clock_t::now();
	}

	// Вывод значения секундомера
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

// Функция вывода ошибок в консоль
void printError(const EV::ErrorValue value)
{
	switch (value)
	{
	case EV::ErrorInFileOpen:
		std::cerr << "The In-file did not open! ";
		break;
	case EV::ErrorLengthInSearch:
		std::cerr << "The length of " << g_inSearch << " is not multiple of " << g_amountPosition << ". ";
		break;
	case EV::ErrorLengthArray:
		std::cerr << "The number of rows in the file and in the settings is different. ";
		break;
	case EV::ErrorLengthInFile:
		std::cerr << "File size larger array size. ";
		break;
	case EV::ErrorFileSettings:
		std::cerr << "The settings file is filled incorrectly. ";
		break;
	default:
		std::cerr << "Unknown error! ";
		break;
	}

	std::cerr << "The program has been terminated." << std::endl;

	// Задержка перед закрытием консоли
	std::cout << "Press any key to close..." << std::endl;
	std::cin.clear();
	std::cin.ignore(32767, '\n');
	std::cin.get();
}

// Чтение содержимого файла в массив
int readIn(const char* fileName, adress_t* arrayDataOut, const adress_t lengthArray, const int readingMode)
{
	// Флаг режима чтения файла (readingMode = 0)
	auto flagIfstream = std::ios::in;
	// Длина считываемой строки
	int sizeLine = g_sizeSettigs;

	// Перевод файла в бинарный режим открытия для мода адресации
	if (readingMode)
	{
		flagIfstream = std::ios::binary;
		sizeLine = g_sizeLine;
	}

	std::ifstream in(fileName, flagIfstream);

	// Проверка открытия файла
	if (!in.is_open())
		return EV::ErrorInFileOpen;

	// Считываем данные в массив пока файл не закончится
	char* str = new char[sizeLine];
	adress_t lineByte = 0;
	Timer readDataTime;
	adress_t indexLength = 0;
	while (!in.eof())
	{
		// Если в настройках строк указано меньше, чем строк в файле
		if (indexLength >= lengthArray)
		{
			delete[] str;
			in.close();

			return EV::ErrorLengthArray;
		}

		// Считывем адрес позиции и значение
		lineByte = in.tellg(); // простота кода от такого росположения операции выше, чем потеря производительности для мода настроек
		in.getline(str, sizeLine, '\n');

		if (readingMode)
		{
			// Создание адресации
			arrayDataOut[indexLength] = lineByte;

			// Прогрессия создания адресации
			if ((indexLength + 1) % g_readProgress == 0)
			{
				std::cout << (indexLength + 1) / g_readProgress << "0kk, time: " << readDataTime.elapsed() << " sec, address in file = " << lineByte << '\n';
			}
		}
		else
		{
			// Чтение настроек
			if (strlen(str) != 0)
			{
				std::stringstream ss;
				ss << str;
				ss >> arrayDataOut[indexLength];
			}
			else
			{
				// Если строка пустая, файл настроек заполнен неверно
				delete[] str;
				in.close();

				return EV::ErrorFileSettings;
			}
		}

		++indexLength;
	}

	// Если в настройках указано больше строк, чем есть в файле
	if (indexLength != lengthArray)
	{
		delete[] str;
		in.close();

		return EV::ErrorLengthArray;
	}



	delete[] str;
	in.close();

	return 0;
}

// Бинарный поиск по файлу через файловую адресацию
int searchData(const char* fileName, adress_t* dataAdress, adress_t lengthVector, char* searchValue)
{
	std::ifstream in(fileName); // по умолчанию режим std::ios::in

	// Проверка открытия файла
	if (!in.is_open())
		return EV::ErrorInFileOpen;

	// Границы для бинарного поиска
	adress_t left = 0;
	adress_t right = lengthVector - 1;
	adress_t mid = 0;
	char* strData = new char[g_sizeLine];

	// Если границы пересеклись, то значение не найдено
	while (left <= right)
	{
		// Индекс срединной строки, с которой сравниваем
		mid = left + (right - left) / 2;

		// Перемещаем курсор к срединной строке через побайтовую адресацию
		// (к байту номер - dataAdress[mid] относительно начала файла)
		in.seekg(dataAdress[mid], std::ios::beg);

		in.getline(strData, g_sizeLine, '\n');

		// Сравниваем строки
		int compare = strcmp(searchValue, strData);
		if (compare < 0)
			right = mid - 1;
		else if (compare > 0)
			left = mid + 1;
		else
		{
			// Есть совпадение с базой данных
			std::cout << "Found the coincidence: " << searchValue << '\n';

			delete[] strData;
			in.close();

			return 1;
		}
	}

	// Совпадений не найдено
	delete[] strData;
	in.close();

	return 0;
}

// Сохранение результатов поиска в файл
void saveSearchResult(const char* fileName, char* parentStr, char* valueStr)
{
	// Файл для сохранения результатов поиска (дописываем в конец файла)
	std::ofstream out(fileName, std::ios::app);

	// Проверка открытия файла out
	if (!out.is_open())
	{
		// Файл не открылся, невозможно сохранить результаты в файл
		std::cout << "The Out-file did not open!\n";

		// Тогда выводим результаты поиска в консоль
		std::cout << "Valid value!\nParent: " << parentStr << "\nValue: " << valueStr << std::endl;
	}

	// Сохраняем найденное значение и название раздела в файл
	out << parentStr << std::endl;
	out << valueStr << std::endl;

	out.close();
}

// Чтение данных для поиска из файла и их поиск
int readSearchSave(const char* inFile, const adress_t lengthFile, adress_t* dataAdress, adress_t lengthAdress, const char* outFile)
{
	// Таймер поиска
	Timer searchTime;

	std::ifstream in(inFile); // по умолчанию режим std::ios::in

	// Проверка открытия файла inFile
	if (!in.is_open())
		return EV::ErrorInFileOpen;

	// Строка со значением для поиска в базе
	char* valueStr = new char[g_sizeLine];
	// Строка с названием раздела для текущего значения
	char* parentStr = new char[g_sizeLine];
	// Индекс текущей строки
	adress_t indexLength = 0;
	// Чтобы упоминание о несовпадении размеров файла и данных из настроек выводилось лишь раз
	bool sizePrint = true;

	// Перебираем позиции для поиска пока не закончится файл
	while (!in.eof())
	{
		// Если в настройках указано меньше строк, чем строк в файле
		if (indexLength >= lengthFile && sizePrint)
		{
			std::cerr << "The number of rows in the file is greater than indicated in the settings!!!\n";

			// Больше сообщение не выводим
			sizePrint = false;
		}

		in.getline(valueStr, g_sizeLine, '\n');

		// Если позиция содержит название раздела(начиная с [0] каждая 4)
		if (indexLength % g_amountPosition == 0)
		{
			// Сохраняем название текущего раздела
#pragma warning(suppress : 4996) // для VS
			strcpy(parentStr, valueStr);

			++indexLength;

			// Пропускаем поиск
			continue;
		}

		// Проверяем наличие значения в базе данных
		int resultSearch = searchData(g_inData, dataAdress, lengthAdress, valueStr);

		if (resultSearch == 1)
		{
			// Совпадение найдено, сохраняем найденной значение и название раздела в файл
			saveSearchResult(outFile, parentStr, valueStr);
		}
		else if (resultSearch < 0)
		{
			// Поиск закончился ошибкой
			std::cerr << "Search failed!\n";

			delete[] valueStr;
			delete[] parentStr;
			in.close();

			return resultSearch;
		}

		// resultSearch = 0 - продолжаем поиск
		++indexLength;

		// Прогрессия поиска
		if (indexLength % g_searchProgress == 0)
		{
			std::cout << "Checked " << indexLength / g_searchProgress << "k positions, time: " << searchTime.elapsed() << " sec.\n";

			// Сброс секундомера
			searchTime.reset();
		}
	}

	// Если в настройках строк указано больше, чем строк в файле
	if (indexLength < lengthFile)
	{
		std::cerr << "The number of rows in the file is less than indicated in the settings!!!\n";
	}

	delete[] valueStr;
	delete[] parentStr;
	in.close();

	return 0;
}

int main()
{
	// Запускаем общий таймер
	Timer overallTime;

	std::cout << "START PROGRAM!" << std::endl;

	// Массив для хранения данных из файла настроек
	adress_t* settings = new adress_t[g_lineSettings];

	// Получаем значения из файла настроек
	int readingMode = 0; // режим чтения для файла настроек
	int errorValue = readIn(g_settings, settings, g_lineSettings, readingMode); // settings - параметр вывода

	// Проверка успешности извлечения настроек
	if (errorValue != 0)
	{
		std::cerr << "Reading the settings file failed:\n";
		printError(static_cast<EV::ErrorValue>(errorValue));

		delete[] settings;

		return -1;
	}

	// Проверка кратности количества позиций для поиска строению маски значений и разделов
	if (settings[0] % g_amountPosition != 0)
	{
		printError(EV::ErrorLengthInSearch);

		delete[] settings;

		return -1;
	}

	std::cout << "Positions for searching: " << settings[0] << ", Data base: " << settings[1] << '\n';


	// Создаем адресацию файла с базой данных
	std::cout << "Start reading Data! Time: " << overallTime.elapsed() << " sec.\n";

	// Выделяем место под хранение файловой адресации
	adress_t* dataAdress = nullptr;
	try
	{
		dataAdress = new adress_t[settings[1]];
	}
	catch (std::bad_alloc& exception)
	{
		// Если память не выделена прекращаем программу
		std::cerr << "You ran out of memory!\n";

		delete[] settings;

		return -1;
	}
	catch (std::exception& exception)
	{
		// Случилось что-то непредвиденное при выделении памяти
		std::cerr << "Standard exception: " << exception.what() << '\n';

		delete[] settings;

		return -1;
	}

	// Создаём файловую адресацию для базы поисков
	readingMode = 1; // переводим функцию readIn() в режим создания адресации
	errorValue = readIn(g_inData, dataAdress, settings[1], readingMode); // dataAdress - параметр вывода

	// Проверка успешности создания адресации
	if (errorValue != 0)
	{
		std::cerr << "Reading the Data file failed:\n";
		printError(static_cast<EV::ErrorValue>(errorValue));

		delete[] dataAdress;
		delete[] settings;

		return -1;
	}

	// Поиск искомых позиций в базе данных
	std::cout << "Start search! Time: " << overallTime.elapsed() << " sec.\n";

	errorValue = readSearchSave(g_inSearch, settings[0], dataAdress, settings[1], g_outSearch);

	// Проверка успешности проведенного поиска
	if (errorValue < 0)
	{
		std::cerr << "Reading the file(" << g_inSearch << ") and searching failed:\n";
		printError(static_cast<EV::ErrorValue>(errorValue));

		delete[] dataAdress;
		delete[] settings;

		return -1;
	}

	delete[] dataAdress;
	delete[] settings;

	std::cout << "THE END! Time: " << overallTime.elapsed() << " sec." << std::endl;

	// Задержка перед закрытием консоли
	std::cout << "Press any key to close..." << std::endl;
	std::cin.clear();
	std::cin.ignore(32767, '\n');
	std::cin.get();

    return 0;
}
