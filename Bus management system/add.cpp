#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// ��鹫��������Ƿ��Ѿ�����
int checkBusNumberExists(const char *filename, const char *busNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // �ļ������ڣ���Ϊ��Ų�����
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, busNumber) != NULL) {
            fclose(file);
            return 1; // �ҵ���ƥ��ı��
        }
    }
    fclose(file);
    return 0; // δ�ҵ�ƥ��ı��
}
// ��txt�ļ�����ӹ�������Ϣ
void addBusInfo(const char *filename, const char *busNumber, const char *startStation, const char *endStation, int numStations, char *stations[]) {
    FILE *file = fopen(filename, "a"); // ��׷��ģʽ���ļ�
    if (file == NULL) {
        printf("�޷����ļ� %s\n", filename);
        return;
    }
    // ��鹫��������Ƿ��Ѿ�����
    if (checkBusNumberExists(filename, busNumber)) {
        printf("���ʧ�ܣ���� %s �Ѿ�����\n", busNumber);
        fclose(file);
        return;
    }
    // �����µĹ�������Ϣ�ַ���
    char newBusInfo[256];
    fprintf(file,"\n");
    snprintf(newBusInfo, sizeof(newBusInfo), "%s(%s->%s):%s", busNumber, startStation, endStation, startStation);
    strcat(newBusInfo, "->");
    strcat(newBusInfo, stations[0]);
    for (int i = 1; i < numStations ; i++) {
        strcat(newBusInfo, "->");
        strcat(newBusInfo, stations[i]);
    }
    strcat(newBusInfo, "->");
    strcat(newBusInfo, endStation);
    // ���µĹ�������Ϣд���ļ�
    fputs(newBusInfo, file);
    printf("��ӳɹ�\n");
    fclose(file);
}
int main() {
    char filename[] = "E:\\Bus management system\\·��";
    char busNumber[50];
    char startStation[50], endStation[50];
    int numStations;
    char *stations[50];
    printf("�����빫������ţ�");
    scanf("%s", busNumber);
    printf("��������ʼվ���֣�");
    scanf("%s",startStation);
    printf("�������յ�վ���֣�");
    scanf("%s",endStation);
    printf("������վ����");
    scanf("%d", &numStations);
    printf("�����������վ���֣��ÿո��������");
    for (int i = 0; i < numStations; i++) {
        stations[i] = (char*)malloc(50 * sizeof(char));
        scanf("%s", stations[i]);
    }
    addBusInfo(filename, busNumber, startStation, endStation, numStations, stations);
    for (int i = 0; i < numStations; i++) {
        free(stations[i]);
    }
    return 0;
}