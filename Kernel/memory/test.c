// // // #include "buddy.h"
// // // #include <stdio.h>

// // // void printStates(){
// // //     for(int i=0; i<CANT_NODES; i++){
// // //         if(i == 15){
// // //             printf("*");
// // //         }

// // //         if(i== 7 || i == 23){
// // //             printf("^");
// // //         }

// // //         if(i==3 || i == 11 || i==19 || i==28){
// // //             printf("'");
// // //         }
// // //         printf("%d ", mem_states[i]);
// // //     }
// // //     printf("\n");
// // //      printf("usado: %d\n", mm_occupied_space());
// // //     printf("libre: %d\n", mm_unused_space());
// // //     printf("total: %d\n", mm_total_space());
// // //     printf("\n");   
// // // }

// // // int main(){
// // //     mm_init(FREE_MEM_START, MEM_SIZE);
// // //     printStates();

// // //     printf("Pido 18\n");
// // //     mm_malloc(18);
// // //     printStates();
// // // }

// #include "buddy.h"
// #include <stdio.h>
// #include <stdlib.h>


// int main() {
//     // Inicializa la memoria
//     mm_init((uint64_t)malloc(MEM_SIZE), MEM_SIZE);

//     printf("Espacio total: %d bytes\n", MEM_SIZE);

//     printf("\n"); 


// // 64k = 64 * 1024 = 65536
// // 48k = 48 * 1024 = 49152
// // 32k = 32 * 1024 = 32768
// // 24k = 24 * 1024 = 24576
// // 16k = 16 * 1024 = 16384
// // 8k = 8 * 1024 = 8192
// // 4k = 4 * 1024 = 4096
// // 2k = 2 * 1024 = 2048


//     // // Solicita bloques de memoria
//     // 10k = 10 * 1024 = 10240
//     // printf("\npido 10k ---------------------------------------------------------------------------------\n");
//     // printf("%p\n",mm_malloc(10240));
//     // printf("\npido 14k ---------------------------------------------------------------------------------\n");
//     // //14k = 14 * 1024 = 14336
//     // // void * ptr2 = mm_malloc(14336);
//     // printf("%p\n",mm_malloc(14336));
//     // printf("\npido 16k ---------------------------------------------------------------------------------\n");
//     // //16k = 16 * 1024 = 16384   
//     // // void * ptr3 = mm_malloc(16384);
//     // printf("%p\n",mm_malloc(16384));
//     // printf("\npido 5k ---------------------------------------------------------------------------------\n");
//     // //5k = 5 * 1024 = 5120
//     // // void * ptr4 = mm_malloc(5120);
//     // printf("%p\n",mm_malloc(5120));


//     void *ptr1 = mm_malloc(10240);
//     printf("\n");
//     void *ptr2 = mm_malloc(14336); 
//     printf("\n");
//     void *ptr3 = mm_malloc(5120); 
//     printf("\n");

//     // Imprimir estado de memoria
//     printf("Espacio ocupado: %d bytes\n", mm_occupied_space());
//     printf("Espacio no utilizado: %d bytes\n", mm_unused_space());

//     // Libera bloques de memoria
//     mm_free(ptr1);
//     printf("\n");
//     mm_free(ptr2);
//     printf("\n");
//     mm_free(ptr3);
//     printf("\n");   

//     // Imprimir estado de memoria después de liberar
//     printf("Espacio ocupado después de liberar: %d bytes\n", mm_occupied_space());
//     printf("Espacio no utilizado después de liberar: %d bytes\n", mm_unused_space());

//     // // Liberar memoria inicial
//     free(mem);

//     return 0;
// }
