#include <stdio.h>

#define VIRTUAL_MEMORY_SIZE 16
#define STORAGE_SIZE 16

typedef struct {
    int page_number;
    int last_access_time;
} PageTableEntry;

int main(int argc, char *argv[]) {
    if (argc != 2 || (argv[1][0] != '-' && (argv[1][1] != 'f' || argv[1][1] != 'l'))) {
        printf("Usage: %s -f (FIFO) o -l (LRU)\n", argv[0]);
        return 1;
    }

    char algorithm = argv[1][1];
    PageTableEntry page_table[VIRTUAL_MEMORY_SIZE];
    int storage[STORAGE_SIZE] = {0};
    int storage_index = 0;
    int fifo_index = 0;

    int i;
    for (i = 0; i < VIRTUAL_MEMORY_SIZE; i++) {
        page_table[i].page_number = -1;
        page_table[i].last_access_time = 0;
    }

    int process_id, page_number;
    while (scanf("%d", &process_id) != EOF && scanf("%d", &page_number) != EOF) {
        int page_index = page_number;

        if (page_table[page_index].page_number == -1) { 

            if (storage_index < STORAGE_SIZE) {
                page_table[page_index].page_number = storage_index;
                storage[storage_index++] = page_index;
            } else { 
                int replace_page;
                if (algorithm == 'f') { // FIFO
                    replace_page = storage[fifo_index++];
                    fifo_index %= (STORAGE_SIZE);
                } else { // LRU
                    int min_access_time = page_table[0].last_access_time;
                    replace_page = 0;
                    int i;
                    for (i = 1; i < VIRTUAL_MEMORY_SIZE; i++) {
                        if (page_table[i].last_access_time < min_access_time) {
                            min_access_time = page_table[i].last_access_time;
                            replace_page = i;
                        }
                    }
                }

                page_table[replace_page].page_number = storage_index;
                storage[storage_index++] = page_index;
            }
        }

        page_table[page_index].last_access_time++;
        
        printf("Process %d: Page %d\n", process_id, page_number);
    }
    
    return 0;
}
