//
// Created by muyu on 2024/12/23.
//

#ifndef OPERATING_SYSTEM_FILE_H
#define OPERATING_SYSTEM_FILE_H

#include <vector>
#include <string>

class FileManager{
public:
    //���캯��
    explicit FileManager(int start_position,int disk_size = 200);
    //���شŵ�����
    void loadTrackSequence(const std::string& file_path);
    //��ô�ͷ�ܾ���
    int getTotalDistance() const {
        return total_distance;
    }
    // ���̵����㷨
    void fcfs();
    void sstf();
    void scan();

    std::vector<int>track_sequence;
    std::vector<int> access_sequence; // �����ͷ�ķ���˳��

private:
    int current_position;   //��ǰ��ͷλ��
    int disk_size;          //�ŵ��ܴŵ���
    //�ŵ���������
    int total_distance;     //�ܴ�ͷ�ƶ�����

    // ��������
    void printResults(const std::string& method);
};

void debugMode4();
void userMode4();


#endif //OPERATING_SYSTEM_FILE_H
