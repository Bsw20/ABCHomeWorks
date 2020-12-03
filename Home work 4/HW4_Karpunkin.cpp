//
//  main.cpp
//
//  Created by Ярослав Карпунькин on 01.12.2020.
//  группа 191
//  Вариант 17

using namespace std;

#include <iostream>
#include <vector>
#include <thread>
#include <omp.h>
#include <map>
#include <String>

//Класс, который описывает координаты в библиотеке
class Coordinates {
private:
    int x, y, z;
public:
    Coordinates(int row, int bookshelf, int bookPlace) {
        x = row;
        y = bookshelf;
        z = bookPlace;
    }
    //Строковое описание класса
    string coordDescription() {
        return "row - " + to_string(x) + ", bookshelf - " + to_string(y) + ", book place - " + to_string(z);
    }
};

//Библиотека
class Library {
private:
    //Контейнер для библиотеки
    vector<vector<vector<int>>> library;
    int m2 = 0; //Количество рядов
    int n2 = 0; //Количество шкафов
    int k2 = 0; //Количество книг в шкафу

private:
    //Метод, который вставляет книгу
    void setBook(int row, int bookshelve, int book, int bookId) {
        library[row][bookshelve][book] = bookId;
    }
    //Базовая настройка класса
    void initLibrary(int m, int n, int k){
        m2 = m;
        n2 = n;
        k2 = k;
        library.resize(m);
        for (int i = 0; i < m; ++i) {
            library[i].resize(n);
            for (int j = 0; j < n; ++j)
                library[i][j].resize(k);
        }
    }

public:
    //Метод, который заполняет библиотеку
    void setupLibrary(int m, int n, int k) {
        initLibrary(m, n, k);
        int bookId = m*n*k;
        int threadNum;
        cout << "Generate library:" << endl;
        //Параллельно заполняем библиотеку
#pragma omp parallel for default(none) private(threadNum) \
        shared(library, m2, n2, k2, cout, bookId)
        for (int i = 0; i < m2; ++i) {
            threadNum = omp_get_thread_num();
            for (int j = 0; j < n2; ++j) {
                for (int k = 0; k < k2; ++k) {
                    setBook(i, j, k, bookId--);
#pragma omp critical
                    {
                        cout << "\t" << "Thread" << threadNum << " generated book with ID "
                             << to_string(library[i][j][k]) << endl;
                    }
                }
            }
        }
    }
    //Метод для получения книги по ее координатам
    int getBook(int i, int j, int y) {
        if(i >= m2 || j >= n2 || y >= k2){
            throw runtime_error("ERROR: Out of bounds!");
        }
        return library[i][j][y];
    }
};

//Класс каталога
class Catalog {
private:
    //Контейнер католога, структура априори сортируется по ключу
    map<int, Coordinates> catalog;
public:
    void setupCatalog(Library library, int m, int n, int k) {
        cout << "Setup catalog:" << endl;
        int threadNum;
        // Параллельно проходимся по библиотеке и заносим записи в каталог
#pragma omp parallel for default(none) private(threadNum) \
        shared(catalog, library, m, n, k, cout)
        for (int i = 0; i < m; ++i) {
            threadNum = omp_get_thread_num();
            for (int j = 0; j < n; ++j) {
                for (int y = 0; y < k; ++y) {
                    int needBookId = library.getBook(i, j, y);
#pragma omp critical
                    {
                        catalog.insert(std::make_pair(needBookId, Coordinates(i, j, y)));
                        cout << "\t" << "Thread" << threadNum << " added book "
                             << needBookId << " to catalog" << endl;
                    }
                }
            }
        }

    }
    //Метод, который возвращает координаты книги по ее id
    Coordinates getBookCoordinatesById(const int& bookId) {
        return catalog.at(bookId);
    }
    //Метод, который принтит координаты книги по ее id
    void printBookDescrByBookId(const int& bookId) {
        auto coordinates = getBookCoordinatesById(bookId);
        cout << "Book with ID " << bookId << " found:" << endl;
        cout << "\t" << "Coordinates: " + coordinates.coordDescription() + "\n";
    }
    //Метод, который принтит весь каталог в консоль
    void printCatalog() {
        cout << "Print catalog:" << endl;
        for(auto elem : catalog)
        {
            cout << "\t" << "BookID - " + to_string(elem.first) + "; Coordinates: " + elem.second.coordDescription() + "\n";
        }
        cout << endl;
    }
};

int main() {
    //количество рядов
    int m;
    //количество шкафов
    int n;
    //количество полок
    int k;
    //Считывание данных
    cout << "Enter count of rows in the library:";
    cin >> m;
    cout << "Enter count of bookshelves at row in the library:";
    cin >> n;
    cout << "Enter count of books at bookshelves in the library:";
    cin >> k;
    //мини проверка, если данные некорректы, выполнение заканчивается
    if( m <= 0 || n <= 0 || k <= 0) {
        cout << "Enter numbers > 0\n";
        return 1;
    }

    Library library;
    // сетапим библиотеку(по факту создаем)
    library.setupLibrary(m, n, k);

    Catalog catalog;
    // сетапим каталог
    catalog.setupCatalog(library, m, n, k);
    //Принтим каталог
    catalog.printCatalog();
    //Принтим координаты книги по ее id
    //catalog.printBookDescrByBookId(1);
    return 0;
}
