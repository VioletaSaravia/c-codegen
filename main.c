#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CodegenRun(Codegen* this) {
    FILE *file = fopen(this.fileName, "r+");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *buffer = NULL;
    size_t length = 0;
    size_t read_size;

    // Read the entire file into a buffer
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char *)malloc(length + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';

    fclose(file);

    // Perform the replacement in the buffer
    char *start_pos = strstr(buffer, "/* GEN START {*this.codegenName} */");
    char *end_pos = strstr(buffer, "/* GEN END {*this.codegenName} */");

    if (start_pos != NULL && end_pos != NULL && start_pos < end_pos) {
        // TODO Inefficient
        char *new_buffer = (char *)malloc(length + strlen(this.newCode) + 1);
        if (new_buffer == NULL) {
            perror("Error allocating memory");
            free(buffer);
            exit(EXIT_FAILURE);
        }

        strncpy(new_buffer, buffer, start_pos - buffer + strlen("/* START */"));
        new_buffer[start_pos - buffer + strlen("/* START */")] = '\0';
        strcat(new_buffer, this.newCode);
        strcat(new_buffer, end_pos);

        // Write the new content back to the file
        file = fopen(this.fileName, "w");
        if (file == NULL) {
            perror("Error opening file");
            free(buffer);
            free(new_buffer);
            exit(EXIT_FAILURE);
        }

        fwrite(new_buffer, 1, strlen(new_buffer), file);

        free(new_buffer);
    }

    free(buffer);
    fclose(file);

    printf("Text replacement completed successfully.\n");
}

struct Codegen {
    const char* fileName;
    const char* codegenName;
    const char* newCode;
    const char*()* generate; // function?
}

int main() {
    Codegen test = {
        fileName = "test/main.c";
        codegenName = "INTEGERS_TEST";
        newCode = " int y = 420\n";
    }
    
    CodegenRun(&test);

    return EXIT_SUCCESS;
}

