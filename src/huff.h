#ifndef __HUFF_H_
#define __HUFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define IS_ROOT             (1u << 0)  // 0 0 0 0 0 0 0 1
#define HAVE_RIGHT_CHILD    (1u << 1)  // 0 0 0 0 0 0 1 0
#define HAVE_LEFT_CHILD     (1u << 2)  // 0 0 0 0 0 1 0 0
#define IS_LEAF             (1u << 3)  // 0 0 0 0 1 0 0 0

typedef struct HTnode_ { // Huffman tree node
    uint8_t symbol;       // character of the leaf node (ascII)
    uint64_t weight;       // frequency of that characrer in input file
    struct HTnode_ *right; // right child (if it's not null)
    struct HTnode_ *left;  // left child (if it's not null)
    uint8_t flags;         // flags each node can have
} HTnode;

// typedef struct byte_info_ {
//     int64_t *buffer; 
//     int64_t total_bytes; 
//     int64_t unique_bytes;
// } byte_info;

int64_t* determine_frequency(FILE* fdIN);
HTnode* sort_node_by_weight(HTnode* unsorted_nodes, int64_t unique);
HTnode* init_huff_tree(int64_t* buffer,int64_t unique);


#endif //__HUFF_H_
