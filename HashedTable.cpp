#include "HashedTable.h"
#include <string.h>

CHashedTable::CHashedTable() : count(0) //обнуляем число элементов
{
}

CHashedTable::~CHashedTable()
{
}

CHashedTable::CHashedTable (Elem* elem, int n) //конструктор из массива
{
    if (n>size) throw "Too big array"; //если размер массива больше максимального
    count = n; //сохраняем количество
    for (int i = 0; i<n; i++) { //идём по элементам
        if (HasKey (elem[i].key)) continue;
        int j = Hash(elem[i].key); //получаем адрес
        if (data[j].occupied!=0) { //если уже занят
            for (int k = 1; k<size; k++) { //ищем новый адрес
                j = Hash(elem[i].key+k);
                if (data[j].occupied==0) break;
            }
        }
        if (data[j].occupied!=0) throw "Too many keys with one hash"; //все места заняты - ошибка
        data[j].occupied = 1; //записываем элемент, помечаем место занятым
        data[j].key = elem[i].key;
        strcpy (data[j].info, elem[i].info);
    }
}

CHashedTable::CHashedTable (const CHashedTable &t) //конструктор копирования
{
    count = t.count; //копируем всё из другого экземпляра
    for (int i = 0; i<size; i++) {
        data[i].key = t.data[i].key;
        data[i].occupied = t.data[i].occupied;
        strcpy (data[i].info, t.data[i].info);
    }
}

CHashedTable& CHashedTable::operator= (const CHashedTable t) //оператор присваивания
        {
    count = t.count; //копируем всё из другого экземпляра
    for (int i = 0; i<size; i++) {
        data[i].key = t.data[i].key;
        data[i].occupied = t.data[i].occupied;
        strcpy (data[i].info, t.data[i].info);
    }
    return *this;
        }

        CHashedTable& CHashedTable::operator+= (const Elem& t)
                {
    int t1=-1, t2=-1;
    if (count == size) return *this; //если таблица уже заполнена, ничего не делаем
    //if (HasKey(t.key)) return *this;
    int start, j, free = -1;
    start = j = Hash (t.key); //получаем адрес
    do {
        if (data[j].occupied==1 && data[j].key==t.key)
            return *this; //место занято и ключи совпадают,выходим
            if (data[j].occupied==-1)
                if(free == -1)
                    free = j;
                if (data[j].occupied == 0) {
                    if(free != -1)
                        j = free;
                    data[j].occupied = 1; //записываем новый элемент
                    data[j].key = t.key;
                    strcpy (data[j].info, t.info);
                    count++; //увеличиваем количество
                    return *this;
                }
                j = (j+step)%size;
    } while(start != j);
    if(free == -1)
        return *this;
    else
        j = free;
    data[j].occupied = 1; //записываем новый элемент
    data[j].key = t.key;
    strcpy (data[j].info, t.info);
    count++; //увеличиваем количество
    return *this;
                }
                //		if (data[j].key==t.key) return *this;
                //		if (data[j].occupied==0 && t1<0) t1=j;
                //		else if (data[j].occupied==-1 && t2<0) t2=j;
                //	}
                //}
                //if (t2<0 && t1<0) return *this; //если все места заняты, ничего не делаем
                //if (t2>=0) j=t2;
                //else j=t1;
                //data[j].occupied = 1; //записываем новый элемент
                //data[j].key = t.key;
                //strcpy (data[j].info, t.info);
                //count++; //увеличиваем количество
                //return *this;
                //}

                CHashedTable& CHashedTable::operator-= (const Elem& t)
                        {
    if (count == 0) return *this; //если таблица пуста, ничего не делаем
    int j = Hash (t.key); //получаем адрес
    if (data[j].occupied==0) return *this; //такого нет и не было
    if (data[j].occupied==-1 || data[j].key!=t.key) { //если место свободно или на нём другой элемент
        for (int k = 1; k<size; k++) { //перебираем другие адреса
            //j = Hash(t.key+k);
            j = (j+step)%size;
            if (data[j].occupied==1 && data[j].key==t.key) break; //если место занято и ключи совпадают, прерываем
        }
    }
    if (data[j].occupied==0 || data[j].key!=t.key) return *this; //не нашли - ничего не делаем
    data[j].occupied = -1; //условно удаляем
    count--; //уменьшаем количество
    return *this;
                        }

                        Elem CHashedTable::operator[] (int key)
                        {
    Elem null; //пустой элемент для возврата
    strcpy (null.info, "Not exist");
    if (count == 0)
        return null; //если таблица пуста, возвращаем пустой
        int start, j;
        start = j = Hash (key); //ищем элемент аналогично прочим функциям
        if (data[j].occupied==0)
            return null; //такого нет и не было
            do {
                if (data[j].occupied == 1 && data[j].key==key)
                    return data[j];	//иначе возвращаем найденный
                    j = (j+step)%size;
            } while(j != start && data[j].occupied != 0);
            return null; //если не нашли, возвращаем пустой
                        }

                        bool CHashedTable::operator() (int key)
                        {
    if (count == 0)
        return false; //если таблица пуста, возвращаем пустой
        int start, j;
        start = j = Hash (key); //ищем элемент аналогично прочим функциям
        if (data[j].occupied==0)
            return false; //такого нет и не было
            do {
                if (data[j].occupied == 1 && data[j].key==key)
                    return true;	//иначе возвращаем найденный
                    j = (j+step)%size;
            } while(j != start && data[j].occupied != 0);
            return false; //если не нашли, возвращаем пустой
                        }

                        void CHashedTable::Clear ()
                        {
    if (count==0) return; //если таблица пуста, нечего сжимать
    for (int i = 0; i<size; i++) { //идём по массиву
        if (data[i].occupied==0 || data[i].occupied==-1) continue; //если место свободно, пропускаем
        int j = Hash(data[i].key); //определяем "идеальный" адрес
        if (i==j) continue; //если элемент на нём и стоит, пропускаем
        for (int k = 0; k<size && j<i; k++) { //перебираем другие подходящие адреса, расположенные раньше текущего
            //j = (j+step)%size;j = Hash(data[i].key+k);
            j = (j+k)%size;
            if (data[j].occupied==0 || data[j].occupied==-1) break; //если нашли свободный, прерываем
        }
        if ((data[j].occupied==0 || data[j].occupied==-1) && j<i) { //если нашли свободный и он раньше текущего
            data[j] = data[i]; //кладём элемент на лучшую позицию
            data[i].occupied = -1; //старое место освобождаем
        }
    }
                        }

                        unsigned int CHashedTable::Hash (int key)
                        {
    return key%size; //вычисляем адрес элемента как остаток от деления на максимальный размер таблицы
                        }

                        bool CHashedTable::HasKey (int key)
                        {
    for (int i = 0; i<size; i++)
        if (data[i].occupied!=0 && data[i].key == key) return true;
        return false;
                        }

                        ostream& operator << (ostream& str, CHashedTable& t) //вывод
                        {
    str << endl;
    for (int i=0; i<t.size; i++) { //выводим все элементы, кроме пустых и условно удалённых
        if (t.data[i].occupied == 0 || t.data[i].occupied == -1) continue;
        str << i << ": key = " << t.data[i].key << ", " << t.data[i].info << endl;
    }
    return str;
                        }

                        istream& operator >> (istream& str, CHashedTable& t) //ввод
                        {
    Elem tmp; //временная переменная для элемента
    cout << "Key: " << endl;
    str >> tmp.key; //ввод ключа
    cout << "Information: " << endl;
    str >> tmp.info; //ввод информации
    t+=tmp; //добавление элемента
    return str;
                        }