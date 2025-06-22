#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include "skb.cpp"
#include "lxt.cpp"
#include "busnum.cpp"
#include "time.cpp"
#include "searchzhan.cpp"
#define MAX_PASSENGERS 30
int ifempty=0 ; //�ж������Ƿ��ǿյĵ�����
int flag,fh;
char filename1[50],filename2[50],filename3[50],filename4[50],filename5[50];
//���幫�����Ľṹ��
typedef struct Bus{
    //����ṹ���Ա
    int number;//�������ı��
    int distance;//���о���
    int stime;//����ʱ��
    char help[5];
    struct Bus *prior, *next;  //ǰ��   ���
}Bus;

Bus *create(Bus *L);
Bus *gly(Bus *L);
Bus *Route(Bus *L);
Bus *Time(Bus *L);
Bus *searchtime(Bus *L);
Bus *degreeofcrowdedness(Bus *L);
Bus *help(Bus *L);
Bus *Vehicle_Malfunction(Bus *L);
void Road_Blockage(Bus *L);
void Traffic_Accident(Bus *L);
Bus *Service_Dispute(Bus *L);
void Call_Police(Bus *L);

//���ص�ǰʱ��
struct tm* get_local_time() {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    return local_time;
}
//����־
void printFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("�޷����ļ�: %s\n", fileName);
        return;
    }
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
}
void openlog(){
    int x;
    char fileName[100];
    printf("1.�鿴'�����ϴ�'��־\n");
    printf("2.�鿴'��������'��־\n");
    printf("3.�鿴'��·����'��־\n");
    printf("4.�鿴'��ͨ�¹�'��־\n");
    printf("5.�鿴'�������'��־\n");
    printf("6.�鿴'��������'��־\n");
    printf("��ѡ��(0~6):\n");
    scanf("%d",&x);
    switch (x) {
        case 1:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\�����ϴ�");
            printFile(fileName);
            break;
        case 2:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\��������");
            printFile(fileName);
            break;
        case 3:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\��·����");
            printFile(fileName);
            break;
        case 4:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\��ͨ�¹�");
            printFile(fileName);
            break;
        case 5:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\�������");
            printFile(fileName);
            break;
        case 6:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\��������");
            printFile(fileName);
            break;
        case 0:
            system("cls");
            break;
        default:
            printf("����ȷ����,��ܰ��ʾ(0-6)\n");
            getchar(); getchar();
            printf("\n\n");
    }
}
//�����־
void qk(const char *filename){
    FILE *file;
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        fwrite("", 1, 0, file);
        printf("�ɹ������־'%s'\n",filename);
    }
    fclose(file);
}
//
int convert_to_sec(char* time_str) {
    int hours, minutes;
    sscanf(time_str, "%d:%d", &hours, &minutes);
    return hours * 3600 + minutes * 60;
}
//���·��
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
void add_data(){
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
}
//ɾ��·��
void deleteBusInfo(const char *filename,const char *filename2,const char *busID) {
    FILE *srcFile = fopen(filename, "r"); // ��ԭʼ�ļ�
    if (srcFile == NULL) {
        perror("���ļ�ʧ��");
        return;
    }
    FILE *tmpFile = fopen("temp.txt", "w"); // ������ʱ�ļ�
    if (tmpFile == NULL) {
        perror("������ʱ�ļ�ʧ��");
        fclose(srcFile);
        return;
    }
    char line[512]; // �����洢ÿһ�е�����
    while (fgets(line, sizeof(line), srcFile)) { // ��ȡ�ļ���ÿһ��
        if (strstr(line, busID) == NULL) { // �����һ�в���������Ĺ��������
            fputs(line, tmpFile); // ����һ��д����ʱ�ļ�
        }
    }
    fclose(srcFile); // �ر�ԭʼ�ļ�
    fclose(tmpFile); // �ر���ʱ�ļ�
    remove(filename); // ɾ��ԭʼ�ļ�
    rename("temp.txt", filename); // ����ʱ�ļ�������Ϊԭʼ�ļ���
    FILE *srcFile1 = fopen(filename2, "r"); // ��ԭʼ�ļ�
    if (srcFile1 == NULL) {
        perror("���ļ�ʧ��");
        return;
    }
    FILE *tmpFile1 = fopen("temp.txt", "w"); // ������ʱ�ļ�
    if (tmpFile1 == NULL) {
        perror("������ʱ�ļ�ʧ��");
        fclose(srcFile);
        return;
    }
    char line1[512]; // �����洢ÿһ�е�����
    while (fgets(line1, sizeof(line1), srcFile1)) { // ��ȡ�ļ���ÿһ��
        if (strstr(line1, busID) == NULL) { // �����һ�в���������Ĺ��������
            fputs(line1, tmpFile1); // ����һ��д����ʱ�ļ�
        }
    }
    fclose(srcFile1); // �ر�ԭʼ�ļ�
    fclose(tmpFile1); // �ر���ʱ�ļ�
    remove(filename2); // ɾ��ԭʼ�ļ�
    rename("temp.txt", filename2); // ����ʱ�ļ�������Ϊԭʼ�ļ���
}
void deletedata(){
    char filename[] = "E:\\Bus management system\\·��";
    char filename2[] = "E:\\Bus management system\\ʱ�̱�";
    char busID[50];
    printf("������Ҫɾ���Ĺ��������: ");
    scanf("%s", busID); // ��ȡ�û�����Ĺ��������
    deleteBusInfo(filename,filename2,busID); // ���ú���ɾ����Ӧ�Ĺ�������Ϣ
    printf("��ɾ�����Ϊ %s �Ĺ�������Ϣ��\n", busID);
}
int main() {
    int x,y,z,p,i=1;
    srand(time(NULL));
    time_t secounds;
    secounds= time(NULL);
    Bus *L;
    if(!(L=(Bus *) malloc(sizeof (Bus)))){
        printf("\n");
        exit(1);
    }
    create(L);
    sleep(1);
    system("cls");
    while(1){
        printf("=========================================================\n");
        printf(" 1. �鿴������·��ͼ��ʱ�̱�\n");
        printf(" 2. ��ѯ���κ�ʱ��վ\n");
        printf(" 3. ��ѯ���ε�ǰӵ���̶�\n");
        printf(" 4. ��������\n");
        printf(" 0. �˳�\n");
        printf(" ��������������Ա����\n");
        printf("=========================================================\n");
        printf("��ѡ��0-4��ֱ����������:\n");
        scanf("%d",&x);
        switch (x) {
            case 0:
                system("cls");
                printf("�����˳�ϵͳ��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("���˳�ϵͳ����лʹ�ã�");
                sleep(1);
                exit(0);
            case 1:
                flag=1;
                while (flag){
                    system("cls");
                    printf("�鿴ʱ�̱���ѡ��1\n�鿴·��ͼ��ѡ��2\n����ѡ��0\n����ѡ���ǣ�\n");
                    scanf("%d",&y);
                        switch (y) {
                            case 0:
                                flag=0;
                                system("cls");
                                break;
                            case 1:
                                fh=1;
                                system("cls");
                                Time(L);
                                while (fh){
                                    printf("����0����\n");
                                    scanf("%d",&z);
                                    switch (z) {
                                        case 0:
                                            fh=0;
                                            break;
                                    }
                                }
                                break;
                            case 2:
                                fh=1;
                                system("cls");
                                Route(L);
                                while (fh){
                                    printf("����0����\n");
                                    scanf("%d",&z);
                                    switch (z) {
                                        case 0:
                                            fh=0;
                                            break;
                                    }
                                }
                                break;
                            default:
                                printf("����ȷ����,��ܰ��ʾ(0-2)\n");
                                getchar(); getchar();
                                printf("\n\n");
                        }
                }
                break;
            case 2:
                fh=1;
                system("cls");
                searchtime(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&z);
                    switch (z) {
                        case 0:
                            fh=0;
                            system("cls");
                            break;
                    }
                }
                break;
            case 3:
                fh=1;
                degreeofcrowdedness(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&z);
                    switch (z) {
                        case 0:
                            fh=0;
                            system("cls");
                            break;
                    }
                }
                break;
            case 4:
                help(L);
                break;
            case 123:
                system("cls");
                printf("���ڽ������Ա���档");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                gly(L);
                break;
            default:
                printf("��ܰ��ʾ������ȷ����(0-4)����ȷ��������\n");
                getchar(); getchar();
                printf("\n\n");
        }
    }
}
Bus *create(Bus *L){
    printf("    ==========================\n");
    printf("    ��ӭʹ��XX��˾��������ϵͳ\n");
    printf("    ==========================\n");
    struct tm* local_time = get_local_time();
    printf("��ǰʱ����: %d-%d-%d %d:%d:%d\n", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    printf("���س��Խ���\n");
    getchar();
    printf("���ڽ����У����Ժ�");
    sleep(1);
    printf("��");
    sleep(1);
    printf("��");
    sleep(1);
    printf("��");
    sleep(1);
    printf("��");
    ifempty=0;
    L->next=NULL;
    L->prior=NULL;
    L->number=L->distance=L->stime=0;
    strcpy(L->help,"");
    //ÿ�����г�����������ϴ�������
    FILE *file;
    file = fopen("E:\\Bus management system\\�����ϴ�", "w");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }
    fwrite("", 1, 0, file);
    fclose(file);
    return L;
}
//����Ա����
Bus *gly(Bus *L){
    int choice;
    system("cls");
    printf("��ӭ�������Ա����!\n");
    sleep(1);
    while(1) {
        system("cls");
        int z;
        printf("=========================================================");
        printf("\n��ѡ��\n");
        printf("1.������·\n");
        printf("2.ɾ����·\n");
        printf("3.�鿴������Ϣ\n");
        printf("4.�����־\n");
        printf("0. ����\n");
        printf("=========================================================\n");
        printf("��ѡ�񣺣�0-4�� \n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                system("cls");
                fh=1;
                add_data();
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&z);
                    switch (z) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 2:
                system("cls");
                fh=1;
                deletedata();
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&z);
                    switch (z) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 3:
                system("cls");
                fh=1;
                openlog();
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&z);
                    switch (z) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 4:
                int c;
                system("cls");
                printf("��ѡ��\n");
                printf("1.��������\n2.��·����\n3.��ͨ�¹�\n4.�������\n5.��������\n0.����\n");
                scanf("%d",&c);
                switch (c) {
                    int y;
                    case 1:
                        fh=1;
                        system("cls");
                        strcpy(filename1,"E:\\Bus management system\\��������");
                        qk(filename1);
                        while (fh){
                            printf("����0����\n");
                            scanf("%d",&y);
                            switch (y) {
                                case 0:
                                    fh=0;
                                    break;
                            }
                        }
                        break;
                    case 2:
                        fh=1;
                        system("cls");
                        strcpy(filename2,"E:\\Bus management system\\��·����");
                        qk(filename2);
                        while (fh){
                            printf("����0����\n");
                            scanf("%d",&y);
                            switch (y) {
                                case 0:
                                    system("cls");
                                    fh=0;
                                    break;
                            }
                        }
                        break;
                    case 3:
                        fh=1;
                        system("cls");
                        strcpy(filename3,"E:\\Bus management system\\��ͨ�¹�");
                        qk(filename3);
                        while (fh){
                            printf("����0����\n");
                            scanf("%d",&y);
                            switch (y) {
                                case 0:
                                    fh=0;
                                    break;
                            }
                        }
                        break;
                    case 4:
                        fh=1;
                        system("cls");
                        strcpy(filename4,"E:\\Bus management system\\�������");
                        qk(filename4);
                        while (fh){
                            printf("����0����\n");
                            scanf("%d",&y);
                            switch (y) {
                                case 0:
                                    fh=0;
                                    break;
                            }
                        }
                        break;
                    case 5:
                        fh=1;
                        system("cls");
                        strcpy(filename5,"E:\\Bus management system\\��������");
                        qk(filename5);
                        while (fh){
                            printf("����0����\n");
                            scanf("%d",&y);
                            switch (y) {
                                case 0:
                                    fh=0;
                                    break;
                            }
                        }
                        break;
                    case 0:
                        system("cls");
                        break;
                    default:
                        printf("����ȷ����,��ܰ��ʾ(0-5)\n");
                        getchar(); getchar();
                        printf("\n\n");
                }
                break;
            case 0:
                system("cls");
                printf("�����˳���");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("��");
                sleep(1);
                printf("�ѳɹ��˳���");
                sleep(1);
                system("cls");
                return(L);
            default:
                printf("����ȷ����,��ܰ��ʾ(0-4)\n");
        }
    }
}
//·��ͼ
Bus *Route(Bus *L){
    int c;
    printf("���г���Ϊ��\n");
    BusInfo1 info=read_bus_number();
    printf("�����ѯ���Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        fh=0;
        return 0;
    }
    system("cls");
    const char *busnumber = info.bus[c];
    BusInfo1 info1= get_bus_road(busnumber);
    printf("========================================================================================================================\n");
    printf("\n");
    printf("���α��Ϊ: %s\n",info.bus[c]);
    printf("����·��Ϊ��%s\n",info1.zhan);
    printf("========================================================================================================================\n");
}
//ʱ�̱�
Bus *Time(Bus *L){
    int c;
    printf("���г���Ϊ��\n");
    BusInfo1 info=read_bus_number();
    printf("�����ѯ���Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    system("cls");
    const char *busnumber = info.bus[c];
    BusInfo info1 = get_bus_info(busnumber);
    printf("=========================================================\n");
    printf("���α��: %s\n", busnumber);
    printf("�׷���ʱ��: %s\n", info1.start_time);
    printf("ĩ�೵ʱ��: %s\n", info1.end_time);
    printf("�������: %s\n", info1.interval);
    printf("ƽ������ʱ��: %s\n", info1.averagetime);
    printf("=========================================================\n\n");
}
//����ʱ��
Bus *searchtime(Bus *L){
    int c,averaget,zhanshu,cfsec,fcjg,ddsec,dijizhan;
    double ltime;
    char stationname[50];
    printf("���г���Ϊ��\n");
    BusInfo1 info=read_bus_number();
    printf("�����ѯ���Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            system("cls");
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        system("cls");
        return 0;
    }
    system("cls");
    const char *busnumber = info.bus[c];
    BusInfo info1 = get_bus_info(busnumber);
    sscanf(info1.averagetime, "%d", &averaget);
    BusInfo2 info2 = get_station_number(busnumber);
    zhanshu=info2.number_of_stations;
    cfsec= convert_to_sec(info1.start_time);
    sscanf(info1.interval,"%d",&fcjg);
    ddsec= convert_to_sec(info1.end_time);
    printf("���뵱ǰ����վ:\n");
    scanf("%s",stationname);
    BusInfo4 info3= get_station(busnumber,stationname);
    dijizhan=info3.nowstation;
    if(dijizhan==-1){
        return 0;
    }
    BusInfo6 info6 = howtime(busnumber,averaget,zhanshu,cfsec,fcjg,ddsec,dijizhan);
    ltime=info6.lasttime;
    if(ltime<=1){
        printf("������վ\n");
    }else{
        printf("���д�Լ%.1lfmin��վ\n",ltime);
    }
}
//ӵ���̶�
Bus *degreeofcrowdedness(Bus *L){
    int c,pnum;
    system("cls");
    printf("���г���Ϊ��\n");
    BusInfo1 info=read_bus_number();
    printf("�����ѯ���Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            system("cls");
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        system("cls");
        return 0;
    }
    pnum=rand() % 50 + 1;
    if(pnum>MAX_PASSENGERS){
        printf("�ó���Ŀǰ��ӵ��\n");
    }else{
        printf("�ó���Ŀǰ�˿ͽ���\n");
    }
}
//��������
Bus *help(Bus *L){
    int x,y;
    while(1){
        system("cls");
        printf("=========================================================");
        printf("\n�����˵�:\n");
        printf("1. ��������\n");
        printf("2. ��·����\n");
        printf("3. ��ͨ�¹�\n");
        printf("4. �������\n");
        printf("5. ��������\n");
        printf("0. ����\n");
        printf("=========================================================\n");
        printf("��ѡ�񣺣�0-5�� \n");
        scanf("%d",&x);
        switch (x) {
            case 1:
                fh=1;
                system("cls");
                Vehicle_Malfunction(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&y);
                    switch (y) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 2:
                fh=1;
                system("cls");
                Road_Blockage(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&y);
                    switch (y) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 3:
                fh=1;
                system("cls");
                Traffic_Accident(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&y);
                    switch (y) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 4:
                fh=1;
                system("cls");
                Service_Dispute(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&y);
                    switch (y) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 5:
                fh=1;
                system("cls");
                Call_Police(L);
                while (fh){
                    printf("����0����\n");
                    scanf("%d",&y);
                    switch (y) {
                        case 0:
                            fh=0;
                            break;
                    }
                }
                break;
            case 0:
                system("cls");
                return(L);
            default:
                printf("����ȷ����,��ܰ��ʾ(0-5)\n");
        }
    }
}
Bus *Vehicle_Malfunction(Bus *L){
    int c;
    char zhan[50];
    printf("���ϳ���Ϊ��\n");
    BusInfo1 info=read_bus_number();
    printf("������ϳ��Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    const char *busnumber = info.bus[c];
    struct tm* local_time = get_local_time();
    printf("��������ǰλ�ã�\n");
    scanf("%s",&zhan);
    FILE* file = fopen("E:\\Bus management system\\�����ϴ�", "a");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file, "������������\n���α�ţ�%s\n��ǰ����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n", busnumber, zhan,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // �ر��ļ�
    }
    FILE* file1 = fopen("E:\\Bus management system\\��������", "a");
    if (file1 == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file1, "���α�ţ�%s\n��ǰ����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n", busnumber, zhan,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // �ر��ļ�
    }
}
void Road_Blockage(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("���뵱ǰ����λ�ã�\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\�����ϴ�", "a");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file, "������·����\n����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // �ر��ļ�
    }
    FILE* file1 = fopen("E:\\Bus management system\\��·����", "a");
    if (file1 == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file1, "����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // �ر��ļ�
    }
}
void Traffic_Accident(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("���뷢���¹�λ�ã�\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\�����ϴ�", "a");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file, "������ͨ�¹�\n�¹�λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // �ر��ļ�
    }
    FILE* file1 = fopen("E:\\Bus management system\\��ͨ�¹�", "a");
    if (file1 == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file1, "�¹�λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // �ر��ļ�
    }
}
Bus *Service_Dispute(Bus *L){
    int c;
    char liyou[200];
    BusInfo1 info=read_bus_number();
    printf("ѡ������Ͷ�ߵĳ��Σ�(1~%d)\n������0����\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("����������1~%d�����ֻ�������0����:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    const char *busnumber = info.bus[c];
    struct tm* local_time = get_local_time();
    printf("����Ͷ������(100������)��\n");
    scanf("%s",&liyou);
    FILE* file = fopen("E:\\Bus management system\\�����ϴ�", "a");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file, "�����������\n���α�ţ�%s\n���ɣ�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n", busnumber, liyou,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // �ر��ļ�
    }
    FILE* file1 = fopen("E:\\Bus management system\\�������", "a");
    if (file1 == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        printf("���ǻ�������õ�����\n");
        fprintf(file1, "���α�ţ�%s\n���ɣ�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n", busnumber, liyou,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // �ر��ļ�
    }
}
void Call_Police(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("��������ǰ��λ�ã�\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\�����ϴ�", "a");
    if (file == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        fprintf(file, "��������\n����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // �ر��ļ�
    }
    FILE* file1 = fopen("E:\\Bus management system\\��������", "a");
    if (file1 == NULL) {
        printf("�޷����ļ�\n");
        exit(EXIT_FAILURE);
    }else{
        printf("�ϴ��ɹ�\n");
        printf("��Ϊ������������ԭ�صȺ򾯲쵽��\n");
        fprintf(file1, "����λ�ã�%s\n��ǰʱ��:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // �ر��ļ�
    }
}