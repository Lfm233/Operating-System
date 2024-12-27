//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_VIRTUAL_MEMORY_H
#define OPERATING_SYSTEM_VIRTUAL_MEMORY_H

#include <vector>
#include <string>

class VirtualMemory{
public:
    VirtualMemory(int frame_count);
    //����ҳ������
    void loadPageSequence(const std::string& file_path);
    //ҳ������㷨
    void fifo(); //�Ƚ��ȳ��㷨
    void lru();//�������ʹ�õ����㷨
    void lfu(); //�������õ����㷨
    void resetFrames(); // ������÷���
    std::vector<int> page_sequence;
private:
    int frame_count; //�ڴ�֡��
    //ҳ���������
    std::vector<int> memory;    //ģ���ڴ�
    std::vector<int> frames; // �̶���С��֡
};

void debugMode3();
void userMode3();

#endif //OPERATING_SYSTEM_VIRTUAL_MEMORY_H
