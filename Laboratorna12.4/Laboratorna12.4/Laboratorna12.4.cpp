#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

struct Room {
    int phone_number;
    int room_number;
    char employees[4][50];  
    int employee_count;
};

// Функції
void AddRoom(const string& filename);
void DeleteRoom(const string& filename);
void EditRoom(const string& filename);
void PrintAllRooms(const string& filename);
void FindByPhoneNumber(const string& filename);
void FindByRoomNumber(const string& filename);
void FindByEmployee(const string& filename);

bool ValidateFile(const string& filename);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    cout << "Введіть ім'я файлу: ";
    getline(cin, filename);

    if (!ValidateFile(filename)) {
        ofstream file(filename, ios::binary);
        file.close();
    }

    int choice;
    do {
        cout << "\nОберіть дію:\n";
        cout << "1 - Додати приміщення\n";
        cout << "2 - Видалити приміщення\n";
        cout << "3 - Редагувати приміщення\n";
        cout << "4 - Вивести всі приміщення\n";
        cout << "5 - Пошук за номером телефону\n";
        cout << "6 - Пошук за номером приміщення\n";
        cout << "7 - Пошук за прізвищем службовця\n";
        cout << "0 - Вихід\n";
        cout << "Ваш вибір: ";
        cin >> choice;

        switch (choice) {
        case 1:
            AddRoom(filename);
            break;
        case 2:
            DeleteRoom(filename);
            break;
        case 3:
            EditRoom(filename);
            break;
        case 4:
            PrintAllRooms(filename);
            break;
        case 5:
            FindByPhoneNumber(filename);
            break;
        case 6:
            FindByRoomNumber(filename);
            break;
        case 7:
            FindByEmployee(filename);
            break;
        case 0:
            cout << "Вихід..." << endl;
            break;
        default:
            cout << "Невірний вибір. Спробуйте ще раз." << endl;
        }
    } while (choice != 0);

    return 0;
}

bool ValidateFile(const string& filename) {
    ifstream file(filename, ios::binary);
    return file.good();
}

void AddRoom(const string& filename) {
    Room room;
    cout << "Введіть номер телефону (2 цифри): ";
    cin >> room.phone_number;
    cout << "Введіть номер приміщення: ";
    cin >> room.room_number;
    cout << "Введіть кількість службовців (до 4): ";
    cin >> room.employee_count;

    if (room.employee_count > 4) {
        cout << "Кількість службовців не може перевищувати 4. Скорочено до 4." << endl;
        room.employee_count = 4;
    }

    cin.ignore();
    for (int i = 0; i < room.employee_count; i++) {
        cout << "Введіть прізвище службовця №" << i + 1 << ": ";
        cin.getline(room.employees[i], 50);
    }

    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    file.write((char*)&room, sizeof(Room));
    file.close();
    cout << "Приміщення додано." << endl;
}

void DeleteRoom(const string& filename) {
    int phone;
    cout << "Введіть номер телефону приміщення для видалення: ";
    cin >> phone;

    ifstream file(filename, ios::binary);
    ofstream temp("temp.bin", ios::binary);

    if (!file || !temp) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    bool found = false;

    while (file.read((char*)&room, sizeof(Room))) {
        if (room.phone_number != phone) {
            temp.write((char*)&room, sizeof(Room));
        }
        else {
            found = true;
        }
    }

    file.close();
    temp.close();

    remove(filename.c_str());
    rename("temp.bin", filename.c_str());

    if (found) {
        cout << "Приміщення видалено." << endl;
    }
    else {
        cout << "Приміщення з таким номером телефону не знайдено." << endl;
    }
}

void EditRoom(const string& filename) {
    int phone;
    cout << "Введіть номер телефону приміщення для редагування: ";
    cin >> phone;

    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    bool found = false;

    while (file.read((char*)&room, sizeof(Room))) {
        if (room.phone_number == phone) {
            found = true;
            cout << "Введіть новий номер приміщення: ";
            cin >> room.room_number;
            cout << "Введіть кількість службовців (до 4): ";
            cin >> room.employee_count;

            if (room.employee_count > 4) {
                cout << "Кількість службовців не може перевищувати 4. Скорочено до 4." << endl;
                room.employee_count = 4;
            }

            cin.ignore();
            for (int i = 0; i < room.employee_count; i++) {
                cout << "Введіть прізвище службовця №" << i + 1 << ": ";
                cin.getline(room.employees[i], 50);
            }

           
            file.write((char*)&room, sizeof(Room));
            cout << "Приміщення оновлено." << endl;
            break;
        }
    }

    if (!found) {
        cout << "Приміщення з таким номером телефону не знайдено." << endl;
    }

    file.close();
}

void PrintAllRooms(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    cout << "=====================================================================" << endl;
    cout << "|  Телефон  | Приміщення | Службовці                                |" << endl;
    cout << "---------------------------------------------------------------------" << endl;

    while (file.read((char*)&room, sizeof(Room))) {
        cout << "| " << setw(9) << room.phone_number << " | " << setw(10) << room.room_number << " | ";
        for (int i = 0; i < room.employee_count; i++) {
            cout << room.employees[i];
            if (i < room.employee_count - 1) cout << ", ";
        }
        cout << " |" << endl;
    }

    cout << "=====================================================================" << endl;
    file.close();
}

void FindByPhoneNumber(const string& filename) {
    int phone;
    cout << "Введіть номер телефону: ";
    cin >> phone;

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    while (file.read((char*)&room, sizeof(Room))) {
        if (room.phone_number == phone) {
            cout << "Номер приміщення: " << room.room_number << endl;
            cout << "Службовці: ";
            for (int i = 0; i < room.employee_count; i++) {
                cout << room.employees[i];
                if (i < room.employee_count - 1) cout << ", ";
            }
            cout << endl;
            file.close();
            return;
        }
    }

    cout << "Приміщення з таким номером телефону не знайдено." << endl;
    file.close();
}

void FindByRoomNumber(const string& filename) {
    int room_num;
    cout << "Введіть номер приміщення: ";
    cin >> room_num;

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    while (file.read((char*)&room, sizeof(Room))) {
        if (room.room_number == room_num) {
            cout << "Номер телефону: " << room.phone_number << endl;
            cout << "Службовці: ";
            for (int i = 0; i < room.employee_count; i++) {
                cout << room.employees[i];
                if (i < room.employee_count - 1) cout << ", ";
            }
            cout << endl;
            file.close();
            return;
        }
    }

    cout << "Приміщення з таким номером не знайдено." << endl;
    file.close();
}

void FindByEmployee(const string& filename) {
    char surname[50];
    cout << "Введіть прізвище службовця: ";
    cin.ignore();
    cin.getline(surname, 50);

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Помилка відкриття файлу." << endl;
        return;
    }

    Room room;
    bool found = false;

    while (file.read((char*)&room, sizeof(Room))) {
        for (int i = 0; i < room.employee_count; i++) {
            if (strcmp(room.employees[i], surname) == 0) {
                cout << "Номер приміщення: " << room.room_number << endl;
                cout << "Номер телефону: " << room.phone_number << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "Службовець не знайдений." << endl;
    }

    file.close();
}
