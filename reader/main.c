#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "../shm_protocol.h"

volatile bool quit = false;
int sh_fd;
const struct shm_data *src;

int shm_init() {
    sh_fd = shm_open(SHMEM_NAME, O_RDONLY, 0600);
    if (sh_fd < 0) {
	return 1;
    }
    ftruncate(sh_fd, sizeof(struct shm_data));
    src = (const struct shm_data *)
	mmap(NULL, sizeof(struct shm_data), PROT_READ, MAP_SHARED, sh_fd, 0);
    return 0;
}


void cleanup() {
    munmap((void*)src, sizeof(struct shm_data));
    close(sh_fd);
    //shm_unlink(SHMEM_NAME);
}
void* read_loop(void* p) {
    (void)p;
    uint8_t a = 0;
    uint8_t b = 0;
    double x = 0.0;
    double y = 0.0;
    while (!quit) {
	    a = src->a;
	    b = src->b;
	    x = src->x;
	    y = src->y;
    	printf("%d, %d, %f, %f\n", a, b, x, y);
    	if (y != 2 * x || b != 2 * a) {
    	    printf("!!!!!!!!!!!!!!!!!!!!!!!!\n");
    	}
	    usleep(300 * 1000);
    }
    return NULL;
}

int main() {

    shm_init();

    pthread_t tid;
    pthread_attr_t tatt;
    pthread_attr_init(&tatt);
    pthread_create(&tid, &tatt, &read_loop, NULL);

    getchar();
    quit = true;

    cleanup();
    return 0;
}

