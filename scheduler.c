// Simulating Six different scheduling azlgorithms
//  FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK
// DISK Model
//  Ideal Conditions: Rotation and Data transfers are instantaneous
//  Seek time: Moving the head from track X to track Y takes |X-Y| time units

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
// Function prototypes
void fcfs(int total_tracks, int init_head, int request[][2], int num_requests);
void sstf(int total_tracks, int init_head, int request[][2], int num_requests);
void scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void c_scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
void c_look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests);
// Outputs:
// First - sequence of request in ORDER THEY WERE SERVED
// Second - Total Seek Time
// Third - Final Completion Time
void fcfs(int total_tracks, int init_head, int request[][2], int num_requests){
    int seek_time = 0;
    int current_time = 0;
    int current_head = init_head;
    
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
    int completion_time = 0;

    for (int i = 0; i < num_requests; i++) {
        int min_distance = __INT_MAX__;
        int index = -1;

        // Find the arrival time of the chronologically next unserved request
        int next_arrival_time = -1;
        for (int j = 0; j < num_requests; j++) {
            if (request[j][0] != -1) {
                next_arrival_time = request[j][0];
                break;
            }
        }
        // If the disk is empty and the next request is in the future, idle until it arrives
        if (next_arrival_time > completion_time) {
            completion_time = next_arrival_time;
        }

        for (int j = 0; j < num_requests; j++) { 
            int arrival_time = request[j][0];
            if (request[j][0] != -1 && arrival_time <= completion_time) { // Check if request is not yet processed and has arrived
                int distance = abs(init_head - request[j][1]);
                if (distance < min_distance || (distance == min_distance && request[j][1] < request[index][1])) {
                    min_distance = distance;
                    index = j;
                }
            }
        }

        if (index != -1) {
            seek_time += min_distance;
            completion_time += min_distance;
            printf("%d ", request[index][1]);
            init_head = request[index][1];
            request[index][0] = -1; // Mark as processed
        }else{
            for( int j = 0; j < num_requests; j++){
                if (request[j][0] != -1 && request[j][0] > completion_time) {
                    completion_time = request[j][0]; 
                    break;
                }
            }
        }
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", completion_time);

}
void scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){
    total_tracks--;
    int seek_time = 0;
    int completion_time = 0;
    int processed[num_requests];
    for (int i = 0; i < num_requests; i++) {
        processed[i] = 0;
    }

    int counter = 0;

    while(counter < num_requests){
        // Idle until next request arrives if needed
        for (int i = 0; i < num_requests; i++) {
            if (!processed[i] && request[i][0] <= completion_time) {
                break;
            }
            if (!processed[i] && request[i][0] > completion_time) {
                completion_time = request[i][0]; 
                break;
            }
        }

        if (init_direction[0] == 'U'){
            // Find nearest unprocessed request >= current head moving UP
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] >= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }
            
            // Process nearest request if found, otherwise move to end of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;
            } else {
                // Move to end of disk
                int distance = abs(init_head - total_tracks);
                seek_time += distance;
                completion_time += distance;
                init_head = total_tracks;
                init_direction[0] = 'D';
                init_direction[1] = '\0';
            }
        }

        else if (init_direction[0] == 'D'){
            // Find nearest unprocessed request <= current head moving DOWN
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] <= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }

            // Process nearest request if found, otherwise move to start of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;

            } else {
                // Move to start of disk
                int distance = abs(init_head - 0);
                seek_time += distance;
                completion_time += distance;
                init_head = 0;
                init_direction[0] = 'U';
                init_direction[1] = '\0';
            }
        }
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", completion_time);
}
void c_scan(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){
    total_tracks--;
    int seek_time = 0;
    int completion_time = 0;
    int processed[num_requests];
    for (int i = 0; i < num_requests; i++) {
        processed[i] = 0;
    }

    int counter = 0;

    while(counter < num_requests){
        // Idle until next request arrives if needed
        for (int i = 0; i < num_requests; i++) {
            // Check for the possibility of identical consecutive requests 
            if ( !processed[i] && request[i][0] <= completion_time) {
                break;
            }
            if (!processed[i] && request[i][0] > completion_time) {
                completion_time = request[i][0]; 

                break;
            }
        }

        if (init_direction[0] == 'U'){
            // Find nearest unprocessed request >= current head moving UP
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] >= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }
            
            // Process nearest request if found, otherwise move to end of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;
            } else {
                // No more requests in this direction, wrap to beginning and continue same direction
                int distance = abs(init_head - total_tracks);
                seek_time += total_tracks + distance;
                completion_time += total_tracks + distance;
                init_head = 0;
                // Stay in UP direction - circular scan
            }
        }

        else if (init_direction[0] == 'D'){
            // Find nearest unprocessed request <= current head moving DOWN
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] <= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }

            // Process nearest request if found, otherwise move to start of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;
            } else {
                // No more requests in this direction, wrap to end and continue same direction
                int distance = abs(init_head - 0);
                seek_time += total_tracks + distance; // Wrap to end and then move to start
                completion_time += total_tracks + distance;
                init_head = total_tracks;
            }

        }
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", completion_time);
}
void look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){
       total_tracks--;
    int seek_time = 0;
    int completion_time = 0;
    int processed[num_requests];
    for (int i = 0; i < num_requests; i++) {
        processed[i] = 0;
    }

    int counter = 0;

    while(counter < num_requests){
        // Idle until next request arrives if needed
        for (int i = 0; i < num_requests; i++) {
            if (!processed[i] && request[i][0] <= completion_time) {
                break;
            }
            if (!processed[i] && request[i][0] > completion_time) {
                completion_time = request[i][0]; 
                break;
            }
        }

        if (init_direction[0] == 'U'){
            // Find nearest unprocessed request >= current head moving UP
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] >= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }
            
            // Process nearest request if found, otherwise move to end of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;
            } else {
                // If no more requests in this direction, just reverse direction without moving to end/start of disk
                init_direction[0] = 'D';
            }
        }

        else if (init_direction[0] == 'D'){
            // Find nearest unprocessed request <= current head moving DOWN
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] <= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }

            // Process nearest request if found, otherwise move to start of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;

            } else {
                // If no more requests in this direction, just reverse direction without moving to end/start of disk
                init_direction[0] = 'U';
            }
        }
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", completion_time);
    

}
void c_look(int total_tracks, int init_head, int request[][2], char init_direction[], int num_requests){
       total_tracks--;
    int seek_time = 0;
    int completion_time = 0;
    int processed[num_requests];
    for (int i = 0; i < num_requests; i++) {
        processed[i] = 0;
    }

    int counter = 0;

    while(counter < num_requests){
        // Idle until next request arrives if needed
        for (int i = 0; i < num_requests; i++) {
            if (!processed[i] && request[i][0] <= completion_time) {
                break;
            }
            if (!processed[i] && request[i][0] > completion_time) {
                completion_time = request[i][0]; 
                break;
            }
        }

        if (init_direction[0] == 'U'){
            // Find nearest unprocessed request >= current head moving UP
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] >= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }
            
            // Process nearest request if found, otherwise move to end of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;
            } else {
                // We need to jump. Find the absolute lowest track among pending requests
                int lowest_track = __INT_MAX__;
                int target_index = -1;
                
                for (int i = 0; i < num_requests; i++){
                    if (!processed[i] && request[i][0] <= completion_time) {
                        if (request[i][1] < lowest_track) {
                            lowest_track = request[i][1];
                            target_index = i;
                        }
                    }
                }

                // Jump to the lowest track
                if (target_index != -1) {
                    int distance = abs(init_head - request[target_index][1]);
                    seek_time += distance;
                    completion_time += distance;
                    init_head = request[target_index][1];
                }
            }
        }

        else if (init_direction[0] == 'D'){
            // Find nearest unprocessed request <= current head moving DOWN
            int nearest = -1;
            int min_dist = INT_MAX;
            
            // Loop through requests to find the nearest one in the current direction
            for (int i = 0; i < num_requests; i++){
                if (!processed[i] && request[i][0] <= completion_time && request[i][1] <= init_head) {
                    int distance = abs(init_head - request[i][1]);
                    if (distance < min_dist) {
                        min_dist = distance;
                        nearest = i;
                    }
                }
            }

            // Process nearest request if found, otherwise move to start of disk and change direction
            if (nearest != -1) {
                int distance = abs(init_head - request[nearest][1]);
                seek_time += distance;
                completion_time += distance;
                printf("%d ", request[nearest][1]);
                init_head = request[nearest][1];
                processed[nearest] = 1;
                counter++;

            } else {
                // We need to jump. Find the absolute highest track among pending requests
                int highest_track = -1;
                int target_index = -1;
                
                for (int i = 0; i < num_requests; i++){
                    if (!processed[i] && request[i][0] <= completion_time) {
                        if (request[i][1] > highest_track) {
                            highest_track = request[i][1];
                            target_index = i;
                        }
                    }
                }

                // Jump to the highest track
                if (target_index != -1) {
                    int distance = abs(init_head - request[target_index][1]);
                    seek_time += distance;
                    completion_time += distance;
                    init_head = request[target_index][1];
                }
            }
        }
    }

    printf("\n%d\n", seek_time);
    printf("%d\n", completion_time);

}

int main(){
    char alg[10];   
    int tot_tracks, init_head_pos, num_requests;
    char init_direction[10];

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