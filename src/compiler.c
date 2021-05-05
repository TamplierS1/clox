#include "error.h"
#include "common.h"
#include "compiler.h"
#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

Parser g_parser;
Chunk* compiling_chunk;

/* Back-end */
static void emit_byte(uint8_t byte, int line);
static void emit_bytes(uint8_t byte1, uint8_t byte2, int line1, int line2);
static void emit_return();
static void emit_constant(Value value);
// Anything that needs to be done after the compiler finishes
// is placed here.
static void end_compiler();
static Chunk* current_chunk();

/* Front-end */
static void advance();
static void error_at(Token* token, const char* msg);
static void consume(TokenType type, const char* msg);

/* Parsing */
static void expression();
static void number();
static void grouping();
static void unary();
static void binary();
// Parse any expression at the given precedence level.
static void parse_precedence(Precedence precedence);
static ParseRule* get_rule(TokenType type);

// clang-format off

// This table contains token types that map to function pointers,
// which are used to parse the corresponding tokens.
// I also store precedence information here to parse binary operators.
ParseRule g_rules[] = {
    [TOKEN_LEFT_PAREN]      = {grouping, NULL, PREC_NONE},
    [TOKEN_RIGHT_PAREN]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};
// clang-format on

bool cpl_compile(const char* source, Chunk* chunk)
{
    scn_init_scanner(source);
    compiling_chunk = chunk;

    g_parser.had_error = false;
    g_parser.panic_mode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    end_compiler();

    return !g_parser.had_error;
}

static void emit_byte(uint8_t byte, int line)
{
    chk_write_chunk(current_chunk(), byte, line);
}

static void emit_bytes(uint8_t byte1, uint8_t byte2, int line1, int line2)
{
    emit_byte(byte1, line1);
    emit_byte(byte2, line2);
}

static void end_compiler()
{
#ifdef DEBUG_PRINT_CODE
    if (!g_parser.had_error)
    {
        dbg_disassemble_chunk(current_chunk(), "code");
    }
#endif
    emit_return();
}

static void emit_return()
{
    emit_byte(OP_RETURN, g_parser.previous.line);
}

static void emit_constant(Value value)
{
    chk_write_constant(current_chunk(), value, g_parser.previous.line);
}

static Chunk* current_chunk()
{
    return compiling_chunk;
}

static void advance()
{
    g_parser.previous = g_parser.current;

    while (true)
    {
        g_parser.current = scn_scan_token();
        if (g_parser.current.type != TOKEN_ERROR)
            break;

        // TOKEN_ERROR is purely synthetic,
        // so I report the error at the previous token.
        error_at(&g_parser.previous, g_parser.current.start);
    }
}

static void error_at(Token* token, const char* msg)
{
    // Don't display errors when in panic mode.
    if (g_parser.panic_mode)
        return;
    g_parser.panic_mode = true;

    err_error_at_token(token, msg);

    g_parser.had_error = true;
}

static void consume(TokenType type, const char* msg)
{
    if (g_parser.current.type == type)
    {
        advance();
        return;
    }

    error_at(&g_parser.current, msg);
}

static void expression()
{
    parse_precedence(PREC_ASSIGNMENT);
}

static void number()
{
    double value = strtod(g_parser.previous.start, NULL);
    emit_constant(value);
}

static void grouping()
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary()
{
    Token op = g_parser.previous;

    parse_precedence(PREC_UNARY);

    switch (op.type)
    {
        case TOKEN_MINUS:
            emit_byte(OP_NEGATE, op.line);
            break;
        default:
            err_error("%s\n[line %d] Error: not every case was handled.", __FILE__,
                      __LINE__);
            return;
    }
}

static void binary()
{
    Token op = g_parser.previous;

    ParseRule* rule = get_rule(op.type);
    parse_precedence((Precedence)(rule->precedence + 1));

    switch (op.type)
    {
        case TOKEN_PLUS:
            emit_byte(OP_ADD, op.line);
            break;
        case TOKEN_MINUS:
            emit_byte(OP_SUBTRACT, op.line);
            break;
        case TOKEN_STAR:
            emit_byte(OP_MULTIPLY, op.line);
            break;
        case TOKEN_SLASH:
            emit_byte(OP_DIVIDE, op.line);
            break;
        default:
            err_error("%s\n[line %d] Error: not every case was handled.", __FILE__,
                      __LINE__);
            return;
    }
}

static void parse_precedence(Precedence precedence)
{
    advance();

    // Prefix
    ParseFn prefix_rule = get_rule(g_parser.previous.type)->prefix;
    if (prefix_rule == NULL)
    {
        error_at(&g_parser.previous, "Expect expression.");
        return;
    }

    prefix_rule();

    // Infix
    while (precedence <= get_rule(g_parser.current.type)->precedence)
    {
        advance();
        ParseFn infix_rule = get_rule(g_parser.previous.type)->infix;
        infix_rule();
    }
}

static ParseRule* get_rule(TokenType type)
{
    return &g_rules[type];
}
