#include <string.h>

#include "scanner.h"

Scanner g_scanner;

void scn_init_scanner(const char* source)
{
    g_scanner.start = source;
    g_scanner.current = source;
    g_scanner.line = 1;
}

Token scn_scan_token()
{
    skip_whitespace();

    g_scanner.start = g_scanner.current;

    if (is_end())
        return make_token(TOKEN_EOF);

    char c = advance();
    if (is_digit(c))
        return number();
    if (is_alpha(c))
        return identifier();

    switch (c)
    {
        case '(':
            return make_token(TOKEN_LEFT_PAREN);
        case ')':
            return make_token(TOKEN_RIGHT_PAREN);
        case '{':
            return make_token(TOKEN_LEFT_BRACE);
        case '}':
            return make_token(TOKEN_RIGHT_BRACE);
        case ';':
            return make_token(TOKEN_SEMICOLON);
        case ',':
            return make_token(TOKEN_COMMA);
        case '.':
            return make_token(TOKEN_DOT);
        case '-':
            return make_token(TOKEN_MINUS);
        case '+':
            return make_token(TOKEN_PLUS);
        case '/':
            if (peek_next() == '/')
            {
                // Skip the comment
                while (peek() != '\n' && !is_end())
                    advance();
            }
            else
            {
                return make_token(TOKEN_SLASH);
            }
            break;
        case '*':
            return make_token(TOKEN_STAR);
        case '!':
            return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"':
            return string();
    }

    return error_token("Unexpected character.");
}

static Token string()
{
    while (!is_end() && peek() != '"')
    {
        if (peek() == '\n')
            g_scanner.line++;
        advance();
    }

    if (is_end())
        return error_token("Unterminated string.");

    // Consume '"'
    advance();

    return make_token(TOKEN_STRING);
}

static Token number()
{
    while (is_digit(peek()))
        advance();

    // Handle fractions.
    if (peek() == '.' && is_digit(peek_next()))
    {
        // Consume '.'
        advance();

        while (is_digit(peek()))
            advance();
    }

    return make_token(TOKEN_NUMBER);
}

static Token identifier()
{
    while (is_alnum(peek()))
    {
        advance();
    }

    return make_token(identifier_type());
}

static TokenType identifier_type()
{
    // A simple trie that matches keywords
    switch (g_scanner.start[0])
    {
        case 'a':
            return check_keyword(1, 2, "nd", TOKEN_AND);
        case 'c':
            return check_keyword(1, 4, "lass", TOKEN_CLASS);
        case 'e':
            return check_keyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (g_scanner.current - g_scanner.start > 1)
            {
                switch (g_scanner.start[1])
                {
                    case 'a':
                        return check_keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return check_keyword(2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return check_keyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        case 'i':
            return check_keyword(1, 1, "f", TOKEN_IF);
        case 'n':
            return check_keyword(1, 2, "il", TOKEN_NIL);
        case 'o':
            return check_keyword(1, 1, "r", TOKEN_OR);
        case 'p':
            return check_keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r':
            return check_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 's':
            return check_keyword(1, 4, "uper", TOKEN_SUPER);
        case 't':
            if (g_scanner.current - g_scanner.start > 1)
            {
                switch (g_scanner.start[1])
                {
                    case 'h':
                        return check_keyword(2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return check_keyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        case 'v':
            return check_keyword(1, 2, "ar", TOKEN_VAR);
        case 'w':
            return check_keyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static TokenType check_keyword(int start, int length, const char* rest, TokenType type)
{
    if (g_scanner.current - g_scanner.start == start + length &&
        memcmp(g_scanner.start + start, rest, length) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static bool is_alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

static bool is_alnum(char c)
{
    return is_digit(c) || is_alpha(c);
}

static void skip_whitespace()
{
    while (true)
    {
        char c = peek();
        switch (c)
        {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                g_scanner.line++;
                advance();
                break;
            default:
                return;
        }
    }
}

static bool is_end()
{
    return *g_scanner.current == '\0';
}

static Token make_token(TokenType type)
{
    Token token = {.type = type,
                   .start = g_scanner.start,
                   .length = (int)(g_scanner.current - g_scanner.start),
                   .line = g_scanner.line};
    return token;
}

static Token error_token(const char* msg)
{
    Token token = {
        .type = TOKEN_ERROR,
        .start = msg,
        .length = (int)strlen(msg),
        .line = g_scanner.line,
    };
    return token;
}

static char advance()
{
    g_scanner.current++;
    return g_scanner.current[-1];
}

static bool match(char c)
{
    if (peek() != c || is_end())
        return false;

    g_scanner.current++;

    return true;
}

static char peek()
{
    return *g_scanner.current;
}

static char peek_next()
{
    if (is_end())
        return '\0';
    return g_scanner.current[1];
}
