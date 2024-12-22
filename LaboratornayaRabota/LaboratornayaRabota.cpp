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
        inRepair = false;
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
    cout << "5. Редактирование всех труб" << endl;
    cout << "6. Редактировать КС" << endl;
    cout << "7. Поиск труб" << endl;
    cout << "8. Поиск КС" << endl;
    cout << "9. Сохранить" << endl;
    cout << "10. Загрузить" << endl;
    cout << "0. Выход" << endl;
}

void toggleAllPipesRepairStatus() {
    if (pipes.empty()) {
        cout << "Нет труб для редактирования." << endl;
        return;
    }

    for (auto& pipe : pipes) {
        pipe.inRepair = !pipe.inRepair;  // Переключаем состояние
    }

    cout << "Статус 'в ремонте' для всех труб был переключен на: "
        << (pipes[0].inRepair ? "Да" : "Нет") << endl;  // Показываем новое состояние
}


void searchPipes() {
    string nameFilter;
    bool repairFilter;
    cout << "Хотите фильтровать по статусу ремонта? (1 - да, 0 - нет): ";
    cin >> repairFilter;

    bool found = false;
    for (const auto& pipe : pipes) {
        if ((!nameFilter.empty() && pipe.name.find(nameFilter) != string::npos) ||
            (repairFilter == pipe.inRepair)) {
            pipe.display();
            found = true;
        }
    }
    if (!found) {
        cout << "Трубы не найдены по заданному фильтру." << endl;
    }
}

void searchStations() {
    string nameFilter;
    double efficiencyFilter;
    cout << "Введите минимальный процент незадействованных цехов (от 0 до 100): ";
    cin >> efficiencyFilter;

    bool found = false;
    for (const auto& cs : stations) {
        double unusedWorkshops = cs.totalWorkShops - cs.workingWorkShops;
        double unusedPercentage = (unusedWorkshops / cs.totalWorkShops) * 100;

        if ((!nameFilter.empty() && cs.name.find(nameFilter) != string::npos) ||
            (unusedPercentage >= efficiencyFilter)) {
            cs.display();
            found = true;
        }
    }
    if (!found) {
        cout << "КС не найдены по заданному фильтру." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
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

        else if (choice == 4) {
            int index;
            cout << "Введите индекс трубы для редактирования (0 - " << pipes.size() - 1 << "): ";
            cin >> index;
            if (index >= 0 && index < pipes.size()) {
                pipes[index].editRepairStatus();
            }
            else {
                cout << "Некорректный индекс!" << endl;
            }
        }

        else if (choice == 6) {
            int index;
            cout << "Введите индекс КС для редактирования (0 - " << stations.size() - 1 << "): ";
            cin >> index;
            if (index >= 0 && index < stations.size()) {
                int action;
                cout << "1. Запустить цех\n2. Остановить цех\nВыберите действие: ";
                cin >> action;
                if (action == 1) {
                    stations[index].startWorkShop();
                }
                else if (action == 2) {
                    stations[index].stopWorkShop();
                }
            }
            else {
                cout << "Некорректный индекс!" << endl;
            }
        }

        else if (choice == 9) {
            ofstream out("data.txt");
            for (const auto& pipe : pipes) {
                pipe.save(out);
            }
            for (const auto& cs : stations) {
                cs.save(out);
            }
            out.close();
            cout << "Данные сохранены!" << endl;
        }

        else if (choice == 10) {
            ifstream in("data.txt");
            if (in.is_open()) {
                pipes.clear();
                stations.clear();

                while (!in.eof()) {
                    Pipe pipe;
                    pipe.load(in);
                    pipes.push_back(pipe);
                }

                while (!in.eof()) {
                    CompressorStation cs;
                    cs.load(in);
                    stations.push_back(cs);
                }
                in.close();
                cout << "Данные загружены!" << endl;
            }
            else {
                cout << "Не удалось открыть файл!" << endl;
            }
        }

        else if (choice == 7) {
            searchPipes();  // Вызов поиска труб
        }

        else if (choice == 8) {
            searchStations();  // Вызов поиска КС
        }

        else if (choice == 5) {
            toggleAllPipesRepairStatus();  // Переключаем статус для всех труб
        }


        else if (choice == 0) {
            break;
        }
        else {
            cout << "Некорректный ввод, попробуйте снова." << endl;
        }
    }
    return 0;
}