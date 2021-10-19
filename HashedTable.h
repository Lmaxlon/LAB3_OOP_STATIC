#pragma once
#include <iostream>
using namespace std;
struct Elem { //элемент таблицы
    int key; //ключ
    int occupied; //признак занятости
    char info [21]; //информация
    Elem () : key(0),occupied(0) {info[0] = '\0';}; //конструктор по умолчанию
};


class CHashedTable
        {
    static const int size = 10; //максимальный размер таблицы
    static const int step = 1;
    unsigned int count; //количество элементов в таблице
    Elem data [size]; //массив данных

        public:
            CHashedTable(); //конструктор по умолчанию
            CHashedTable (Elem* elem, int n); //конструктор из массива
            CHashedTable (const CHashedTable &t); //конструктор копирования
            ~CHashedTable(); //деструктор

            CHashedTable& operator= (const CHashedTable t); //оператор присваивания

            CHashedTable& operator+= (const Elem& t);
            CHashedTable& operator-= (const Elem& t);

            Elem operator[] (int key); //получение элемента по ключу
            bool operator() (int key); //проверка существования элемента по ключу

            void Clear (); //сжатие таблицы (условное, поскольку память статическая)

            friend ostream& operator << (ostream& str, CHashedTable& t); //вывод
            friend istream& operator >> (istream& str, CHashedTable& t); //ввод

        private:
            unsigned int Hash (int key); //функция хеширования
            bool HasKey (int key);
        };
