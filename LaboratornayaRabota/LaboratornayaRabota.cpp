#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <map>

using namespace std;

struct Pipe {
    static int id_counter;  // Статический счетчик ID для каждой трубы
    int id;  // Уникальный идентификатор трубы
    string name;
    double length;
    double diameter;
    bool inRepair;

    Pipe() {
        id = id_counter++;  // Устанавливаем уникальный ID
        inRepair = false;
    }

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
        cout << "Труба " << name << " (ID: " << id << "), длина: " << length << " км, диаметр: "
            << diameter << " см, в ремонте: " << (inRepair ? "Да" : "Нет") << endl;
    }

    void editRepairStatus() {
        inRepair = !inRepair;
        cout << "Статус 'в ремонте' обновлен на: " << (inRepair ? "Да" : "Нет") << endl;
    }

    void save(ofstream& out) const {
        out << id << " " << name << " " << length << " " << diameter << " " << inRepair << endl;
    }

    void load(ifstream& in) {
        in >> id >> name >> length >> diameter >> inRepair;
    }
};

int Pipe::id_counter = 1; // Инициализация счетчика ID

struct CompressorStation {
    static int id_counter;  // Статический счетчик ID для каждой КС
    int id;  // Уникальный идентификатор КС
    string name;
    int totalWorkShops;
    int workingWorkShops;
    double efficiency;

    CompressorStation() {
        id = id_counter++;  // Устанавливаем уникальный ID
    }

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
        cout << "КС " << name << " (ID: " << id << "), всего цехов: " << totalWorkShops
            << ", работающих цехов: " << workingWorkShops << ", эффективность: "
            << efficiency << endl;
    }

    void save(ofstream& out) const {
        out << id << " " << name << " " << totalWorkShops << " " << workingWorkShops << " " << efficiency << endl;
    }

    void load(ifstream& in) {
        in >> id >> name >> totalWorkShops >> workingWorkShops >> efficiency;
    }
};

int CompressorStation::id_counter = 1; // Инициализация счетчика ID для КС

struct Connection {
    int cs_in_id;
    int cs_out_id;
    int pipe_id;

    Connection(int cs_in, int cs_out, int pipe)
        : cs_in_id(cs_in), cs_out_id(cs_out), pipe_id(pipe) {
    }
};

vector<Pipe> pipes;
vector<CompressorStation> stations;
vector<Connection> connections;

CompressorStation* findStationById(int id) {
    for (auto& station : stations) {
        if (station.id == id) {
            return &station;  // Возвращаем указатель на найденную станцию
        }
    }
    return nullptr;  // Если станция не найдена
}

Pipe* findPipeById(int id) {
    for (auto& pipe : pipes) {
        if (pipe.id == id) {
            return &pipe;  // Возвращаем указатель на трубу
        }
    }
    return nullptr;  // Если труба не найдена
}

Pipe* findPipeByDiameter(double diameter) {
    for (auto& pipe : pipes) {
        if (pipe.diameter == diameter && !pipe.inRepair) {
            return &pipe;  // Возвращаем указатель на трубу, если она не в ремонте и найдено совпадение по диаметру
        }
    }
    return nullptr;
}

Pipe createNewPipe(double diameter) {
    Pipe newPipe;
    newPipe.diameter = diameter;
    cout << "Труба с диаметром " << diameter << " см не найдена, создается новая.\n";
    newPipe.read();  // Ввод данных для новой трубы
    pipes.push_back(newPipe);  // Добавляем в список труб
    return newPipe;  // Возвращаем объект по значению
}

void connectStationsToPipe(int cs_in_id, int cs_out_id, double diameter) {
    CompressorStation* cs_in = findStationById(cs_in_id);  // Ищем станцию по ID
    CompressorStation* cs_out = findStationById(cs_out_id);  // Ищем станцию по ID

    if (!cs_in || !cs_out) {
        cout << "Ошибка: Одна из станций с указанным ID не найдена.\n";
        return;  // Прерываем выполнение, если станция не найдена
    }

    Pipe* pipe = findPipeByDiameter(diameter);  // Ищем трубу по диаметру
    if (pipe == nullptr) {
        Pipe newPipe = createNewPipe(diameter);  // Создаем новый объект трубы
        pipe = &pipes.back();  // Используем последний добавленный объект
    }

    connections.push_back(Connection(cs_in->id, cs_out->id, pipe->id));  // Добавляем соединение
    cout << "Соединение КС " << cs_in->name << " с КС " << cs_out->name
        << " трубой с ID " << pipe->id << " установлено.\n";
}

void displayConnections() {
    cout << "Список соединений:\n";
    for (const auto& conn : connections) {
        CompressorStation* cs_in = findStationById(conn.cs_in_id);
        CompressorStation* cs_out = findStationById(conn.cs_out_id);
        Pipe* pipe = findPipeById(conn.pipe_id);

        if (cs_in && cs_out && pipe) {
            cout << "КС " << cs_in->name << " (ID: " << cs_in->id << ") --> "
                << "КС " << cs_out->name << " (ID: " << cs_out->id << ") через трубу "
                << pipe->name << " (ID: " << pipe->id << ", диаметр: " << pipe->diameter << " см)" << endl;
        }
        else {
            cout << "Ошибка: Неверное соединение (ID: " << conn.cs_in_id << " -> " << conn.cs_out_id << ").\n";
        }
    }
}

void printMenu() {
    cout << "Выберите действие:" << endl;
    cout << "1. Добавить трубу" << endl;
    cout << "2. Добавить КС" << endl;
    cout << "3. Просмотр всех объектов" << endl;
    cout << "4. Редактировать трубу" << endl;
    cout << "5. Соединить КС трубой" << endl;
    cout << "6. Выход" << endl;
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
            cout << "Список труб:\n";
            for (const auto& pipe : pipes) {
                pipe.display();
            }

            cout << "\nСписок КС:\n";
            for (const auto& cs : stations) {
                cs.display();
            }

            cout << "\nСписок соединений:\n";
            displayConnections();
        }
        else if (choice == 4) {
            int pipe_id;
            cout << "Введите ID трубы для редактирования: ";
            cin >> pipe_id;

            for (auto& pipe : pipes) {
                if (pipe.id == pipe_id) {
                    pipe.editRepairStatus();
                    break;
                }
            }
        }
        else if (choice == 5) {
            int cs_in_id, cs_out_id;
            double diameter;
            cout << "Введите ID КС для входа: ";
            cin >> cs_in_id;
            cout << "Введите ID КС для выхода: ";
            cin >> cs_out_id;
            cout << "Введите диаметр трубы: ";
            cin >> diameter;

            connectStationsToPipe(cs_in_id, cs_out_id, diameter);
        }
        else if (choice == 6) {
            break;
        }
        else {
            cout << "Некорректный ввод, попробуйте снова." << endl;
        }
    }
    return 0;
}
