#include "ecc.h"

char *concat(const char str1[], size_t size1, const char str2[], size_t size2, size_t *res_size){
    unsigned size = size1 + size2;
    char *result = malloc(sizeof(char) * size);
    for(unsigned i=0;i<size1;i++){
        result[i] = str1[i];
    }
    for(unsigned i=0;i<size2;i++){
        result[i + size1] = str2[i];
    }
    *res_size = size;
    return result;
}


void random_by_password_init(random_by_password * res, const char *password, size_t size) {
    char *x = code(password, size, 1, &res->size);
    char *y = concat(password, size, x, res->size, &res->size);
    res->master_key = code(y, res->size, 1, &res->size);
    res->counter = 0;
    free(x);
    free(y);
}

void random_by_password_clear(random_by_password *random){
    free(random->master_key);
    free(random);
}

int from_hex(char x) {
    if (x >= '0' && x <= '9') return x - '0';
    if (x >= 'a' && x <= 'f') return x - 'a' + 10;
    return 0;
}

void random_rand(BIG_SIZE n, bi_pt result, random_by_password *random) {
    random->counter++;
    char *y;
    size_t y_size = 0;
    int size = 0;
    bi_set_ui(result, 0);

    char buf[256];
    char *result_Y;

    while (size < n) {
        if (y_size == 0) {
            sprintf(buf, "%d", random->counter);

            unsigned Y_size = random->size + strlen(buf);
            result_Y = malloc(sizeof(char) * Y_size);
            for(unsigned i=0;i<random->size;i++){
                result_Y[i] = random->master_key[i];
            }
            for(unsigned i=0;i<strlen(buf);i++){
                result_Y[i + random->size] = buf[i];
            }
            y = code(result_Y, Y_size, 1, &y_size);
            free(result_Y);
        } else {
            sprintf(buf, "%d", random->counter);

            unsigned Y_size = y_size + random->size + strlen(buf);
            result_Y = malloc(sizeof(char) * Y_size);
            for(unsigned i=0;i<random->size;i++){
                result_Y[i] = y[i];
            }
            for(unsigned i=0;i<random->size;i++){
                result_Y[i + y_size] = random->master_key[i];
            }
            for(unsigned i=0;i<strlen(buf);i++){
                result_Y[i + random->size + y_size] = buf[i];
            }
            free(y);
            y = code(result_Y, Y_size, 1, &y_size);
            free(result_Y);
        }
        for (int j = 0; j < strlen(y) && size < n; j += 2, size++) {
            unsigned k = from_hex(y[j]) * 16 + from_hex(y[j + 1]);
            bi_left_shift(result, result, 8);
            bi_add_ui(result, result, k);
        }
    }
}
