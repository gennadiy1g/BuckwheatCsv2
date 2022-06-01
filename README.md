# Buckwheat CSV

A viewer for (big) CSV files.

## Why this program is useful

This program is useful if:

 * You want to preview a CSV file.
 * The file is too big for Microsoft Excel or LibreOffice Calc, or it takes too much time to open the file in one of these programs.

 Buckwheat CSV:

 * Will not load the whole file into memory.
 * Does not have a limit on the size of the file. However, there is a limit on the number of rows that can be displayed in a grid (see below).

## Limitations

* Maximum number of rows that can be displayed in a grid is 2,147,483,647. If a file has more rows, only first 2,147,483,647 rows are displayed, and the user is notified.
* CSV files must be [UTF-8](https://en.wikipedia.org/wiki/UTF-8) encoded.

## Getting started

* Download zip file of the latest release from [https://github.com/gennadiy1g/BuckwheatCsv2/releases](https://github.com/gennadiy1g/BuckwheatCsv2/releases).
* Unzip into a folder of your choice.
* On Windows: Open Windows Explorer and navigate to the folder containing ***`BuckwheatCsv.exe`***. Double click on ***`BuckwheatCsv.exe`***.
* On Linux: Open terminal and change the shell's current working directory to the folder containing ***`BuckwheatCsv`***. Type ***`./BuckwheatCsv`*** and press Enter.
* After Buckwheat CSV started, click File->Open ..., or drag and drop a CSV file onto Buckwheat CSV's window.

## Credit and licenses for embedded code

This project includes code from outside sources:

* [Boost](https://www.boost.org/LICENSE_1_0.txt)
* [wxWidgets](https://www.wxwidgets.org/about/licence/)
