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
int ifempty=0 ; //判断链表是否是空的的链表
int flag,fh;
char filename1[50],filename2[50],filename3[50],filename4[50],filename5[50];
//定义公交车的结构体
typedef struct Bus{
    //定义结构体成员
    int number;//公交车的编号
    int distance;//运行距离
    int stime;//发车时间
    char help[5];
    struct Bus *prior, *next;  //前驱   后继
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

//返回当前时间
struct tm* get_local_time() {
    time_t current_time = time(NULL);
    struct tm* local_time = localtime(&current_time);
    return local_time;
}
//打开日志
void printFile(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("无法打开文件: %s\n", fileName);
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
    printf("1.查看'求助上传'日志\n");
    printf("2.查看'车辆故障'日志\n");
    printf("3.查看'道路堵塞'日志\n");
    printf("4.查看'交通事故'日志\n");
    printf("5.查看'服务纠纷'日志\n");
    printf("6.查看'报警求助'日志\n");
    printf("请选择(0~6):\n");
    scanf("%d",&x);
    switch (x) {
        case 1:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\求助上传");
            printFile(fileName);
            break;
        case 2:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\车辆故障");
            printFile(fileName);
            break;
        case 3:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\道路堵塞");
            printFile(fileName);
            break;
        case 4:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\交通事故");
            printFile(fileName);
            break;
        case 5:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\服务纠纷");
            printFile(fileName);
            break;
        case 6:
            system("cls");
            strcpy(fileName,"E:\\Bus management system\\报警求助");
            printFile(fileName);
            break;
        case 0:
            system("cls");
            break;
        default:
            printf("请正确输入,温馨提示(0-6)\n");
            getchar(); getchar();
            printf("\n\n");
    }
}
//清空日志
void qk(const char *filename){
    FILE *file;
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        fwrite("", 1, 0, file);
        printf("成功清空日志'%s'\n",filename);
    }
    fclose(file);
}
//
int convert_to_sec(char* time_str) {
    int hours, minutes;
    sscanf(time_str, "%d:%d", &hours, &minutes);
    return hours * 3600 + minutes * 60;
}
//添加路线
// 检查公交车编号是否已经存在
int checkBusNumberExists(const char *filename, const char *busNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // 文件不存在，认为编号不存在
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, busNumber) != NULL) {
            fclose(file);
            return 1; // 找到了匹配的编号
        }
    }
    fclose(file);
    return 0; // 未找到匹配的编号
}
// 向txt文件中添加公交车信息
void addBusInfo(const char *filename, const char *busNumber, const char *startStation, const char *endStation, int numStations, char *stations[]) {
    FILE *file = fopen(filename, "a"); // 以追加模式打开文件
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    // 检查公交车编号是否已经存在
    if (checkBusNumberExists(filename, busNumber)) {
        printf("添加失败：编号 %s 已经存在\n", busNumber);
        fclose(file);
        return;
    }
    // 构造新的公交车信息字符串
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
    // 将新的公交车信息写入文件
    fputs(newBusInfo, file);
    printf("添加成功\n");
    fclose(file);
}
void add_data(){
    char filename[] = "E:\\Bus management system\\路线";
    char busNumber[50];
    char startStation[50], endStation[50];
    int numStations;
    char *stations[50];
    printf("请输入公交车编号：");
    scanf("%s", busNumber);
    printf("请输入起始站名字：");
    scanf("%s",startStation);
    printf("请输入终点站名字：");
    scanf("%s",endStation);
    printf("请输入站数：");
    scanf("%d", &numStations);
    printf("请输入其余各站名字（用空格隔开）：");
    for (int i = 0; i < numStations; i++) {
        stations[i] = (char*)malloc(50 * sizeof(char));
        scanf("%s", stations[i]);
    }
    addBusInfo(filename, busNumber, startStation, endStation, numStations, stations);
    for (int i = 0; i < numStations; i++) {
        free(stations[i]);
    }
}
//删除路线
void deleteBusInfo(const char *filename,const char *filename2,const char *busID) {
    FILE *srcFile = fopen(filename, "r"); // 打开原始文件
    if (srcFile == NULL) {
        perror("打开文件失败");
        return;
    }
    FILE *tmpFile = fopen("temp.txt", "w"); // 创建临时文件
    if (tmpFile == NULL) {
        perror("创建临时文件失败");
        fclose(srcFile);
        return;
    }
    char line[512]; // 用来存储每一行的内容
    while (fgets(line, sizeof(line), srcFile)) { // 读取文件的每一行
        if (strstr(line, busID) == NULL) { // 如果这一行不包含输入的公交车编号
            fputs(line, tmpFile); // 将这一行写入临时文件
        }
    }
    fclose(srcFile); // 关闭原始文件
    fclose(tmpFile); // 关闭临时文件
    remove(filename); // 删除原始文件
    rename("temp.txt", filename); // 将临时文件重命名为原始文件名
    FILE *srcFile1 = fopen(filename2, "r"); // 打开原始文件
    if (srcFile1 == NULL) {
        perror("打开文件失败");
        return;
    }
    FILE *tmpFile1 = fopen("temp.txt", "w"); // 创建临时文件
    if (tmpFile1 == NULL) {
        perror("创建临时文件失败");
        fclose(srcFile);
        return;
    }
    char line1[512]; // 用来存储每一行的内容
    while (fgets(line1, sizeof(line1), srcFile1)) { // 读取文件的每一行
        if (strstr(line1, busID) == NULL) { // 如果这一行不包含输入的公交车编号
            fputs(line1, tmpFile1); // 将这一行写入临时文件
        }
    }
    fclose(srcFile1); // 关闭原始文件
    fclose(tmpFile1); // 关闭临时文件
    remove(filename2); // 删除原始文件
    rename("temp.txt", filename2); // 将临时文件重命名为原始文件名
}
void deletedata(){
    char filename[] = "E:\\Bus management system\\路线";
    char filename2[] = "E:\\Bus management system\\时刻表";
    char busID[50];
    printf("请输入要删除的公交车编号: ");
    scanf("%s", busID); // 获取用户输入的公交车编号
    deleteBusInfo(filename,filename2,busID); // 调用函数删除对应的公交车信息
    printf("已删除编号为 %s 的公交车信息。\n", busID);
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
        printf(" 1. 查看公交车路线图和时刻表\n");
        printf(" 2. 查询车次何时到站\n");
        printf(" 3. 查询车次当前拥挤程度\n");
        printf(" 4. 紧急求助\n");
        printf(" 0. 退出\n");
        printf(" 输入密码进入管理员界面\n");
        printf("=========================================================\n");
        printf("请选择0-4或直接输入密码:\n");
        scanf("%d",&x);
        switch (x) {
            case 0:
                system("cls");
                printf("正在退出系统。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("已退出系统，感谢使用！");
                sleep(1);
                exit(0);
            case 1:
                flag=1;
                while (flag){
                    system("cls");
                    printf("查看时刻表请选择1\n查看路线图请选择2\n返回选择0\n您的选择是：\n");
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
                                    printf("输入0返回\n");
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
                                    printf("输入0返回\n");
                                    scanf("%d",&z);
                                    switch (z) {
                                        case 0:
                                            fh=0;
                                            break;
                                    }
                                }
                                break;
                            default:
                                printf("请正确输入,温馨提示(0-2)\n");
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
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                printf("正在进入管理员界面。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                gly(L);
                break;
            default:
                printf("温馨提示：请正确输入(0-4)或正确输入密码\n");
                getchar(); getchar();
                printf("\n\n");
        }
    }
}
Bus *create(Bus *L){
    printf("    ==========================\n");
    printf("    欢迎使用XX公司公交管理系统\n");
    printf("    ==========================\n");
    struct tm* local_time = get_local_time();
    printf("当前时间是: %d-%d-%d %d:%d:%d\n", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    printf("按回车以进入\n");
    getchar();
    printf("正在进入中，请稍候。");
    sleep(1);
    printf("。");
    sleep(1);
    printf("。");
    sleep(1);
    printf("。");
    sleep(1);
    printf("。");
    ifempty=0;
    L->next=NULL;
    L->prior=NULL;
    L->number=L->distance=L->stime=0;
    strcpy(L->help,"");
    //每次运行程序清空求助上传中内容
    FILE *file;
    file = fopen("E:\\Bus management system\\求助上传", "w");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    fwrite("", 1, 0, file);
    fclose(file);
    return L;
}
//管理员界面
Bus *gly(Bus *L){
    int choice;
    system("cls");
    printf("欢迎进入管理员界面!\n");
    sleep(1);
    while(1) {
        system("cls");
        int z;
        printf("=========================================================");
        printf("\n请选择：\n");
        printf("1.增加线路\n");
        printf("2.删除线路\n");
        printf("3.查看求助信息\n");
        printf("4.清空日志\n");
        printf("0. 返回\n");
        printf("=========================================================\n");
        printf("请选择：（0-4） \n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                system("cls");
                fh=1;
                add_data();
                while (fh){
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                printf("请选择：\n");
                printf("1.车辆故障\n2.道路堵塞\n3.交通事故\n4.服务纠纷\n5.报警求助\n0.返回\n");
                scanf("%d",&c);
                switch (c) {
                    int y;
                    case 1:
                        fh=1;
                        system("cls");
                        strcpy(filename1,"E:\\Bus management system\\车辆故障");
                        qk(filename1);
                        while (fh){
                            printf("输入0返回\n");
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
                        strcpy(filename2,"E:\\Bus management system\\道路堵塞");
                        qk(filename2);
                        while (fh){
                            printf("输入0返回\n");
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
                        strcpy(filename3,"E:\\Bus management system\\交通事故");
                        qk(filename3);
                        while (fh){
                            printf("输入0返回\n");
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
                        strcpy(filename4,"E:\\Bus management system\\服务纠纷");
                        qk(filename4);
                        while (fh){
                            printf("输入0返回\n");
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
                        strcpy(filename5,"E:\\Bus management system\\报警求助");
                        qk(filename5);
                        while (fh){
                            printf("输入0返回\n");
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
                        printf("请正确输入,温馨提示(0-5)\n");
                        getchar(); getchar();
                        printf("\n\n");
                }
                break;
            case 0:
                system("cls");
                printf("正在退出。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("。");
                sleep(1);
                printf("已成功退出！");
                sleep(1);
                system("cls");
                return(L);
            default:
                printf("请正确输入,温馨提示(0-4)\n");
        }
    }
}
//路线图
Bus *Route(Bus *L){
    int c;
    printf("现有车次为：\n");
    BusInfo1 info=read_bus_number();
    printf("输入查询车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
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
    printf("车次编号为: %s\n",info.bus[c]);
    printf("完整路线为：%s\n",info1.zhan);
    printf("========================================================================================================================\n");
}
//时刻表
Bus *Time(Bus *L){
    int c;
    printf("现有车次为：\n");
    BusInfo1 info=read_bus_number();
    printf("输入查询车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    system("cls");
    const char *busnumber = info.bus[c];
    BusInfo info1 = get_bus_info(busnumber);
    printf("=========================================================\n");
    printf("车次编号: %s\n", busnumber);
    printf("首发车时间: %s\n", info1.start_time);
    printf("末班车时间: %s\n", info1.end_time);
    printf("发车间隔: %s\n", info1.interval);
    printf("平均运行时长: %s\n", info1.averagetime);
    printf("=========================================================\n\n");
}
//到达时间
Bus *searchtime(Bus *L){
    int c,averaget,zhanshu,cfsec,fcjg,ddsec,dijizhan;
    double ltime;
    char stationname[50];
    printf("现有车次为：\n");
    BusInfo1 info=read_bus_number();
    printf("输入查询车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            system("cls");
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
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
    printf("输入当前所在站:\n");
    scanf("%s",stationname);
    BusInfo4 info3= get_station(busnumber,stationname);
    dijizhan=info3.nowstation;
    if(dijizhan==-1){
        return 0;
    }
    BusInfo6 info6 = howtime(busnumber,averaget,zhanshu,cfsec,fcjg,ddsec,dijizhan);
    ltime=info6.lasttime;
    if(ltime<=1){
        printf("即将到站\n");
    }else{
        printf("还有大约%.1lfmin到站\n",ltime);
    }
}
//拥挤程度
Bus *degreeofcrowdedness(Bus *L){
    int c,pnum;
    system("cls");
    printf("现有车次为：\n");
    BusInfo1 info=read_bus_number();
    printf("输入查询车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            system("cls");
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        system("cls");
        return 0;
    }
    pnum=rand() % 50 + 1;
    if(pnum>MAX_PASSENGERS){
        printf("该车次目前较拥挤\n");
    }else{
        printf("该车次目前乘客较少\n");
    }
}
//求助功能
Bus *help(Bus *L){
    int x,y;
    while(1){
        system("cls");
        printf("=========================================================");
        printf("\n帮助菜单:\n");
        printf("1. 车辆故障\n");
        printf("2. 道路堵塞\n");
        printf("3. 交通事故\n");
        printf("4. 服务纠纷\n");
        printf("5. 报警求助\n");
        printf("0. 返回\n");
        printf("=========================================================\n");
        printf("请选择：（0-5） \n");
        scanf("%d",&x);
        switch (x) {
            case 1:
                fh=1;
                system("cls");
                Vehicle_Malfunction(L);
                while (fh){
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                    printf("输入0返回\n");
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
                printf("请正确输入,温馨提示(0-5)\n");
        }
    }
}
Bus *Vehicle_Malfunction(Bus *L){
    int c;
    char zhan[50];
    printf("故障车次为：\n");
    BusInfo1 info=read_bus_number();
    printf("输入故障车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    const char *busnumber = info.bus[c];
    struct tm* local_time = get_local_time();
    printf("输入您当前位置：\n");
    scanf("%s",&zhan);
    FILE* file = fopen("E:\\Bus management system\\求助上传", "a");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file, "车辆发生故障\n车次编号：%s\n当前所在位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n", busnumber, zhan,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // 关闭文件
    }
    FILE* file1 = fopen("E:\\Bus management system\\车辆故障", "a");
    if (file1 == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file1, "车次编号：%s\n当前所在位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n", busnumber, zhan,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // 关闭文件
    }
}
void Road_Blockage(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("输入当前堵塞位置：\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\求助上传", "a");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file, "发生道路堵塞\n堵塞位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // 关闭文件
    }
    FILE* file1 = fopen("E:\\Bus management system\\道路堵塞", "a");
    if (file1 == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file1, "堵塞位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // 关闭文件
    }
}
void Traffic_Accident(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("输入发生事故位置：\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\求助上传", "a");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file, "发生交通事故\n事故位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // 关闭文件
    }
    FILE* file1 = fopen("E:\\Bus management system\\交通事故", "a");
    if (file1 == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file1, "事故位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // 关闭文件
    }
}
Bus *Service_Dispute(Bus *L){
    int c;
    char liyou[200];
    BusInfo1 info=read_bus_number();
    printf("选择您想投诉的车次：(1~%d)\n或输入0返回\n",info.op-1);
    scanf("%d",&c);
    while(c<1||c>info.op-1){
        if(c==0){
            fh=0;
            break;
        }
        printf("请重新输入1~%d的数字或者输入0返回:\n",info.op-1);
        scanf("%d",&c);
    }
    if(c==0){
        return 0;
    }
    const char *busnumber = info.bus[c];
    struct tm* local_time = get_local_time();
    printf("输入投诉理由(100字以内)：\n");
    scanf("%s",&liyou);
    FILE* file = fopen("E:\\Bus management system\\求助上传", "a");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file, "发生服务纠纷\n车次编号：%s\n理由：%s\n当前时间:%d-%d-%d %d:%d:%d\n", busnumber, liyou,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // 关闭文件
    }
    FILE* file1 = fopen("E:\\Bus management system\\服务纠纷", "a");
    if (file1 == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        printf("我们会给您更好的体验\n");
        fprintf(file1, "车次编号：%s\n理由：%s\n当前时间:%d-%d-%d %d:%d:%d\n", busnumber, liyou,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // 关闭文件
    }
}
void Call_Police(Bus *L){
    char wz[50];
    struct tm* local_time = get_local_time();
    printf("输入您当前的位置：\n");
    scanf("%s",&wz);
    FILE* file = fopen("E:\\Bus management system\\求助上传", "a");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        fprintf(file, "报警求助\n报警位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file); // 关闭文件
    }
    FILE* file1 = fopen("E:\\Bus management system\\报警求助", "a");
    if (file1 == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }else{
        printf("上传成功\n");
        printf("已为您报警，请在原地等候警察到来\n");
        fprintf(file1, "报警位置：%s\n当前时间:%d-%d-%d %d:%d:%d\n",wz,local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        fprintf(file,"\n");
        fclose(file1); // 关闭文件
    }
}