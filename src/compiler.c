#include <stdio.h>

#include "common.h"
#include "compiler.h"

void cpl_compile(const char* source)
{
    scn_init_scanner(source);

    int line = -1;
    while (true)
    {
        Token token = scn_scan_token();
        if (token.line != line)
        {
            printf("%4d ", token.line);
            line = token.line;
        }
        else
        {
            printf("   | ");
        }

        printf("%-16s '%.*s'\n", g_token_type_to_str[token.type], token.length,
               token.start);

        if (token.type == TOKEN_EOF)
            break;
    }
}
