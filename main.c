#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 255

struct Node {

        int vertex_no;
        int edge_weight;
        struct Node* next;

};


/*prototypes*/
struct Node** read_file(int*,int*,int*,int*);
struct Node* create_node(int,int);
void add_node(struct Node*,struct Node*);


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
                printf("Alan ayrilamadi!\n");
                exit(-3);
        }


        /*Reading base node and destination node*/
        fgets(line_buffer,LINESIZE,fp);
        p=strtok(line_buffer," ");
        *base_node = atoi(p);
        p=strtok(NULL," ");
        *dest_node= atoi(p);

        /*Reading adjacent nodes to head nodes*/
        fgets(line_buffer,LINESIZE,fp);
        while (!feof(fp)) {

                p=strtok(line_buffer," ");
                head_index = atoi(p);

                if(adj_list[head_index] == NULL)
                        adj_list[head_index] = create_node(0,0);


                p=strtok(NULL," ");
                vertex_no = atoi(p);

                p=strtok(NULL," ");
                edge_weight = atoi(p);

                /*Creating a new adjacent node with vertex no and edge_weight that are read from file*/
                new_node = create_node(vertex_no,edge_weight);
                add_node(adj_list[head_index],new_node);

                fgets(line_buffer,LINESIZE,fp);


        }

        return adj_list;

}

/*Creates a node of type struct Node*/
struct Node* create_node(int vertex_no,int edge_weight) {

        struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));

        tmp->vertex_no = vertex_no;
        tmp->edge_weight = edge_weight;

        if (tmp == NULL) {
                printf("Alan ayrilamadi!\n");
                exit(-2);
        }
        tmp->next = NULL;

        return tmp;
}


/*Adds a given node to the given head node's list*/
void add_node(struct Node* head,struct Node* add) {

        struct Node* iterator = head;

        while (iterator->next) {
                iterator = iterator->next;
        }

        iterator->next = add;

}





int main(int argc, char *argv[]) {

        int base_node;
        int dest_node;
        int vertex_cnt;
        int edge_cnt;
        int i;
        struct Node** adj_list;

        adj_list = read_file(&base_node,&dest_node,&vertex_cnt,&edge_cnt);


        /*Utility code to print adj list*/
        struct Node* iterator;
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





        return 0;
}
