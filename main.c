#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"


#define LINESIZE 255


struct Node {

        int vertex_no;
        int edge_weight;
        int min_distance_val; //Holds the minimum distance from base node to initial node
        int min_distance_parent; //Holds the minimum distance parent node index
        struct Node* parent_next; //To parent nodes
        struct Node* next; //To adjacent nodes

};


/*Prototypes*/
struct Node** read_file(int*,int*,int*,int*);
struct Node* create_node(int,int);
void add_node(struct Node*,struct Node*);
void add_parent_node(struct Node*,struct Node*);
int find_paths(struct Node**,int,int,int);
int bfs_possible_paths(struct Node**,int,int,int);
void print_shortest_path(struct Node**,int,int,int,int);
void print_possible_paths(struct Node**,struct Node*,int,int,int*,int);
int get_distance(struct Node**,int,int);
void find_directly_connected_nodes(struct Node**,int);
void find_distant_nodes(struct Node**,int);
void bfs_longest_path(struct Node**,int,int,int*);
int find_max_node(int* distance_values,int vertex_cnt);



/*Reads graph from file and stores as adjacency list*/
struct Node** read_file(int* base_node,int* dest_node,int* vertex_cnt,int* edge_cnt) {

        FILE* fp;
        char line_buffer[LINESIZE];
        struct Node** adj_list;
        char* p;
        int head_index;
        int vertex_no;
        int edge_weight;
        struct Node* new_node;



        fp = fopen("input.txt","r");

        if (fp == NULL) {
                printf("File is not opened!\n");
                exit(-1);
        }


        /*Reading vertex count and edge count*/
        fgets(line_buffer,LINESIZE,fp);
        p=strtok(line_buffer," ");
        *vertex_cnt = atoi(p);
        p=strtok(NULL," ");
        *edge_cnt = atoi(p);


        adj_list = (struct Node**)malloc( (*vertex_cnt)*sizeof(struct Node*) );

        if (adj_list == NULL) {
                printf("Memory is not allocated!\n");
                exit(-3);
        }


        /*Reading base node and destination node*/
        fgets(line_buffer,LINESIZE,fp);
        p=strtok(line_buffer," ");
        *base_node = atoi(p);
        p=strtok(NULL," ");
        *dest_node= atoi(p);

        /*Reading adjacent nodes of head nodes*/
        fgets(line_buffer,LINESIZE,fp);
        while (!feof(fp)) {

                p=strtok(line_buffer," ");
                head_index = atoi(p);

                if(adj_list[head_index] == NULL)
                        adj_list[head_index] = create_node(head_index,0);


                p=strtok(NULL," ");
                vertex_no = atoi(p);

                p=strtok(NULL," ");
                edge_weight = atoi(p);

                /*Creating a new adjacent node with vertex no and edge_weight that are read from file*/
                new_node = create_node(vertex_no,edge_weight);
                add_node(adj_list[head_index],new_node);

                fgets(line_buffer,LINESIZE,fp);


        }

        fclose(fp);
        return adj_list;

}

/*Creates a node of type struct Node*/
struct Node* create_node(int vertex_no,int edge_weight) {

        struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));

        tmp->vertex_no = vertex_no;
        tmp->edge_weight = edge_weight;
        tmp->min_distance_val = 0;

        if (tmp == NULL) {
                printf("Memory is not allocated!\n");
                exit(-2);
        }
        tmp->next = NULL;
        tmp->parent_next = NULL;

        return tmp;
}


/*Adds given adjacent node to the given head node's adjacent node list*/
void add_node(struct Node* head,struct Node* add) {

        struct Node* iterator = head;

        while (iterator->next) {
                iterator = iterator->next;
        }

        iterator->next = add;

}

/*Adds given parent node to the given head node's parent node list*/
void add_parent_node(struct Node* head,struct Node* add) {

        struct Node* iterator = head;

        while (iterator->parent_next) {
                iterator = iterator->parent_next;
        }

        iterator->parent_next = add;
}


