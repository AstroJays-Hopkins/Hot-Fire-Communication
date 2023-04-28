typedef struct {
    char header = 0x83;
    int8_t type;
    unsigned long counter;
} __attribute__((packed)) Response;