//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_MEMORY_H
#define OPERATING_SYSTEM_MEMORY_H

#include <vector>
#include <string>

// �����ṹ��
struct Partition {
    int start_address;    // ��ʼ��ַ
    int size;             // ������С��KB��
    bool is_allocated;    // �Ƿ��ѷ���
    int process_id;       // �󶨵Ľ���ID��-1��ʾδ���䣩

    // ���� operator== ����ʹ�� std::remove��
    bool operator==(const Partition& other) const {
        return start_address == other.start_address &&
               size == other.size &&
               is_allocated == other.is_allocated &&
               process_id == other.process_id;
    }
};

// ȫ�ֱ���
extern std::vector<Partition> allocatedPartitions;  // �ѷ�������б�
extern std::vector<Partition> freePartitions;       // δ��������б�

// ���ܺ���
void initializeFixedPartition();
void initializeVariablePartition();
bool allocateFixedPartition(int process_size);
bool allocateVariablePartition(int process_size);
void releasePartition(int start_address);
void mergeFreePartitions();
void displayFixedPartition();
void displayVariablePartition();

void debugMode2();
void userMode2();

#endif //OPERATING_SYSTEM_MEMORY_H
