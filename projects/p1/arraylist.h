typedef struct{
    unsigned length;
    unsigned capacity;
    int *data;
} array_t;

void al_init(array_t *, unsigned);
void al_destroy(array_t *);

void al_append(array_t, int);