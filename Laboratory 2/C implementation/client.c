#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char** argv) {
    int sockfd;
    struct sockaddr_in server;
    float number;

    if(argc<2){
        printf("<port>");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        perror("Error on creating client socket");
        exit(1);
    }
    int opt = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))<0){
        perror("setsockopt");
        exit(1);
    }


    bzero(&server, sizeof(server));
    server.sin_addr.s_addr = inet_addr("192.168.100.40");
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));

    //connect to the server
    printf("hey\n");

    int rc;
    rc = connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    printf("hey\n");

    if(rc<0){
        perror("cannot connect to the server");
        exit(1);
    }
    printf("hey\n");

    int n;
    printf("hey\n");



    int over =  0;
    int msg;
    printf("hey\n");
    while(!over) {
        printf("Size of the array: ");
        scanf("%d",&n);
        //n = htonl(n);

        printf("\nsending data to server\n");
        send(sockfd, &n, sizeof(n), 0);

        recv(sockfd, &msg, sizeof(msg), 0);
        printf("msg = %d\n", msg);

        if(msg == 1 || n==0){
            over = 1;
            break;
        }

        char num[100];
        for (int i = 0; i < n; i++) {
            printf("\nRead arr[%d]: ", i);
            scanf("%f", &number);
            send(sockfd, &number, sizeof(number), 0);
        }


    }


    printf("\n");

    int size;
    recv(sockfd, &size, sizeof(size), 0);
    printf("Size = %d\n", size);

    for(int i=0;i<size;i++){
        float no;
        recv(sockfd, &no, sizeof(no), 0);
        printf("%.2f ;", no);
    }


    return 0;
}
