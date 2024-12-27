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
        // 显示主菜单
        std::cout << "\n=== 操作系统模拟程序 ===\n";
        std::cout << "1. 进程管理模块\n";
        std::cout << "2. 内存管理模块\n";
        std::cout << "3. 虚拟内存管理模块\n";
        std::cout << "4. 文件管理模块\n";
        std::cout << "5. 退出程序\n";
        std::cout << "请输入您的选择：";
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
                std::cout << "程序已退出，再见！\n";
                return 0;
            default:
                std::cout << "无效的选择！请重试。\n";
        }
    }
}
