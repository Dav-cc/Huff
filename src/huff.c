#include "huff.h"
#include <iso646.h>
#include <stdint.h>

HTnode* sort_node_by_weight(HTnode* unsort_nodes, int64_t unique) {
    HTnode* buff = unsort_nodes;
    HTnode tmp;

    // bubble sort :)))))
    for (int64_t i = 0; i < unique - 1; i++) {
        for (int64_t j = 0; j < unique - 1 - i; j++) {
            if (buff[j].weight > buff[j + 1].weight) {
                tmp = buff[j];
                buff[j] = buff[j + 1];
                buff[j + 1] = tmp;
            }
        }
    }
    return buff;
}

int64_t* determine_frequency(FILE* fdIN) {
    int64_t total_bytes = 0;
    int64_t unique_bytes = 0;
    int64_t *buff, *buff_ptr;
    char c;

    fseek(fdIN, 0, SEEK_END);

    int64_t file_size = ftell(fdIN);

    // FIX : setup a logging system and error handling here

    fseek(fdIN, 0, SEEK_SET);
    int64_t* buffer = (int64_t*)calloc(256, sizeof(int64_t)); // for null termination
    buff_ptr = buffer;

    // uint64_t bytes_readed = fread(buff, 1, file_size, fdIN);
    // if(bytes_readed == 0){
    //     // FX : setup a logging system
    // }

    for (int64_t i = 0; i < file_size; i++) {
        c = fgetc(fdIN);
        buff[c]++;
        total_bytes++;
    }
    for (int64_t i = 0; i < 256; i++) {
        if (buff[i])
            unique_bytes++;
    }

    // now we know the frequency of each element and number of all bytes and unique bytes

    init_huff_tree(buffer, unique_bytes);

    return buff;
}

HTnode* init_huff_tree(int64_t* buffer, int64_t unique) {

    int64_t num_nodes = (2 * unique) - 1;

    HTnode* nodes = (HTnode*)calloc(num_nodes, sizeof(HTnode));
    if (!nodes)
        return NULL;

    HTnode* node_ptr = nodes;

    for (int64_t i = 0; i < 256; i++) {
        if (buffer[i]) {
            node_ptr->weight = *(buffer + i);
            node_ptr->symbol = i;
            node_ptr->right = NULL;
            node_ptr->left = NULL;
            node_ptr->flags = 8u; //   0 0 0 0 1 0 0 0   it maeans this node only are leaf
            node_ptr++;
        }
    }

    HTnode* leaf_nodes = (HTnode*)calloc((2 * unique) - 1, sizeof(HTnode));
    if (!leaf_nodes)
        return NULL;

    leaf_nodes = sort_node_by_weight(nodes, unique);
    if (leaf_nodes) {
        free(nodes);
        free(buffer);
    }
    // return nodes;
    parse_huffman_tree(leaf_nodes, unique);
    // return leaf_nodes;
}

/*
//
//
//
//    |-------------------------------------------------------------------------------------------|
//    |least_weight1 -|- least_weight2 -|- is_leaf -|- . . . . .  -|- not_leaf \ current -| . . . |
//    |-------------------------------------------------------------------------------------------|
//                                                                  ^
//                                                                  |
//                                                                  end of unique elements here(rest
of this is for making NOT_LEAF nodes)
//
//
//
//
//
//
//
*/

HTnode* parse_huffman_tree(HTnode* leafs, int64_t unique_number) {
    HTnode *least_weight1 = leafs, *least_weight2 = leafs + 1, *current = leafs + unique_number,
           *not_leaf = leafs + unique_number, *is_leaf = leafs + 2;
    HTnode* i = leafs;
    for (int64_t i = 0; i < unique_number - 1; i++) {
        current->weight = least_weight1->weight + least_weight2->weight;
        current->left = least_weight1;
        current->right = least_weight2;
        current->flags = 13u; // this node is not leaf  and have leaft and right child so . . .( 0 0
                              // 0 0 1 1 1 0 )
        least_weight1->flags = 16u; // bit 0 (0 0 0 1 0 0 0 0)
        least_weight2->flags = 32u; // bit 1 (0 0 1 0 0 0 0 0)
        current++;


        // finding least weight node 
        if (is_leaf > leafs + unique_number) {
            least_weight1 = not_leaf;
            not_leaf++;
        } else {
            if (not_leaf->weight < is_leaf->weight) {
                least_weight1 = not_leaf;
                not_leaf++;
            } else {
                least_weight1 = is_leaf;
                is_leaf;
            }
        }

        // and for the seccond least wegith node repeate this for node 2
        if (is_leaf > leafs + unique_number) {
            least_weight2 = not_leaf;
            not_leaf++;
        } else {
            if (not_leaf->weight < is_leaf->weight) {
                least_weight2 = not_leaf;
                not_leaf++;
            } else {
                least_weight2 = is_leaf;
                is_leaf++;
            }
        }
    }
    for(i = ((leafs + (2 *unique_number) - 1) -1 ) ; i <leafs -1; i-- ){
        if(i->right){
            i->right->flags |= i->flags;
        }
        if(i->left){
            i->left->flags |= i->flags;
        }
    }
}


void write_file(HTnode* nodes, const char* filename){
    FILE* out_fd = fopen(filename, "w");

}
