#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

typedef struct
{
    char ip[50];
    float latency;
} HostInfo;

float ParsePingResponse(const char *bematch);
float TestNetwoklatency(HostInfo *host);
int ScanLanHost(HostInfo hosts[], const char *net_addr, int masknum);

int main()
{
    // scan a lan
    const char *net_addr = "192.168.0.0"; // the start ip address of a LAN
    int masknum = 24; // subnet mask
    int numhosts = pow(2, 32 - masknum) - 2;

    HostInfo hosts[numhosts];
    memset(hosts, 0x00, sizeof(HostInfo) * numhosts);
    int ret;
    if ((ret = ScanLanHost(hosts, net_addr, masknum)) == 0) {
        printf("\nValid hosts:\n");
        int seq = 1;
        for (int k = 0; k < numhosts; k++) {
            if (0.0 < hosts[k].latency && hosts[k].latency < 1000.0) printf("\t%3.i IP %15.15s Latency %6.6fms\n", seq++, hosts[k].ip, hosts[k].latency);
        }
    }
    else {
        printf("Scan LAN hosts failed: %i\n", ret);
    }
}


float TestNetwoklatency(HostInfo *host)
{
    char *ip = host->ip;
    FILE *fp;
    char buffer[1000];
    int i = 0;
    char command[100] = {'\0'};
    sprintf(command, "ping -c 5 %s", ip);
    printf("Testing:%s\n", command);
    fp = popen(command, "r");
    while ((buffer[i++] = fgetc(fp)) != EOF)
        ;
    buffer[--i] = '\0';
    pclose(fp);

    // printf("respose is %s\n", buffer);
    float avg_time = ParsePingResponse(buffer);
    host->latency = avg_time;
    if (avg_time > 1000.0) {
        printf("Detination %s, unreachable.\n", ip);
    }
    else
        printf("Detination %s, average latency is: %fms\n", ip, avg_time);
    return avg_time;
}

float ParsePingResponse(const char *bematch)
{
    float avg_time = 10000.0;
    char pattern[] = ".*received, ([0-9]+)%.*round-trip min/avg/max/stddev = ([0-9]+.[0-9]+)/([0-9]+.[0-9]+)/([0-9]+.[0-9]+)/.*";
    // char pattern_time[] = ".*round-trip min/avg/max/stddev = ([0-9]+.[0-9]/[0-9]+.[0-9]/[0-9]+.[0-9]/.*";
    char errbuf[1024];
    char match[1000];
    regex_t reg;
    int err, nm = 10;
    regmatch_t pmatch[nm];
    if (regcomp(&reg, pattern, REG_EXTENDED) < 0) {
        regerror(err, &reg, errbuf, sizeof(errbuf));
        printf("\nError:%s\n", errbuf);
    }
    err = regexec(&reg, bematch, nm, pmatch, 0);

    if (err == REG_NOMATCH) {
        // printf("\nNo match\n");
        return 10000.0;
    }
    else if (err) {
        regerror(err, &reg, errbuf, sizeof(errbuf));
        printf("err:%s\n", errbuf);
        return 10000.0;
    }

    // for (int i = 1; i < 10 && pmatch[i].rm_so != -1; i++) {
    //     int len = pmatch[i].rm_eo - pmatch[i].rm_so;
    //     if (len) {
    //         memset(match, '\0', sizeof(match));
    //         memcpy(match, bematch + pmatch[i].rm_so, len);
    //         printf("match is %s\n", match);
    //     }
    // }
    memset(match, '\0', sizeof(match));
    memcpy(match, bematch + pmatch[3].rm_so, pmatch[3].rm_eo - pmatch[3].rm_so);
    // printf("match is %s\n", match);
    // float avg_time = atof(match);
    sscanf(match, "%f", &avg_time);
    return avg_time;
}

int ScanLanHost(HostInfo hosts[], const char *net_addr, int masknum)
{
    // if the subnet mask is not 24, it's more complicated to enumerate the valid host IP of the LAN.
    if (masknum < 24) {
        printf("Currently only subnet mask number > 24 is accepted.\n");
        return -1;
    }
    int numhosts = pow(2.0, 32 - masknum) - 2;
    printf("Total number of hosts %i\n", numhosts);
    char hostip[50];
    int ipaddr[4];
    sscanf(net_addr, "%i.%i.%i.%i", ipaddr, ipaddr + 1, ipaddr + 2, ipaddr + 3);

    pthread_t thrds[numhosts];
    for (int i = 0; i < numhosts; i++) {
        memset(hosts[i].ip, '\0', sizeof(hosts[i].ip));
        sprintf(hosts[i].ip, "%i.%i.%i.%i", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3] + i + 1);
        pthread_create(thrds + i, NULL, (void *(*)(void *)) & TestNetwoklatency, (void *)&hosts[i]);
    }
    for (int i = 0; i < numhosts; i++) {
        pthread_join(thrds[i], NULL);
    }
    return 0;
}
