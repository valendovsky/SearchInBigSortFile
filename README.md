# SearchInBigSortFile
The search program in a very large assorted file.


The program searches for values in sorted by a very large text file. The search is made by the binary method through the creation of the file addressing of this large file.

The program takes on the input 3 file: the settings file (Settings.txt), the file with the search values (In.txt) and the file with the database for the search (Data.txt).

In the settings file you need to fill out two lines. The first line contains the number of rows (values) in In.txt, the second line contains the number of rows (value) in Data.txt.

The file with the search values has a specific structure. The first line contains the name of the section. From the second to the fourth line contains values for search. This mask is repeated every 4 lines. Accordingly, the program searches only by values of 2-4 lines. Values for searching, including sections name, do not exceed 99 characters.

The database file contains values sorted by increasing value. All files should not contain empty rows.

The program saves all the coincidences in the text file (Out.txt). The first line indicates the name of the section, in the second line the value itself. If the program cannot save the value in the Out.txt, it displays the values into the console and continues the search further.



Программа производит поиск значений в отсортированном очень большом текстовом файле. Поиск производится бинарным методом через создание файловой адресации этого файла.

Программа принимает на вход 3 файла: файл настроек (Settings.txt), файл со значениями для поиска (In.txt) и файл с базой данных для поиска (Data.txt).

В файле настроек надо заполнить две строки. Первая строка содержит число строк(значений) в файле In.txt, вторая строка содержит число строк(значение) в файле Data.txt.

Файл со значениями для поиска имеет определенную структуру. Первая строка содержит название раздела. Со 2 по 4 строку содержаться значения для поиска. Эта маска повторяется каждые 4 строки. Соответственно программа производит поиск только по значениям 2-4 строк. Значения для поиска, включая название разделов, не превышают 99 символов.

Файл с базой данных содержит отсортированные по возрастанию значения. Все файлы не должны содержать пустые строки.

Программа сохраняет все найденные совпадения в текстовый файл(Out.txt). Первой строкой указывается название раздела, во второй строке само значение. Если программа не может сохранить найденное совпадение в файл, то выводит его в консоль и продолжает поиск дальше.
