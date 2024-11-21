/*
 * By elPytel
 */

int* Int(int value) {
    int* data = (int*)malloc(sizeof(int));
    *data = value;
    return data;
}

float* Float(float value) {
    float* data = (float*)malloc(sizeof(float));
    *data = value;
    return data;
}

char* Char(char value) {
    char* data = (char*)malloc(sizeof(char));
    *data = value;
    return data;
}

int* range(int start, int end, int step) {
    int* data = (int*)malloc((end - start) * sizeof(int));
    for (int i = start; i < end; i += step) {
        data[i] = i;
    }
    return data;
}

float* range_float(int start, int end, float step) {
    float* data = (float*)malloc((end - start) * sizeof(float));
    for (int i = start; i < end; i += step) {
        data[i] = i;
    }
    return data;
}