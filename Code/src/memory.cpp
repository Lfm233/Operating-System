// memory.cpp
#include "../include/memory.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

// ȫ�ֱ���
std::vector<Partition> allocatedPartitions; // �ѷ�������б�
std::vector<Partition> freePartitions;      // δ��������б�
int next_process_id = 1;                    // ����ID�Զ�����

// ��ʼ���̶�����
void initializeFixedPartition() {
    allocatedPartitions.clear();
    freePartitions.clear();
    int total_size = 128; // ���ڴ��С����λ��KB��
    int os_reserved = 4;  // OSռ�õ��ڴ�
    int partition_size = 12; // �̶�������С
    int available_size = total_size - os_reserved;

    // ��ʼ���̶�����
    for (int i = os_reserved; i < total_size; i += partition_size) {
        freePartitions.push_back({i, partition_size, false, -1});
    }
    std::cout << "�̶������ڴ��ѳ�ʼ����\n";
}

// ��ʼ���ɱ����
void initializeVariablePartition() {
    allocatedPartitions.clear();
    freePartitions.clear();
    freePartitions.push_back({4, 124, false, -1}); // �ɱ������ʼΪ124KB����
    std::cout << "�ɱ�����ڴ��ѳ�ʼ����\n";
}

// ����̶��������Զ����� process_id��
bool allocateFixedPartition(int process_size) {
    for (auto it = freePartitions.begin(); it != freePartitions.end(); ++it) {
        if (!it->is_allocated && it->size >= process_size) {
            int start_address = it->start_address;
            int process_id = next_process_id++;

            // �������
            it->is_allocated = true;
            it->process_id = process_id;

            // ��������ӵ��ѷ����б�
            allocatedPartitions.push_back(*it);

            // �ӿ��з����б����Ƴ��÷���
            freePartitions.erase(it);

            std::cout << "���� " << process_id << " ���䵽�̶���������ʼ��ַ: "
                      << start_address << "��\n";

            return true; // ����ɹ�
        }
    }
    std::cout << "����ʧ�ܣ�δ�ҵ����ʵĹ̶����������̴�С: " << process_size << "KB��\n";
    return false; // ����ʧ��
}

// ����ɱ�������Զ����� process_id��
bool allocateVariablePartition(int process_size) {
    for (auto it = freePartitions.begin(); it != freePartitions.end(); ++it) {
        if (it->size >= process_size) {
            int remaining_size = it->size - process_size;
            int start_address = it->start_address;
            int process_id = next_process_id++;

            // �޸Ŀ��з���
            it->start_address += process_size;
            it->size = remaining_size;

            // ��ӵ��ѷ������
            Partition allocated = {start_address, process_size, true, process_id};
            allocatedPartitions.push_back(allocated);

            // ���ʣ��ռ�Ϊ0���Ƴ����з���
            if (remaining_size == 0) {
                freePartitions.erase(it);
            }

            std::cout << "���� " << process_id << " ���䵽�ɱ��������ʼ��ַ: "
                      << start_address << "��\n";
            return true; // ����ɹ�
        }
    }
    std::cout << "����ʧ�ܣ�δ�ҵ��㹻��Ŀ��з��������̴�С: " << process_size << "KB��\n";
    return false; // ����ʧ��
}

// ��ʾ�̶�����״̬
void displayFixedPartition() {
    std::cout << "\n�̶������ڴ�״̬:\n";
    std::cout << std::setw(15) << "��ʼ��ַ" << std::setw(10) << "��С(KB)"
              << std::setw(15) << "״̬" << std::setw(15) << "����ID\n";
    std::cout << "------------------------------------------------------------\n";

    // ��ʾ�ѷ���Ĺ̶�����
    for (const auto& partition : allocatedPartitions) {
        if (partition.size == 12) { // ����ʾ�̶�����������̶�������СΪ12KB��
            std::cout << std::setw(15) << partition.start_address
                      << std::setw(10) << partition.size
                      << std::setw(15) << "�ѷ���"
                      << std::setw(15) << partition.process_id << "\n";
        }
    }

    // ��ʾ���еĹ̶�����
    for (const auto& partition : freePartitions) {
        if (partition.size == 12) { // ����ʾ�̶�����
            std::cout << std::setw(15) << partition.start_address
                      << std::setw(10) << partition.size
                      << std::setw(15) << "����"
                      << std::setw(15) << "-" << "\n";
        }
    }

    std::cout << "============================================================\n";
}

