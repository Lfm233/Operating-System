//
// Created by muyu on 2024/12/24.
//


#include <iostream>
#include "../include/process.h"
#include "../include/memory.h"
#include "../include/virtual_memory.h"
#include "../include/file.h"

// 通用菜单显示函数
int displayMenu(const std::vector<std::string>& options) {
    int choice;
    while (true) {
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 1 << ". " << options[i] << "\n";
        }
        std::cout << "请输入您的选择：";
        std::cin >> choice;

        if (choice >= 1 && choice <= options.size()) {
            return choice;
        } else {
            std::cout << "无效选择，请重试。\n";
        }
    }
}

// 进程管理模块
void processManagement() {
    std::vector<std::string> options = {"调试模式", "用户模式", "返回主菜单"};
    while (true) {
        std::cout << "\n=== 进程管理模块 ===\n";
        int choice = displayMenu(options);

        switch (choice) {
            case 1:
                debugMode1();
                break;
            case 2:
                userMode1();
                break;
            case 3:
                std::cout << "返回主菜单。\n";
                return;
        }
    }
}


void memoryManagement() {
    int mode;

    while (true) {
        // 显示内存管理菜单
        std::cout << "\n=== 内存管理模块 ===\n";
        std::cout << "1. 调试模式\n";
        std::cout << "2. 用户模式\n";
        std::cout << "3. 返回主菜单\n";
        std::cout << "请输入您的选择: ";
        std::cin >> mode;

        switch (mode) {
            case 1:
                std::cout << "\n进入调试模式...\n";
                debugMode2(); // 调用调试模式函数
                break;
            case 2:
                std::cout << "\n进入用户模式...\n";
                userMode2(); // 调用用户模式函数
                break;
            case 3:
                std::cout << "返回主菜单。\n";
                return; // 返回主菜单
            default:
                std::cout << "无效的选择！请重试。\n";
        }
    }
}

void virtualMemoryManagement() {
    int mode;

    while (true) {
        std::cout << "\n=== 虚拟内存管理模块 ===\n";
        std::cout << "1. 调试模式\n";
        std::cout << "2. 用户模式\n";
        std::cout << "3. 返回主菜单\n";
        std::cout << "请输入您的选择：";
        std::cin >> mode;

        switch (mode) {
            case 1:
                debugMode3();
                break;
            case 2:
                userMode3();
                break;
            case 3:
                std::cout << "返回主菜单。\n";
                return;
            default:
                std::cout << "无效的选择，请重试。\n";
        }
    }
}

void fileManagement() {
    int mode;

    while (true) {
        std::cout << "\n=== 文件管理模块 ===\n";
        std::cout << "1. 调试模式\n";
        std::cout << "2. 用户模式\n";
        std::cout << "3. 返回主菜单\n";
        std::cout << "请输入您的选择：";
        std::cin >> mode;

        switch (mode) {
            case 1:
                debugMode4();
                break;
            case 2:
                userMode4();
                break;
            case 3:
                std::cout << "返回主菜单。\n";
                return;
            default:
                std::cout << "无效的选择，请重试。\n";
        }
    }
}


