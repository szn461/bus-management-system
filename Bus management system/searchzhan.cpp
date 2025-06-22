#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct {
    int nowstation;
} BusInfo4;
//��ѯ��ǰվ�ǵڼ�վ
BusInfo4 get_station(const char *busnumber,const char *stationname){
    char line[1000];
    BusInfo4 info = {0};
    FILE *file = fopen("E:\\Bus management system\\·��", "r");
    if (file == NULL) {
        printf("�޷����ļ���\n");
        exit(EXIT_FAILURE);
    }
    int stationIndex = -1; // ��ʼ����վ����Ϊ-1����ʾδ�ҵ�
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, busnumber, strlen(busnumber)) == 0) { // ��鳵�α���Ƿ�ƥ��
            char *token = strtok(line, ":"); // ʹ��ð�ŷָ��ַ�������ȡ��վ��Ϣ����
            if (token != NULL) { // ȷ���г�վ��Ϣ����
                token = strtok(NULL, "->"); // ʹ�ü�ͷ��ð�ŷָ��ַ�������ȡ��վ��Ϣ
                int index = 0;
                while (token != NULL) {
                    if (strcmp(token, stationname) == 0) { // ��鳵վ�Ƿ�ƥ��
                        stationIndex = index + 1; // ���㳵վ����������1��ʼ����
                        break; // �ҵ����˳�ѭ��
                    }
                    token = strtok(NULL, "->"); // �����ָ��ַ�������ȡ��һ����վ��Ϣ
                    index++;
                }
            }
            break; // �ҵ����˳�ѭ��
        }
    }
    fclose(file); // �ر��ļ�
    if (stationIndex != -1) { // ����ҵ��˳�վ�����������������
        info.nowstation=stationIndex;
    } else { // ���û���ҵ���վ�����������������Ϣ
        printf("δ�ҵ���Ӧ�ĳ�վ��\n");
        info.nowstation=stationIndex;
    }
}