// ��ʾ�ɱ����״̬
void displayVariablePartition() {
    std::cout << "\n�ɱ�����ڴ�״̬:\n";
    std::cout << "�ѷ������:\n";
    if (allocatedPartitions.empty()) {
        std::cout << "��\n";
    } else {
        std::cout << std::setw(15) << "��ʼ��ַ" << std::setw(10) << "��С(KB)" << std::setw(15) << "����ID\n";
        for (const auto& partition : allocatedPartitions) {
            if (partition.size != 12) { // �ų��̶�����
                std::cout << std::setw(15) << partition.start_address
                          << std::setw(10) << partition.size
                          << std::setw(15) << partition.process_id << "\n";
            }
        }
    }

    std::cout << "���з���:\n";
    if (freePartitions.empty()) {
        std::cout << "��\n";
    } else {
        std::cout << std::setw(15) << "��ʼ��ַ" << std::setw(10) << "��С(KB)\n";
        for (const auto& partition : freePartitions) {
            if (partition.size != 12) { // �ų��̶�����
                std::cout << std::setw(15) << partition.start_address
                          << std::setw(10) << partition.size << "\n";
            }
        }
    }

    std::cout << "============================================================\n";
}

// �ϲ����ڵĿ��з���
void mergeFreePartitions() {
    // ����ʼ��ַ������з���
    std::sort(freePartitions.begin(), freePartitions.end(), [](const Partition& a, const Partition& b) {
        return a.start_address < b.start_address;
    });

    // �ϲ����ڵķ���
    for (size_t i = 0; i < freePartitions.size() - 1; ) {
        if (freePartitions[i].start_address + freePartitions[i].size == freePartitions[i + 1].start_address) {
            // �ϲ�����
            freePartitions[i].size += freePartitions[i + 1].size;
            freePartitions.erase(freePartitions.begin() + i + 1);
        } else {
            ++i;
        }
    }
}

// �ͷŷ���
void releasePartition(int start_address) {
    // �����ѷ�������б��ҵ�ƥ��ķ���
    for (auto it = allocatedPartitions.begin(); it != allocatedPartitions.end(); ++it) {
        if (it->start_address == start_address) {
            // ��ӡ�ͷ���Ϣ
            std::cout << "�ͷŷ�������ʼ��ַ: " << start_address << ", ��С: " << it->size << "KB��\n";

            // ���·���״̬Ϊδ����
            it->is_allocated = false;
            it->process_id = -1;

            // �������ƶ������з����б�
            freePartitions.push_back(*it);
            allocatedPartitions.erase(it);

            // �ϲ����з���
            mergeFreePartitions();

            return;
        }
    }

    // ���δ�ҵ���������ӡ������Ϣ
    std::cout << "����δ�ҵ���ʼ��ַΪ " << start_address << " ���ѷ��������\n";
}

// ����ģʽ
void debugMode2() {
    std::cout << "\n=== ����ģʽ ===\n";

    // ��ʼ���̶��������������
    std::cout << "\n--- ��ʼ���̶����� ---\n";
    initializeFixedPartition();
    allocateFixedPartition(10); // ����ID = 1
    allocateFixedPartition(12);  // ����ID = 2
    allocateFixedPartition(20);  // ����ID = 3
    displayFixedPartition();

    // ��ʼ���ɱ�������������
    std::cout << "\n--- ��ʼ���ɱ���� ---\n";
    initializeVariablePartition(); // ��ʼ���ɱ����
    allocateVariablePartition(30); // ����ID = 4 ����30KB
    allocateVariablePartition(50); // ����ID = 5 ����50KB
    displayVariablePartition();

    // �ͷŷ���
    std::cout << "\n--- �ͷŷ��� ---\n";
    releasePartition(34); // �����ͷ���ʼ��ַΪ34�ķ���
    displayVariablePartition();
}

// �û�ģʽ
void userMode2() {
    int choice;
    while (true) {
        std::cout << "\n=== �û�ģʽ ===\n";
        std::cout << "1. ��ʼ���̶�����\n";
        std::cout << "2. ��ʼ���ɱ����\n";
        std::cout << "3. ����̶�����\n";
        std::cout << "4. ����ɱ����\n";
        std::cout << "5. ��ʾ�̶�����״̬\n";
        std::cout << "6. ��ʾ�ɱ����״̬\n";
        std::cout << "7. �ͷŷ���\n";
        std::cout << "8. �˳��û�ģʽ\n";
        std::cout << "����������ѡ��: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                initializeFixedPartition();
                break;
            case 2:
                initializeVariablePartition();
                break;
            case 3: { // ����̶�����
                int process_size;
                std::cout << "��������̴�С (KB): ";
                std::cin >> process_size;
                if (!allocateFixedPartition(process_size)) {
                    std::cout << "����鿴�̶�����״̬��������̴�С��\n";
                }
                break;
            }
            case 4: { // ����ɱ����
                int process_size;
                std::cout << "��������̴�С (KB): ";
                std::cin >> process_size;
                if (!allocateVariablePartition(process_size)) {
                    std::cout << "����鿴�ɱ����״̬��������̴�С��\n";
                }
                break;
            }
            case 5:
                displayFixedPartition();
                break;
            case 6:
                displayVariablePartition();
                break;
            case 7: { // �ͷŷ���
                int start_address;
                std::cout << "�������ͷŷ�������ʼ��ַ: ";
                std::cin >> start_address;
                releasePartition(start_address);
                break;
            }
            case 8:
                std::cout << "�˳��û�ģʽ��\n";
                return;
            default:
                std::cout << "��Ч��ѡ�������ԡ�\n";
        }
    }
}
