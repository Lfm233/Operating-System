//
// Created by muyu on 2024/12/24.
//


#include <iostream>
#include "../include/process.h"
#include "../include/memory.h"
#include "../include/virtual_memory.h"
#include "../include/file.h"

// ͨ�ò˵���ʾ����
int displayMenu(const std::vector<std::string>& options) {
    int choice;
    while (true) {
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 1 << ". " << options[i] << "\n";
        }
        std::cout << "����������ѡ��";
        std::cin >> choice;

        if (choice >= 1 && choice <= options.size()) {
            return choice;
        } else {
            std::cout << "��Чѡ�������ԡ�\n";
        }
    }
}

// ���̹���ģ��
void processManagement() {
    std::vector<std::string> options = {"����ģʽ", "�û�ģʽ", "�������˵�"};
    while (true) {
        std::cout << "\n=== ���̹���ģ�� ===\n";
        int choice = displayMenu(options);

        switch (choice) {
            case 1:
                debugMode1();
                break;
            case 2:
                userMode1();
                break;
            case 3:
                std::cout << "�������˵���\n";
                return;
        }
    }
}


void memoryManagement() {
    int mode;

    while (true) {
        // ��ʾ�ڴ����˵�
        std::cout << "\n=== �ڴ����ģ�� ===\n";
        std::cout << "1. ����ģʽ\n";
        std::cout << "2. �û�ģʽ\n";
        std::cout << "3. �������˵�\n";
        std::cout << "����������ѡ��: ";
        std::cin >> mode;

        switch (mode) {
            case 1:
                std::cout << "\n�������ģʽ...\n";
                debugMode2(); // ���õ���ģʽ����
                break;
            case 2:
                std::cout << "\n�����û�ģʽ...\n";
                userMode2(); // �����û�ģʽ����
                break;
            case 3:
                std::cout << "�������˵���\n";
                return; // �������˵�
            default:
                std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    }
}

void virtualMemoryManagement() {
    int mode;

    while (true) {
        std::cout << "\n=== �����ڴ����ģ�� ===\n";
        std::cout << "1. ����ģʽ\n";
        std::cout << "2. �û�ģʽ\n";
        std::cout << "3. �������˵�\n";
        std::cout << "����������ѡ��";
        std::cin >> mode;

        switch (mode) {
            case 1:
                debugMode3();
                break;
            case 2:
                userMode3();
                break;
            case 3:
                std::cout << "�������˵���\n";
                return;
            default:
                std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    }
}

void fileManagement() {
    int mode;

    while (true) {
        std::cout << "\n=== �ļ�����ģ�� ===\n";
        std::cout << "1. ����ģʽ\n";
        std::cout << "2. �û�ģʽ\n";
        std::cout << "3. �������˵�\n";
        std::cout << "����������ѡ��";
        std::cin >> mode;

        switch (mode) {
            case 1:
                debugMode4();
                break;
            case 2:
                userMode4();
                break;
            case 3:
                std::cout << "�������˵���\n";
                return;
            default:
                std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    }
}


