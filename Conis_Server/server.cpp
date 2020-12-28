#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
 
#define BUF_LEN 256

#define T_GENERAL_DATA      0x20
#define T_SPECIAL_DATA      0x21

#define R_HEARTBEAT         0x40
#define R_DATA_FRAME        0x44
#define R_ACK_FRAME         0x45

#define G_HEARTBEAT         0x80
#define G_DATA_FRAME        0x84
#define G_ATHEN_REQUEST     0x88
#define G_ATHEN_RESPONSE    0x89

void interpret(uint8_t* buf);
void print_reader_tag(uint8_t* buf);

int main(int argc, char *argv[]) {
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd;
    int len, msg_size;
 
    if(argc != 2) {
        printf("Usage : %s [port]\n", argv[0]);
        exit(0);
    }

    if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    memset(&server_addr, 0x00, sizeof(server_addr));
 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
 
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0) {
        printf("Server : Can't bind local address.\n");
        exit(0);
    }
 
    if(listen(server_fd, 5) < 0) {
        printf("Server : Can't listening connect.\n");
        exit(0);
    }
 
    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");

    socklen_t client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    while( (len = read(client_fd, buffer, BUF_LEN)) != 0 ) {
        interpret((uint8_t*)buffer);
    }
    close(server_fd);
    return 0;
}




void interpret(uint8_t* buf) {
    print_reader_tag(buf);

    if ( buf[0] == R_DATA_FRAME) {
        int index = 15;
        int size, count;

        if ( buf[index] == T_GENERAL_DATA) {
            size = buf[++index];
            count = size / 7;
            index++;

            for (int i = 0 ; i < count ; i++ ) {
                printf("\t TAG_ADDRESS : %02X:%02X:%02X \t/ BATT_LV : %03d \t/ RSSI : %d \t/ BUTTON : 0 \t/ T_OFFSET : %03d \t/ D_OFFSET : %d\n",
                    buf[index+0], buf[index+1], buf[index+2], buf[index+3], buf[index+4]-256, buf[index+5], buf[index+6]);
                index +=7;
            }
        } else if ( buf[index] == T_SPECIAL_DATA ) {
            size = buf[++index];
            count = size / 8;
            index++;

            for (int i = 0 ; i < count ; i++ ) {
                printf("\t TAG_ADDRESS : %02X:%02X:%02X \t/ BATT_LV : %03d \t/ RSSI : %d \t/ BUTTON : %d \t/ T_OFFSET : %03d \t/ D_OFFSET : %d\n",
                    buf[index+0], buf[index+1], buf[index+2], buf[index+3], buf[index+4]-256, buf[index+5], buf[index+6], buf[index+7]);
                index +=8;
            }
        }
    }
}

char* print_time(timeval val) {
    static char now[30];
    struct tm *today;
     
    today = localtime(&val.tv_sec);

    sprintf(now, "%02d:%02d:%02d.%06d", today->tm_hour,
                                        today->tm_min,
                                        today->tm_sec,
                                        val.tv_usec);

    return now;
}

void print_reader_tag(uint8_t* buf) {
    timeval cur;
    short usec;
    int type_usec;

    if ( buf[0] == R_HEARTBEAT ) {
        printf("READER_HEARTBEAT \t/ ");

        memcpy(&cur.tv_sec, buf + 9, 4);
        memcpy(&usec, buf + 13, 2);
        type_usec = usec * 100;
        cur.tv_usec = type_usec;

        printf("MAC_ADDRESS : %02X:%02X:%02X:%02X:%02X:%02X \t/ SEQEUNCE_NUM : %d \t/ TIME : %s\n", buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], print_time(cur));       
    } else if ( buf[0] == R_DATA_FRAME ) {
        printf("READER_DATA_FRMAE \t/ ");

        memcpy(&cur.tv_sec, buf + 9, 4);
        memcpy(&usec, buf + 13, 2);
        type_usec = usec * 100;
        cur.tv_usec = type_usec;

        printf("MAC_ADDRESS : %02X:%02X:%02X:%02X:%02X:%02X \t/ SEQEUNCE_NUM : %d \t/ TIME : %s\n", buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], print_time(cur));
    } else if ( buf[0] == R_ACK_FRAME ) { 
        printf("READER_ACK_FRMAE \t/ ");
    }
}