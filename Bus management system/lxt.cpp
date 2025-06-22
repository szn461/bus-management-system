#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char busnumber[50];
    char zhan[200];
    int op;
    char bus[200][50];
} BusInfo1;
//������г��α��
BusInfo1 read_bus_number(){
    FILE *file = fopen("E:\\Bus management system\\·��", "r"); // ���ļ�
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }
    char line[1000]; // ���ڴ洢��ȡ��ÿһ���ı�
    BusInfo1 info = {"","",1,""};
    while (fgets(line, sizeof(line), file)) { // ���ж�ȡ�ļ�����
        char *token = strtok(line, ":"); // �ָ�ÿһ���ı�����ȡ���α��
        if (token != NULL) {
            char *remember[] = {token}; // �����α�Ŵ洢��������
            int rememberSize = sizeof(remember) / sizeof(char*); // ��������Ԫ�ظ���
            for (int i = 0; i < rememberSize; i++) { // ��˳��������α��
                printf("%d.%s\n",info.op ,remember[i]);
                strcpy(info.bus[info.op], token);
                info.op++;
            }
        }
    }
    fclose(file);
    return info;
}
//��ȡ·��
BusInfo1 get_bus_road(const char *busnumber){
    FILE *file = fopen("E:\\Bus management system\\·��", "r");
    if (!file) {
        perror("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }
    char line[256];
    BusInfo1 info = {"",""};
    while (fgets(line, sizeof(line), file)) {
        char *token,*token2;
        token=strtok(line, ":");
        token2= strtok(NULL,":");
        if (strcmp(token,busnumber)==0) {
            strcpy(info.zhan,token2);
            break;
        }
    }
    fclose(file);
    return info;
}