/*Finds all possible paths between given nodes and also stores the shortest path*/
int find_paths(struct Node** adj_list,int vertex_cnt,int base_node,int dest_node) {

        int i;
        int flag = 0;

        if (adj_list[base_node] == NULL) {
                printf("There is not any node labeled as %d or there is no exit from that node!\n",base_node );
                exit(-7);
        }


        flag = bfs_possible_paths(adj_list,base_node,dest_node,vertex_cnt);

        if (!flag) {
                printf("Given two nodes %d and %d are not connected directly or indirectly!\n",base_node,dest_node );
        }

        return flag;


}

/*BFS on graph that is implemented with adjaceny list*/
int bfs_possible_paths(struct Node** adj_list,int base_node,int dest_node,int vertex_cnt) {

        QUEUE* q;
        int* visited;
        int node_index;
        struct Node* iterator;
        struct Node* parent;
        struct Node* tmp;
        int flag = 0;


        q = create_queue();
        visited = (int*)calloc(vertex_cnt,sizeof(int));

        if (visited == NULL) {
                printf("Memory is not allocated!\n");
                exit(-8);
        }

        visited[base_node] = 1;
        enqueue(q,base_node);


        /*BFS traversal continues until queue is empty*/
        while (!is_empty(q)) {


                node_index = dequeue(q);

                if (node_index == dest_node) {
                        flag = 1;
                }


                iterator = adj_list[node_index];

                iterator = iterator->next;


                /*Adjacent nodes traversing loop*/
                while (iterator != NULL) {


                        /*If initial adjacent node is not visited*/
                        if (!visited[iterator->vertex_no]) {

                                visited[iterator->vertex_no] = 1;

                                /*Storing initial adjacent node in the queue*/
                                enqueue(q,iterator->vertex_no);

                                /*If initial adjacent node has no head pointer in the adj_list*/
                                if (adj_list[iterator->vertex_no] == NULL) {
                                        adj_list[iterator->vertex_no]=create_node(iterator->vertex_no,0);
                                }

                                /*Setting up a new parent node list for the initial adjacent node and adding initial parent node (node_index) as parent*/
                                parent = create_node(node_index,iterator->edge_weight);


                                /*Initializing current adjacent node's parent list*/
                                adj_list[iterator->vertex_no]->parent_next = parent;
                                tmp = adj_list[iterator->vertex_no];


                                /*Initializing minimum distance and minimum parent node index*/
                                tmp->min_distance_val = iterator->edge_weight + adj_list[node_index]->min_distance_val;
                                tmp->min_distance_parent = node_index;


                        }

                        /*If initial adjacent node is visited then there are more than one path to that node from different parent nodes*/
                        else {


                                tmp = create_node(node_index,iterator->edge_weight);

                                /*Adding node_index to initial adjacent node's parent node list*/
                                add_parent_node(adj_list[iterator->vertex_no],tmp);

                                /*If initial parent node's distance is shorter than initial adjacent node's minimum distance val then set minimum distance with parent node's distance + edge_weight*/
                                if (iterator->edge_weight + adj_list[node_index]->min_distance_val < adj_list[iterator->vertex_no]->min_distance_val) {
                                        adj_list[iterator->vertex_no]->min_distance_val = iterator->edge_weight + adj_list[node_index]->min_distance_val;
                                        adj_list[iterator->vertex_no]->min_distance_parent = node_index;

                                }

                        }

                        iterator = iterator->next;
                }

        }
        free(visited);

        return flag;

}

/*Prints shortest path recursively*/
void print_shortest_path(struct Node** adj_list,int base_node,int dest_node,int min_node,int prev_node) {

        if (min_node == base_node) {
                printf("%d ",min_node );
                printf(" (< %d units >)  ",get_distance(adj_list,min_node,prev_node) );   //get_distance(adj_list,min_node,prev_node)
        }

        else {

                print_shortest_path(adj_list,base_node,dest_node,adj_list[min_node]->min_distance_parent,min_node);
                printf("%d ",min_node );

                if (min_node != dest_node) {
                        printf(" (< %d units >)  ",get_distance(adj_list,min_node,prev_node) );
                }


                if (min_node == dest_node) {
                        printf("   *** Total cost: %d units ***\n",adj_list[dest_node]->min_distance_val );
                }
        }


}

