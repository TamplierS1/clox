#ifndef ERROR_H
#define ERROR_H

#include "scanner.h"

/* === Reporting errors to the user === */

void err_error(const char* format, ...);
void err_error_at_token(Token* token, const char* msg);

/* === Reporting errors to the user === */

#endif  // ERROR_H
