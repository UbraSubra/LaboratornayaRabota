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
