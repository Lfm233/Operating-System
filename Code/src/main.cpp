//
// Created by muyu on 2024/12/23.
//



#include <iostream>

void processManagement();
void memoryManagement();
void virtualMemoryManagement();
void fileManagement();

int main() {
    int choice;

    while (true) {
        // ��ʾ���˵�
        std::cout << "\n=== ����ϵͳģ����� ===\n";
        std::cout << "1. ���̹���ģ��\n";
        std::cout << "2. �ڴ����ģ��\n";
        std::cout << "3. �����ڴ����ģ��\n";
        std::cout << "4. �ļ�����ģ��\n";
        std::cout << "5. �˳�����\n";
        std::cout << "����������ѡ��";
        std::cin >> choice;

        switch (choice) {
            case 1:
                processManagement();
                break;
            case 2:
                memoryManagement();
                break;
            case 3:
                virtualMemoryManagement();
                break;
            case 4:
                fileManagement();
                break;
            case 5:
                std::cout << "�������˳����ټ���\n";
                return 0;
            default:
                std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    }
}
