#include <stdio.h>

#include "debug.h"
#include "error.h"
#include "vm.h"

static void repl();
static void run_file(const char* path);
static char* read_file(const char* path);

int main(int argc, const char* argv[])
{
    vm_init_vm();

    if (argc == 1)
    {
        repl();
    }
    else if (argc == 2)
    {
        run_file(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    vm_free_vm();

    return 0;
}

static void repl()
{
    char line[1024];
    while (true)
    {
        printf(">");

        if (!fgets(line, sizeof(line), stdin))
        {
            printf("\n");
            break;
        }

        vm_interpret(line);
    }
}

static void run_file(const char* path)
{
    char* source = read_file(path);
    InterpreterResult result = vm_interpret(source);
    free(source);

    if (result == INTPR_COMPILE_ERROR)
        exit(65);
    if (result == INTPR_RUNTIME_ERROR)
        exit(70);
}

static char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL)
    {
        err_runtime_error(1, "Error: failed to open file \"%s\"\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL)
    {
        err_runtime_error(1, "Error: not enough memory to read \"%s\"\n", path);
        exit(74);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size)
    {
        err_runtime_error(1, "Error: failed to read file \"%s\"\n", path);
        exit(74);
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}
