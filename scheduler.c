// Simulating Six different scheduling azlgorithms
//  FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK
// DISK Model
//  Ideal Conditions: Rotation and Data transfers are instantaneous
//  Seek time: Moving the head from track X to track Y takes |X-Y| time units

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Function prototypes

void fcfs(int total_tracks, int init_head, int request[][2], int num_requests);
void sstf(int total_tracks, int init_head, int request[][2], int num_requests);
void scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void c_scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void c_look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
int compare(const void *a, const void *b);

// Outputs:
// First - sequence of request in ORDER THEY WERE SERVED
// Second - Total Seek Time
// Thirds - Final Completion Time
void fcfs(int total_tracks, int init_head, int request[][2], int num_requests){
    int seek_time = 0;
    int current_time = 0;
    int current_head = init_head;
    
    printf("\n");
    
    // FCFS: process requests in order of arrival time
    for(int i = 0; i < num_requests; i++){
        int arrival_time = request[i][0];
        int track = request[i][1];
        
        // If request hasn't arrived yet, wait for it
        if (arrival_time > current_time) {
            current_time = arrival_time;
        }
        
        // Calculate seek time and update current time
        int distance = abs(current_head - track);
        seek_time += distance;
        current_time += distance;
        
        printf("%d ", track);
        current_head = track;
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", current_time);
}


void sstf(int total_tracks, int init_head, int request[][2], int num_requests){
    int seek_time = 0;
    int len = *(&request + 1) - request;
    int completion_time = 0;

    printf("\n");

}
void scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){

}
void c_scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){

}
void look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){

}
void c_look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){

}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b); 
}

int main(){
    char alg[] = "FCFS";
    int tot_tracks, init_head_pos, num_requests;
    char init_direction[4];

    scanf("%s %d %d", alg, &tot_tracks, &init_head_pos);

    if (strcmp(alg, "SCAN") == 0 || strcmp(alg, "C-SCAN") == 0 || strcmp(alg, "LOOK") == 0 || strcmp(alg, "C-LOOK") == 0){
        scanf("%s", init_direction);
    }

    scanf("%d", &num_requests);

    int requests[num_requests][2];
    for(int i = 0; i < num_requests; i++){
        scanf("%d %d", &requests[i][0], &requests[i][1]);
    }

    if (strcmp(alg, "FCFS") == 0){
        fcfs(tot_tracks, init_head_pos, requests, num_requests);
    }
    else if (strcmp(alg, "SSTF") == 0){
        sstf(tot_tracks, init_head_pos, requests, num_requests);
    }
    else if (strcmp(alg, "SCAN") == 0){
        scan(tot_tracks, init_head_pos, requests, init_direction, num_requests);
    }
    else if (strcmp(alg, "C-SCAN") == 0){
        c_scan(tot_tracks, init_head_pos, requests, init_direction, num_requests);
    }
    else if (strcmp(alg, "LOOK") == 0){
        look(tot_tracks, init_head_pos, requests, init_direction, num_requests);
    }
    else if (strcmp(alg, "C-LOOK") == 0){
        c_look(tot_tracks, init_head_pos, requests, init_direction, num_requests);
    }
    else{
        printf("Invalid Algorithm\n");
    }


}