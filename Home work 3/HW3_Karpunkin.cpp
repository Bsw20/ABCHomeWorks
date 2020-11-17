//
//  main.cpp
//  ABC_HW3
//
//  Created by Ярослав Карпунькин группа 191 on 16.11.2020.
// Вариант 17

using namespace std;

#include <iostream>
#include <stack>
#include <mutex>
#include <vector>
#include <thread>

// мьютекс для ограничения доступа к стеку
mutex mtx;
// стек задач
stack<int> st;
//количество рядов
int m;
//количество шкафов
int n;
//количество полок
int k;


class Library {
public:
    //метод, добавляет задачу по восстановлению в "портфель задач"
    void setupLibrary(int m, int n, int k) {
        library = new string **[m];
        for (int i = 0; i < m; ++i) {
            library[i] = new string *[n];
            for (int j = 0; j < n; ++j) {
                library[i][j] = new string[k];
                for (int y = 0; y < k; ++y) {
                    // добавляем задачу по восстановлению в стек
                    st.push(i * n * k + j * k + y);
                }
            }
        }
    }
    
    //метод, который "ставит" книгу на нужное место
    void setBook(int row, int bookshelve, int book, string bookDescr) {
        library[row][bookshelve][book] = bookDescr;
    }
    
    //принт библиотеки
    void print() {
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                for (int y = 0; y < k; ++y)
                    cout << library[i][j][y] << endl;
    }
private:
    string ***library;
};
//библиотека
Library library;

// метод для восстановления названия книги
static string getBookName(int row, int bookshelve, int book, int streamId ) {
    
    return "It's a book with coordinates (" + to_string(row) + ", " + to_string(bookshelve) + ", " + to_string(book) + "), stream id: " + to_string(streamId);
}

//метод для потоков
void setBooks(int threadId) {
    while (!st.empty()) {
        // блокируем доступ к стеку с помощью мьютексов
        mtx.lock();
        // поток завершает работу если в портфеле не осталось задач
        if (st.empty())
            return;
        int num = st.top();
        st.pop();
        // после работы со стеком разблокируем доступ к нему для остальных потоков
        mtx.unlock();

        // вычисляем номер ряда, шкафа и книги
        int row = num / k / n % m;
        int bookshelve = num / k % n;
        int book = num % k;
        // записываем информацию о книге в библиотеку
        string bookDecsr = getBookName(row, bookshelve, book, threadId);
        library.setBook(row, bookshelve, book, bookDecsr);
    }
}

int main(int argc, const char * argv[]) {
    //количество потоков
    const int THREAD_COUNT = 3;
    
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
    
    // сетапим библиотеку(по факту создаем)
    library.setupLibrary(m, n, k);
    
    // cоздаем потоки и ждем, пока они завершатся
    vector<thread> threads(THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; ++i)
        threads[i] = thread(setBooks, i);
    
    for (int i = 0; i < THREAD_COUNT; ++i)
        threads[i].join();
    //принт библиотеки
    library.print();
    
    return 0;
}
