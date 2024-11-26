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

// �������
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
    cout << "������ ��'� �����: ";
    getline(cin, filename);

    if (!ValidateFile(filename)) {
        ofstream file(filename, ios::binary);
        file.close();
    }

    int choice;
    do {
        cout << "\n������ ��:\n";
        cout << "1 - ������ ���������\n";
        cout << "2 - �������� ���������\n";
        cout << "3 - ���������� ���������\n";
        cout << "4 - ������� �� ���������\n";
        cout << "5 - ����� �� ������� ��������\n";
        cout << "6 - ����� �� ������� ���������\n";
        cout << "7 - ����� �� �������� ���������\n";
        cout << "0 - �����\n";
        cout << "��� ����: ";
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
            cout << "�����..." << endl;
            break;
        default:
            cout << "������� ����. ��������� �� ���." << endl;
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
    cout << "������ ����� �������� (2 �����): ";
    cin >> room.phone_number;
    cout << "������ ����� ���������: ";
    cin >> room.room_number;
    cout << "������ ������� ���������� (�� 4): ";
    cin >> room.employee_count;

    if (room.employee_count > 4) {
        cout << "ʳ������ ���������� �� ���� ������������ 4. ��������� �� 4." << endl;
        room.employee_count = 4;
    }

    cin.ignore();
    for (int i = 0; i < room.employee_count; i++) {
        cout << "������ ������� ��������� �" << i + 1 << ": ";
        cin.getline(room.employees[i], 50);
    }

    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    file.write((char*)&room, sizeof(Room));
    file.close();
    cout << "��������� ������." << endl;
}

void DeleteRoom(const string& filename) {
    int phone;
    cout << "������ ����� �������� ��������� ��� ���������: ";
    cin >> phone;

    ifstream file(filename, ios::binary);
    ofstream temp("temp.bin", ios::binary);

    if (!file || !temp) {
        cout << "������� �������� �����." << endl;
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
        cout << "��������� ��������." << endl;
    }
    else {
        cout << "��������� � ����� ������� �������� �� ��������." << endl;
    }
}

void EditRoom(const string& filename) {
    int phone;
    cout << "������ ����� �������� ��������� ��� �����������: ";
    cin >> phone;

    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    Room room;
    bool found = false;

    while (file.read((char*)&room, sizeof(Room))) {
        if (room.phone_number == phone) {
            found = true;
            cout << "������ ����� ����� ���������: ";
            cin >> room.room_number;
            cout << "������ ������� ���������� (�� 4): ";
            cin >> room.employee_count;

            if (room.employee_count > 4) {
                cout << "ʳ������ ���������� �� ���� ������������ 4. ��������� �� 4." << endl;
                room.employee_count = 4;
            }

            cin.ignore();
            for (int i = 0; i < room.employee_count; i++) {
                cout << "������ ������� ��������� �" << i + 1 << ": ";
                cin.getline(room.employees[i], 50);
            }

           
            file.write((char*)&room, sizeof(Room));
            cout << "��������� ��������." << endl;
            break;
        }
    }

    if (!found) {
        cout << "��������� � ����� ������� �������� �� ��������." << endl;
    }

    file.close();
}

void PrintAllRooms(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    Room room;
    cout << "=====================================================================" << endl;
    cout << "|  �������  | ��������� | ���������                                |" << endl;
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
    cout << "������ ����� ��������: ";
    cin >> phone;

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    Room room;
    while (file.read((char*)&room, sizeof(Room))) {
        if (room.phone_number == phone) {
            cout << "����� ���������: " << room.room_number << endl;
            cout << "���������: ";
            for (int i = 0; i < room.employee_count; i++) {
                cout << room.employees[i];
                if (i < room.employee_count - 1) cout << ", ";
            }
            cout << endl;
            file.close();
            return;
        }
    }

    cout << "��������� � ����� ������� �������� �� ��������." << endl;
    file.close();
}

void FindByRoomNumber(const string& filename) {
    int room_num;
    cout << "������ ����� ���������: ";
    cin >> room_num;

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    Room room;
    while (file.read((char*)&room, sizeof(Room))) {
        if (room.room_number == room_num) {
            cout << "����� ��������: " << room.phone_number << endl;
            cout << "���������: ";
            for (int i = 0; i < room.employee_count; i++) {
                cout << room.employees[i];
                if (i < room.employee_count - 1) cout << ", ";
            }
            cout << endl;
            file.close();
            return;
        }
    }

    cout << "��������� � ����� ������� �� ��������." << endl;
    file.close();
}

void FindByEmployee(const string& filename) {
    char surname[50];
    cout << "������ ������� ���������: ";
    cin.ignore();
    cin.getline(surname, 50);

    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "������� �������� �����." << endl;
        return;
    }

    Room room;
    bool found = false;

    while (file.read((char*)&room, sizeof(Room))) {
        for (int i = 0; i < room.employee_count; i++) {
            if (strcmp(room.employees[i], surname) == 0) {
                cout << "����� ���������: " << room.room_number << endl;
                cout << "����� ��������: " << room.phone_number << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "���������� �� ���������." << endl;
    }

    file.close();
}
