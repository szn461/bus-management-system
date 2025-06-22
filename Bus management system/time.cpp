#include <stdio.h>
#include <time.h>
typedef struct {
    double lasttime;
} BusInfo6;
BusInfo6 howtime(const char *busnumber,const int averaget,const int zhanshu,const int cfsec,const int fcjg,const int ddsec,const int dijizhan){
    int i;
    BusInfo6 info6={0};
    // ��ȡ��ǰʱ��
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    // ��ȡСʱ�����Ӻ���
    int hours = local->tm_hour;
    int minutes = local->tm_min;
    int seconds = local->tm_sec;
    printf("����ʱ��Ϊ: %d:%d:%d\n",hours,minutes,seconds);
    // ת��Ϊ������
    int totalSeconds = hours * 3600 + minutes * 60 + seconds;
    double averagezhan;
    averagezhan=averaget*1.0/zhanshu;
    for(i=cfsec;i<=ddsec;i+=fcjg*60){
        if(totalSeconds>=ddsec+averaget*60||totalSeconds<=cfsec){
            printf("��ѯ���λ�δ������ʱ����ѳ�����Ӫʱ��\n");
            break;
        }else if(totalSeconds<=i+dijizhan*averagezhan){
            info6.lasttime=(i+dijizhan*averagezhan-totalSeconds)/60;
            break;
        }
    }
    return info6;
}