/*Finds possible paths recursively by using parent list created in bfs_enhanced()*/
void print_possible_paths(struct Node** adj_list,struct Node* head,int dest_node,int base_node,int* path,int path_len) {



        struct Node* iterator = head;
        iterator = iterator->parent_next;
        int i;
        int total_cost = 0;



        if (head == NULL) {
                return;
        }


        path[path_len] = head->vertex_no;
        path_len++;


        /*If the initial node is the base node then print path*/
        if (head->vertex_no == base_node) {

                for (i = path_len-1; i >= 0; i--) {

                        printf("%d ",path[i] );
                        if (i>=1)
                                total_cost += get_distance(adj_list,path[i],path[i-1]);


                }

                printf("(%d units)\n",total_cost);
        }


        else {

                /*Traversing all parent nodes (subtrees) in parent list. For each parent node recurs to find next path.*/
                while (iterator) {

                        print_possible_paths(adj_list,adj_list[iterator->vertex_no],dest_node,base_node,path,path_len);
                        iterator = iterator->parent_next;
                }

        }

}


/*This function returns distance between given two nodes of the graph. If two nodes are directly connected (two-way check) returns distance
otherwise returns zero*/
int get_distance(struct Node** adj_list,int first_node,int second_node) {

        struct Node* iterator;

        /*Checking if there is an exit from first node to second_node*/
        iterator = adj_list[first_node];
        iterator = iterator->next;

        while (iterator) {
                if (iterator->vertex_no == second_node)
                        return iterator->edge_weight;

                iterator = iterator->next;
        }


        /*Checking if there is an exit from second_node to first_node*/
        iterator = adj_list[second_node];
        iterator = iterator->next;

        while (iterator) {
                if (iterator->vertex_no == first_node)
                        return iterator->edge_weight;

                iterator = iterator->next;
        }

        return 0;

}

/*Finds directly connected nodes and also prints the node that has the highest number of neighbors*/
void find_directly_connected_nodes(struct Node** adj_list,int vertex_cnt) {


        struct Node* iterator;
        int flag;
        int i,k;
        int max_nb_cnt; //Holds maximum neighbor count
        int nb_cnt; //Holds initial neighbor count
        int max_nb_node; //Holds node that has the highest number of neighbors

        printf("\n");

        max_nb_cnt = 0;

        for (i = 0; i < vertex_cnt; i++) {

                flag = 0;
                nb_cnt = 0;

                printf("Directly connected nodes to %d node: ",i);

                /*If there are at least one exiting from initial i node or at least one entering to initial i node*/
                if (adj_list[i]) {

                        iterator = adj_list[i]->next;

                        /*Traversing through adjacent (entering) nodes*/
                        while (iterator) {
                                printf("%d ",iterator->vertex_no );
                                iterator=iterator->next;
                                flag = 1;
                                nb_cnt++;
                        }

                }


                k=0;

                /*Checking every node's adjacent nodes list in the graph to find initial i node (entering nodes)*/
                while (k<vertex_cnt && adj_list[k]) {
                        iterator = adj_list[k]->next;
                        while (iterator) {
                                if (iterator->vertex_no == i){
                                        printf("%d ",adj_list[k]->vertex_no );
                                        flag = 1;
                                        nb_cnt++;
                                }


                                iterator=iterator->next;
                        }
                        k++;
                }

                if(!flag)
                        printf("None");

                printf("\n");

                if (nb_cnt > max_nb_cnt) {
                        max_nb_cnt = nb_cnt;
                        max_nb_node = i;

                }

        }

        printf("\nNode that has the highest number of neighbors: %d\n",max_nb_node );
        printf("Node %d has %d neighbors\n",max_nb_node,max_nb_cnt );
}

