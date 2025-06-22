#include <stdio.h>
#include <time.h>
typedef struct {
    double lasttime;
} BusInfo6;
BusInfo6 howtime(const char *busnumber,const int averaget,const int zhanshu,const int cfsec,const int fcjg,const int ddsec,const int dijizhan){
    int i;
    BusInfo6 info6={0};
    // 获取当前时间
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    // 提取小时、分钟和秒
    int hours = local->tm_hour;
    int minutes = local->tm_min;
    int seconds = local->tm_sec;
    printf("现在时间为: %d:%d:%d\n",hours,minutes,seconds);
    // 转换为总秒数
    int totalSeconds = hours * 3600 + minutes * 60 + seconds;
    double averagezhan;
    averagezhan=averaget*1.0/zhanshu;
    for(i=cfsec;i<=ddsec;i+=fcjg*60){
        if(totalSeconds>=ddsec+averaget*60||totalSeconds<=cfsec){
            printf("查询车次还未到发车时间或已超过运营时间\n");
            break;
        }else if(totalSeconds<=i+dijizhan*averagezhan){
            info6.lasttime=(i+dijizhan*averagezhan-totalSeconds)/60;
            break;
        }
    }
    return info6;
}