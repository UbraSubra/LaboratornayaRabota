//LR1 Karpov-AA2307
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

    