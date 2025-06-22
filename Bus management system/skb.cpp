#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char start_time[20];
    char end_time[20];
    char interval[20];
    char averagetime[20];
} BusInfo;
//��ȡʱ�̱�
BusInfo get_bus_info(const char *busnumber) {
    FILE *file = fopen("E:\\Bus management system\\ʱ�̱�", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[256];
    BusInfo info = {"", "", "",""};
    while (fgets(line, sizeof(line), file)) {
        // ʹ�� strncmp ȷ������ busnumber ��ͷ�����Һ���������ǿո���з�
        if (strncmp(line, busnumber, strlen(busnumber)) == 0 &&
            (line[strlen(busnumber)] == ' ' || line[strlen(busnumber)] == '\n')) {
            char *token = strtok(line, " ");
            while (token != NULL) {
                if (strstr(token, "�׷���ʱ��:")) {
                    sscanf(token + 11, "%5s", info.start_time);
                } else if (strstr(token, "ĩ�೵ʱ��:")) {
                    sscanf(token + 11, "%5s", info.end_time);
                } else if (strstr(token, "�������:")) {
                    sscanf(token + 9, "%6s", info.interval);
                } else if (strstr(token, "ƽ������ʱ��:")) {
                    sscanf(token + 13, "%6s", info.averagetime);
                }
                token = strtok(NULL, " ");
                }
                break;
            }
        }
    fclose(file);
    return info;
}