/*Finds the most distant nodes each other two nodes in the graph*/
void find_distant_nodes(struct Node** adj_list,int vertex_cnt){

        int i;
        int global_max_val = 0;
        int global_max_base;
        int global_max_dest;
        int init_max;
        int* distance_values;



        for (i = 0; i < vertex_cnt; i++) {

                if ( (adj_list[i] != NULL) && (adj_list[i]->next != NULL)) {

                        distance_values = (int*)calloc(vertex_cnt,sizeof(int));

                        bfs_longest_path(adj_list,i,vertex_cnt,distance_values);
                        init_max = find_max_node(distance_values,vertex_cnt);

                        if (distance_values[init_max] > global_max_val) {
                                global_max_val = distance_values[init_max];
                                global_max_base = i;
                                global_max_dest = init_max;
                        }
                        free(distance_values);

                }

        }

        printf("\nThe most distant two nodes: %d %d \nDistance between %d and %d is: %d\n",global_max_base,global_max_dest,global_max_base,global_max_dest,global_max_val );

}

/*Standard BFS function which additionally stores the maximum distance parents.*/
void bfs_longest_path(struct Node** adj_list,int base_node,int vertex_cnt,int* distance_values) {

        struct Node* iterator;
        int* visited;
        QUEUE* q;
        int node_index;
        int i;

        q = create_queue();

        visited = (int*)calloc(vertex_cnt,sizeof(int));

        if (visited == NULL) {
                printf("Memory is not allocated!\n");
                exit(-9);
        }

        enqueue(q,base_node);
        visited[base_node] = 1;

        while (!is_empty(q)) {

                node_index = dequeue(q);
                iterator = adj_list[node_index];

                if (iterator)
                        iterator = iterator->next;


                /* Traversing adjacent nodes*/
                while (iterator) {

                        /*if not visited*/
                        if (!visited[iterator->vertex_no]) {
                                distance_values[iterator->vertex_no] = distance_values[node_index] + iterator->edge_weight;
                                visited[iterator->vertex_no] = 1;
                                enqueue(q,iterator->vertex_no);
                        }

                        /*If visited*/
                        else {
                                if (iterator->edge_weight + distance_values[node_index] > distance_values[iterator->vertex_no])
                                        distance_values[iterator->vertex_no] = iterator->edge_weight + distance_values[node_index];

                        }

                        iterator=iterator->next;
                }

        }


        free(q);
        free(visited);

}

/*Returns the node index which has the highest distance value*/
int find_max_node(int* distance_values,int vertex_cnt) {

        int i;
        int max;
        int max_node;

        max = distance_values[0];
        for (i = 0; i < vertex_cnt; i++) {

                if (distance_values[i] > max) {
                        max = distance_values[i];
                        max_node = i;
                }
        }
        return max_node;

}



int main(int argc, char *argv[]) {


        int base_node; //Path starting node index
        int dest_node; //Path finish node index
        int vertex_cnt; //Total vertex count
        int edge_cnt; //Total edge count
        int i;
        struct Node** adj_list; //The adjacency list of graph
        struct Node* iterator; //Used for generating possible paths recursively after BFS is done
        int* path; //Used for holding paths while generating possible path after BFS is done


        adj_list = read_file(&base_node,&dest_node,&vertex_cnt,&edge_cnt);



        /*Utility code to print adj list*/

        /*
        printf("The graph as adjacency list:\n");
        for (i = 0; i < vertex_cnt; i++) {
                iterator = adj_list[i];

                if (iterator != NULL) {
                        iterator = iterator->next;
                        printf("Node %d adjacents: \n",i );
                        while (iterator) {
                                printf("Vertex no: %d Edge weight: %d\n",iterator->vertex_no,iterator->edge_weight );
                                iterator = iterator->next;
                        }
                }


        }
        */

        /**/
        i = find_paths(adj_list,vertex_cnt,base_node,dest_node);

        if (i) {

                printf("\nThe shortest path: ");
                print_shortest_path(adj_list,base_node,dest_node,dest_node,dest_node);
                printf("\n");
                iterator = adj_list[dest_node];
                path = (int*)malloc(sizeof(int)*vertex_cnt);
                printf("Possible paths between nodes %d and %d:\n",base_node,dest_node);
                print_possible_paths(adj_list,iterator,dest_node,base_node,path,0);


        }

        find_directly_connected_nodes(adj_list,vertex_cnt);
        find_distant_nodes(adj_list,vertex_cnt);




        return 0;
}
