#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
/* close */

pthread_mutex_t lock;
int over=0;
float bigArray[1000];
int size=0;

void appendToArray(float number){
    printf("size = %d\n", size);
    bigArray[size++] = number;
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(float arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    float L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(float arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void* clientThread(void* clientsocket){
    int sockfd = *((int*)clientsocket);
    printf("Hello client %d: \n", sockfd);

    while(!over) {


        int n,i, tmp;

        //pthread_mutex_lock(&lock);
        tmp = recv(sockfd, &n, sizeof(n), 0);
        //pthread_mutex_unlock(&lock);
        if (tmp < 0) {
            perror("Error on receiving N from client");
        }
        printf("Client %d sent n = %d\n", sockfd, n);
       // pthread_mutex_unlock(&lock);

        if (n == 0) {
            //printf("hey client %d\n", sockfd);
            pthread_mutex_lock(&lock);
            over = 1;
            printf("Client %d sent N=0. Goodbye!", sockfd);
            pthread_mutex_unlock(&lock);

        }
        if(over){
            pthread_mutex_lock(&lock);
            printf("\nSending %d to thread %d\n", over, sockfd);
            send(sockfd, &over, sizeof(over), 0);
            pthread_mutex_unlock(&lock);
            break;
        }
        else
        {pthread_mutex_lock(&lock);
            printf("\nSending %d to thread %d\n", over, sockfd);
            send(sockfd, &over, sizeof(over), 0);
            pthread_mutex_unlock(&lock);}

        if(n!=0)
        {

            if(over) break;
            pthread_mutex_lock(&lock);
            // float *array = malloc(sizeof(float) * n);
            printf("Thread %d sent the array: \n", sockfd);
            for ( i = 0; i < n; i++) {
                float number;
                tmp = recv(sockfd, &number, sizeof(number), 0);
                //array[i] = number;
                if (tmp < 0) {
                    perror("Error on receiving from client");
                }
                appendToArray(number);
                printf("%.2f ", number);
            }

              pthread_mutex_unlock(&lock);
        }



    }


    int j;

   // for(j=0;j<size;j++)
    //    printf("%.2f , ",bigArray[j]);

   // pthread_mutex_lock(&lock);
    mergeSort(bigArray, 0 ,size-1);
    printf("\nThread %d got out\n", sockfd);
    send(sockfd, &size, sizeof(size), 0);
    for(j=0; j < size;j++){
        send(sockfd, &bigArray[j], sizeof(bigArray[j]), 0);
    }
   // printf("size = %d\n", size);
   // for(j=0;j<size;j++)
   //     printf("j = %d ",j);
  //  printf("hey??");
   // pthread_mutex_unlock(&lock);

    close(sockfd);
    return NULL;

}


int main(int argc, char** argv) {
    int sock_server, sock_client;
    struct sockaddr_in server, client;
    int len;

    if(argc < 2){
        perror("Please provide port as a command line argument");
        exit(1);
    }

    sock_server = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_server<0){
        perror("Error on creating server socket");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    int opt = 1;
    if(setsockopt(sock_server, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt))<0){
        perror("setsockopt");
        exit(1);
    }

    //bind the socket to the server

    if(bind(sock_server, (struct sockaddr*)&server, sizeof(server))<0){
        perror("Error binding address to socket");
        exit(1);
    }

    if(listen(sock_server, 5)<0){
        perror("Error on listen");
        exit(1);
    }

    len = sizeof(struct sockaddr_in);
    bzero(&client, len);

   // sock_client = accept(sock_server, (struct sockaddr*)&client, (socklen_t*)&len);




    pthread_t thread;
    pthread_mutex_init(&lock, NULL);

    int i=0;
    pthread_t * threads = malloc(5*sizeof(pthread_t));

    while(1){
        bzero(&client, len);
        sock_client = accept(sock_server, (struct sockaddr*)&client, (socklen_t*)&len);
        if(sock_client<0){
            perror("Error on accepting client!");
        }
        else {
            i = i+1;
            pthread_create(&threads[i], NULL, clientThread, (void*)&sock_client);
            pthread_detach(threads[i]);
            printf("goodbye %d\n", i);
        }

        for(int j=0;j<=i;j++)
            pthread_join(threads[i], NULL);
        i=0;

    }

    close(sock_client);

    return 0;
}
