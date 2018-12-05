#include "arp.h"
#include "zenarp.h"
void  arp()
{
    int cl_fd, len;
    int sfd = 0;
    fd_set rd_set;
    struct arp_packet rcvBuffer;
    struct device_node zenith_device;
    char buf[1000];
    if(-1 == (sfd = CreartSerSock()))
        return ;

    memset(&zenith_device, '\0', sizeof(zenith_device));
    while(1)
    {
        FD_ZERO(&rd_set);
        FD_SET(sfd, &rd_set);
        if((cl_fd=select(sfd+1,&rd_set,NULL,NULL, NULL))<=0){
            if(errno == EINTR){
                Log0("select continue errno: [%s]", strerror(errno));
                continue;
            }else{
                Log0("select error errno: [%s] need start ser sock", strerror(errno));
                close(sfd);
                if(-1 == (sfd = CreartSerSock()))
                    return;
                else
                    continue;
            }
        }
#if 0
        if(FD_ISSET(sfd, &rd_set)){
            memset(&rcvBuffer, 0, sizeof(rcvBuffer));
            if((len=recv(sfd, &rcvBuffer, sizeof(rcvBuffer),0))<=0){
                if(errno == EINTR){
                    Log0("recv continue errno: [%s]", strerror(errno));
                    continue;
                }else{
                    Log0("recv error errno: [%s] need start ser sock", strerror(errno));
                    close(sfd);
                    if(-1 == (sfd = CreartSerSock()))
                        return;
                    else
                        continue;
                }
            }
        }
        printf("get len -> %d , expect %d\n\n",len,sizeof(rcvBuffer));
        if(len!=sizeof(rcvBuffer))
            continue;
#else
        if(FD_ISSET(sfd, &rd_set)){
            memset(&buf, 0, sizeof(buf));
            if((len=recv(sfd, &buf, sizeof(buf),0))<=0){
                if(errno == EINTR){
                    Log0("recv continue errno: [%s]", strerror(errno));
                    continue;
                }else{
                    Log0("recv error errno: [%s] need start ser sock", strerror(errno));
                    close(sfd);
                    if(-1 == (sfd = CreartSerSock()))
                        return;
                    else
                        continue;
                }
            }
        }
        printf("get len -> %d , expect %d\n\n",len,sizeof(buf));
        if(len!=sizeof(rcvBuffer))
            continue;
#endif
#if 0
        if(strstr(rcvBuffer.padding, "zenreq") || strstr(rcvBuffer.padding, "zenset")){
            Log0("Send Arp struct: ========>>>>>>>>");
            Log0("mac_target:   [%.2x:%.2x:%.2x:%.2x:%.2x:%.2x]",
                    rcvBuffer.mac_target[0],
                    rcvBuffer.mac_target[1],
                    rcvBuffer.mac_target[2],
                    rcvBuffer.mac_target[3],
                    rcvBuffer.mac_target[4],
                    rcvBuffer.mac_target[5]);
            Log0("mac_source:   [%.2x:%.2x:%.2x:%.2x:%.2x:%.2x]",
                    rcvBuffer.mac_source[0],
                    rcvBuffer.mac_source[1],
                    rcvBuffer.mac_source[2],
                    rcvBuffer.mac_source[3],
                    rcvBuffer.mac_source[4],
                    rcvBuffer.mac_source[5]);

            Log0("ethertype:  [0x%x]", ntohs(rcvBuffer.ethertype));
            Log0("hw_type:    [0x%x]", ntohs(rcvBuffer.hw_type));
            Log0("proto_type: [0x%x]", ntohs(rcvBuffer.proto_type));
            Log0("mac_addr_len: [%d]", rcvBuffer.mac_addr_len);
            Log0("ip_addr_len:  [%d]", rcvBuffer.ip_addr_len);
            Log0("operation_code: [0x%x]", ntohs(rcvBuffer.operation_code));
            Log0("mac_sender:   [%.2x:%.2x:%.2x:%.2x:%.2x:%.2x]",
                    rcvBuffer.mac_sender[0],
                    rcvBuffer.mac_sender[1],
                    rcvBuffer.mac_sender[2],
                    rcvBuffer.mac_sender[3],
                    rcvBuffer.mac_sender[4],
                    rcvBuffer.mac_sender[5]);
            Log0("mac_receiver: [%.2x:%.2x:%.2x:%.2x:%.2x:%.2x]",
                    rcvBuffer.mac_receiver[0],
                    rcvBuffer.mac_receiver[1],
                    rcvBuffer.mac_receiver[2],
                    rcvBuffer.mac_receiver[3],
                    rcvBuffer.mac_receiver[4],
                    rcvBuffer.mac_receiver[5]);
            Log0("ip_sender:    [%d.%d.%d.%d]",
                    rcvBuffer.ip_sender[0],
                    rcvBuffer.ip_sender[1],
                    rcvBuffer.ip_sender[2],
                    rcvBuffer.ip_sender[3]);
            Log0("ip_receiver:    [%d.%d.%d.%d]",
                    rcvBuffer.ip_receiver[0],
                    rcvBuffer.ip_receiver[1],
                    rcvBuffer.ip_receiver[2],
                    rcvBuffer.ip_receiver[3]);
            Log0("padding:     [%s]", rcvBuffer.padding);
            Log0("Send Arp struct: >>>>>>>>========");

            //旧版本
            #if 0
            int ik;
            for(ik=0; ik<6; ik++){
                sourcemac[ik] = rcvBuffer.mac_source[ik];
            }
            for(ik=0; ik<4; ik++){
                sourceip[ik] = rcvBuffer.ip_sender[ik];
            }
            #endif

            if(strstr(rcvBuffer.padding, "zenset")){
                struct in_addr p;
                unsigned char * ptr;
                char * tmp;
                ptr = (unsigned char *)&p;
                if(tmp = strtok(rcvBuffer.padding, ","))
                {
                    if(tmp = strtok(NULL, ","))
                        ptr2ip(tmp, zenith_device.ip);
                    if(tmp = strtok(NULL, ","))
                        ptr2ip(tmp, zenith_device.netmask);
                    if(tmp = strtok(NULL, ","))
                        ptr2ip(tmp, zenith_device.gateway);
                }
                Log0("ip:          [%s]", zenith_device.ip);
                Log0("netmask:     [%s]", zenith_device.netmask);
                Log0("gateway:     [%s]", zenith_device.gateway);
                if(strcmp(zenith_device.ip, local_ip)
                    || strcmp(zenith_device.netmask, local_netmask)
                    || strcmp(zenith_device.gateway, local_gateway)){
                    modify_config_ini(zenith_device.ip, zenith_device.netmask,zenith_device.gateway);
                    system("sync");
                    system("reboot");
                }else
                    Log0("net param not changle");
            }else{
                //新版本
                int ik;
                for(ik=0; ik<6; ik++){
                    sourcemac[ik] = rcvBuffer.mac_source[ik];
                }
                char * tmp;
                if(tmp = strtok(rcvBuffer.padding, ",")){
                    if(tmp = strtok(NULL, ","))
                        ptr2ipint(tmp, sourceip);
                }
                g_flag = 1;
            }
        }
#endif
        ////////////////

    }
    return ;
}
