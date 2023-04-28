typedef struct {
    char header = 0x83; //1
    int8_t type; //1
    unsigned long counter; //4
    double tc1; //4
    double tc2; //4
    int pt1; //2
    int pt2; //2
} __attribute__((packed)) Response;
