#ifndef ERROR_H
#define ERROR_H

// Each variant describes any additional information that is
// needed to be passed to `err_error()`
typedef enum {
    ERROR_UNKNOWN_UPCODE, // Pass the unknown opcode
    ERROR_FAILED_MEM_ALLOC,
} ErrorType;

/* === Reporting errors to the user === */

// Displays an error using the provided variadic arguments.
void err_error(ErrorType error, ...);

/* === Reporting errors to the user === */

#endif // ERROR_H
