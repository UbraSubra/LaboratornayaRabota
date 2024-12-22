﻿//LR1 Karpov-AA2307
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Pipe {
    string name;
    double length;
    double diameter;
    bool inRepair;

    void read() {
        cout << "Ввод имени трубы: ";
        cin >> name;
        cout << "Ввод длины трубы (в км): ";
        cin >> length;
        cout << "Ввод диаметра трубы (в см): ";
        cin >> diameter;
        inRepair = false; // По умолчанию не в ремонте
    }

    void display() const {
        cout << "Труба " << name << ", длина: " << length << " км, диаметр: "
            << diameter << " см, в ремонте: " << (inRepair ? "Да" : "Нет") << endl;
    }

    void editRepairStatus() {
        inRepair = !inRepair;
        cout << "Статус 'в ремонте' обновлен на: " << (inRepair ? "Да" : "Нет") << endl;
    }

    void save(ofstream& out) const {
        out << name << " " << length << " " << diameter << " " << inRepair << endl;
    }

    void load(ifstream& in) {
        in >> name >> length >> diameter >> inRepair;
    }
};

struct CompressorStation {
    string name;
    int totalWorkShops;
    int workingWorkShops;
    double efficiency;
    
    void read() {
        cout << "Ввод имени КС: ";
        cin >> name;
        cout << "Ввод количества цехов: ";
        cin >> totalWorkShops;
        cout << "Ввод количества работающих цехов: ";
        cin >> workingWorkShops;
        cout << "Ввод эффективности: ";
        cin >> efficiency;
    }

    void display() const {
        cout << "КС " << name << ", всего цехов: " << totalWorkShops
            << ", работающих цехов: " << workingWorkShops << ", эффективность: "
            << efficiency << endl;
    }

    void startWorkShop() {
        if (workingWorkShops < totalWorkShops) {
            workingWorkShops++;
            cout << "Цех запущен. Теперь работающих цехов: " << workingWorkShops << endl;
        }
        else {
            cout << "Все цеха уже работают!" << endl;
        }
    }

    void stopWorkShop() {
        if (workingWorkShops > 0) {
            workingWorkShops--;
            cout << "Цех остановлен. Теперь работающих цехов: " << workingWorkShops << endl;
        }
        else {
            cout << "Все цеха уже остановлены!" << endl;
        }
    }

    void save(ofstream& out) const {
        out << name << " " << totalWorkShops << " " << workingWorkShops << " " << efficiency << endl;
    }

    void load(ifstream& in) {
        in >> name >> totalWorkShops >> workingWorkShops >> efficiency;
    }
};

vector<Pipe> pipes;
vector<CompressorStation> stations;

void printMenu() {
    cout << "Выберите действие:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Редактировать КС" << endl;
    cout << "6. Сохранить" << endl;
    cout << "7. Загрузить" << endl;
    cout << "0. Выход" << endl;
}

int main() {
    while (true) {
        printMenu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            Pipe pipe;
            pipe.read();
            pipes.push_back(pipe);
        }

        else if (choice == 2) {
            CompressorStation cs;
            cs.read();
            stations.push_back(cs);
        }

        else if (choice == 3) {
            cout << "Список труб:" << endl;
            for (const auto& pipe : pipes) {
                pipe.display();
            }
            cout << "Список КС:" << endl;
            for (const auto& cs : stations) {
                cs.display();
            }
        }

