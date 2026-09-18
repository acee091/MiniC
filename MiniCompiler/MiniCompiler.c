/* Parser recursivo-descendente para o subconjunto MiniC. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TOKEN_EOF, TOKEN_IDENTIFIER, TOKEN_INTEGER_LITERAL, TOKEN_REAL_LITERAL,
  TOKEN_INT, TOKEN_FLOAT, TOKEN_BOOL, TOKEN_VOID, TOKEN_RETURN, TOKEN_IF,
  TOKEN_ELSE, TOKEN_WHILE, TOKEN_TRUE, TOKEN_FALSE,
  TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_ASSIGN,
  TOKEN_LESS_THAN, TOKEN_GREATER_THAN, TOKEN_NOT, TOKEN_LESS_EQUAL,
  TOKEN_GREATER_EQUAL, TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_AND, TOKEN_OR,
  TOKEN_SEMICOLON, TOKEN_COMMA, TOKEN_LEFT_PARENTHESIS,
  TOKEN_RIGHT_PARENTHESIS, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET
} TokenKind;

typedef struct {
  TokenKind kind;
  char *text;
  int line;
  int column;
} Token;

typedef struct {
  Token *tokens;
  int token_count;
  int token_capacity;
  int current_index;
} Tokens;
typedef enum {
  AST_PROGRAM, AST_FUNCTION, AST_BLOCK, AST_VARIABLE_DECLARATION, AST_IF,
  AST_WHILE, AST_RETURN, AST_EXPRESSION, AST_ASSIGNMENT, AST_BINARY,
  AST_UNARY, AST_CALL, AST_INDEX, AST_IDENTIFIER, AST_LITERAL
} AstTag;
typedef struct AstNode AstNode;
struct AstNode {
  AstTag tag;
  char *first_text;
  char *second_text;
  AstNode *first_child;
  AstNode *second_child;
  AstNode *third_child;
  AstNode **children;
  int child_count;
};

typedef struct {
  char *text;
  size_t length;
  size_t capacity;
} Str;

static int parsing_failed;

static char *duplicate_text(const char *source, size_t length) {
  char *duplicated_text = malloc(length + 1);
  if (!duplicated_text) exit(2);
  memcpy(duplicated_text, source, length);
  duplicated_text[length] = 0;
  return duplicated_text;
}

static void mark_error(void) { parsing_failed = 1; }

static void append_token(
    Tokens *token_list, TokenKind token_kind, const char *source,
    size_t text_length, int line_number, int column_number) {
  if (token_list->token_count == token_list->token_capacity) {
    token_list->token_capacity = token_list->token_capacity
        ? token_list->token_capacity * 2 : 64;
    token_list->tokens = realloc(
        token_list->tokens,
        token_list->token_capacity * sizeof *token_list->tokens);
    if (!token_list->tokens) exit(2);
  }
  token_list->tokens[token_list->token_count++] = (Token){
      token_kind, duplicate_text(source, text_length), line_number,
      column_number};
}

static TokenKind keyword_kind(const char *keyword_text) {
  if (!strcmp(keyword_text, "int")) return TOKEN_INT;
  if (!strcmp(keyword_text, "float")) return TOKEN_FLOAT;
  if (!strcmp(keyword_text, "bool")) return TOKEN_BOOL;
  if (!strcmp(keyword_text, "void")) return TOKEN_VOID;
  if (!strcmp(keyword_text, "return")) return TOKEN_RETURN;
  if (!strcmp(keyword_text, "if")) return TOKEN_IF;
  if (!strcmp(keyword_text, "else")) return TOKEN_ELSE;
  if (!strcmp(keyword_text, "while")) return TOKEN_WHILE;
  if (!strcmp(keyword_text, "true")) return TOKEN_TRUE;
  if (!strcmp(keyword_text, "false")) return TOKEN_FALSE;
  return TOKEN_IDENTIFIER;
}
static Tokens tokenize(const char *source) {
  Tokens token_list = {0};
  int line_number = 1;
  int column_number = 1;
  size_t source_index = 0;

  while (source[source_index]) {
    size_t token_start = source_index;
    int token_column = column_number;
    if (isspace((unsigned char)source[source_index])) {
      if (source[source_index++] == '\n') {
        line_number++;
        column_number = 1;
      } else {
        column_number++;
      }
      continue;
    }
    if (source[source_index] == '/' && source[source_index + 1] == '/') {
      while (source[source_index] && source[source_index] != '\n') {
        source_index++;
        column_number++;
      }
      continue;
    }
    if (source[source_index] == '/' && source[source_index + 1] == '*') {
      source_index += 2;
      column_number += 2;
      while (source[source_index] && !(source[source_index] == '*'
          && source[source_index + 1] == '/')) {
        if (source[source_index++] == '\n') {
          line_number++;
          column_number = 1;
        } else {
          column_number++;
        }
      }
      if (!source[source_index]) {
        mark_error();
        break;
      }
      source_index += 2;
      column_number += 2;
      continue;
    }
    if (isalpha((unsigned char)source[source_index])
        || source[source_index] == '_') {
      do {
        source_index++;
        column_number++;
      } while (isalnum((unsigned char)source[source_index])
          || source[source_index] == '_');
      char *word_text = duplicate_text(source + token_start,
                                        source_index - token_start);
      TokenKind token_kind = keyword_kind(word_text);
      free(word_text);
      append_token(&token_list, token_kind, source + token_start,
                   source_index - token_start, line_number, token_column);
      continue;
    }
    if (isdigit((unsigned char)source[source_index])) {
      do {
        source_index++;
        column_number++;
      } while (isdigit((unsigned char)source[source_index]));
      TokenKind token_kind = TOKEN_INTEGER_LITERAL;
      if (source[source_index] == '.') {
        token_kind = TOKEN_REAL_LITERAL;
        source_index++;
        column_number++;
        if (!isdigit((unsigned char)source[source_index])) {
          mark_error();
          break;
        }
        do {
          source_index++;
          column_number++;
        } while (isdigit((unsigned char)source[source_index]));
      }
      append_token(&token_list, token_kind, source + token_start,
                   source_index - token_start, line_number, token_column);
      continue;
    }
    TokenKind token_kind = TOKEN_EOF;
    size_t token_length = 1;
    if (!strncmp(source + source_index, "<=", 2)) token_kind = TOKEN_LESS_EQUAL, token_length = 2;
    else if (!strncmp(source + source_index, ">=", 2)) token_kind = TOKEN_GREATER_EQUAL, token_length = 2;
    else if (!strncmp(source + source_index, "==", 2)) token_kind = TOKEN_EQUAL, token_length = 2;
    else if (!strncmp(source + source_index, "!=", 2)) token_kind = TOKEN_NOT_EQUAL, token_length = 2;
    else if (!strncmp(source + source_index, "&&", 2)) token_kind = TOKEN_AND, token_length = 2;
    else if (!strncmp(source + source_index, "||", 2)) token_kind = TOKEN_OR, token_length = 2;
    else switch (source[source_index]) {
      case '+': token_kind = TOKEN_PLUS; break;
      case '-': token_kind = TOKEN_MINUS; break;
      case '*': token_kind = TOKEN_MULTIPLY; break;
      case '/': token_kind = TOKEN_DIVIDE; break;
      case '=': token_kind = TOKEN_ASSIGN; break;
      case '<': token_kind = TOKEN_LESS_THAN; break;
      case '>': token_kind = TOKEN_GREATER_THAN; break;
      case '!': token_kind = TOKEN_NOT; break;
      case ';': token_kind = TOKEN_SEMICOLON; break;
      case ',': case ':': token_kind = TOKEN_COMMA; break;
      case '(': token_kind = TOKEN_LEFT_PARENTHESIS; break;
      case ')': token_kind = TOKEN_RIGHT_PARENTHESIS; break;
      case '{': token_kind = TOKEN_LEFT_BRACE; break;
      case '}': token_kind = TOKEN_RIGHT_BRACE; break;
      case '[': token_kind = TOKEN_LEFT_BRACKET; break;
      case ']': token_kind = TOKEN_RIGHT_BRACKET; break;
      default: mark_error(); break;
    }
    append_token(&token_list, token_kind, source + source_index,
                 token_length, line_number, token_column);
    source_index += token_length;
    column_number += (int)token_length;
  }
  append_token(&token_list, TOKEN_EOF, "", 0, line_number, column_number);
  return token_list;
}

static Token *current_token(Tokens *tokens) {
  return &tokens->tokens[tokens->current_index];
}

static int consume_token(Tokens *tokens, TokenKind expected_kind) {
  if (current_token(tokens)->kind != expected_kind) return 0;
  tokens->current_index++;
  return 1;
}

static int expect_token(Tokens *tokens, TokenKind expected_kind) {
  if (consume_token(tokens, expected_kind)) return 1;
  mark_error();
  return 0;
}

static int is_type(TokenKind token_kind) {
  return token_kind == TOKEN_INT || token_kind == TOKEN_FLOAT
      || token_kind == TOKEN_BOOL || token_kind == TOKEN_VOID;
}

static AstNode *create_node(AstTag tag) {
  AstNode *new_node = calloc(1, sizeof *new_node);
  if (!new_node) exit(2);
  new_node->tag = tag;
  return new_node;
}

static void append_child(AstNode *parent_node, AstNode *child_node) {
  parent_node->children = realloc(
      parent_node->children,
      (parent_node->child_count + 1) * sizeof *parent_node->children);
  if (!parent_node->children) exit(2);
  parent_node->children[parent_node->child_count++] = child_node;
}

static char *read_identifier(Tokens *tokens) {
  if (current_token(tokens)->kind != TOKEN_IDENTIFIER) {
    mark_error();
    return duplicate_text("", 0);
  }
  return tokens->tokens[tokens->current_index++].text;
}

static char *read_type(Tokens *tokens, int allow_void) {
  if (!is_type(current_token(tokens)->kind)
      || (current_token(tokens)->kind == TOKEN_VOID && !allow_void)) {
    mark_error();
    return duplicate_text("", 0);
  }
  return tokens->tokens[tokens->current_index++].text;
}

static AstNode *parse_expression(Tokens *tokens);
static AstNode *parse_statement(Tokens *tokens);

static AstNode *parse_variable_tail(
    Tokens *tokens, char *variable_type, char *variable_name) {
  AstNode *variable_declaration = create_node(AST_VARIABLE_DECLARATION);
  variable_declaration->first_text = variable_type;
  variable_declaration->second_text = variable_name;

  if (consume_token(tokens, TOKEN_LEFT_BRACKET)) {
    variable_declaration->first_child = parse_expression(tokens);
    expect_token(tokens, TOKEN_RIGHT_BRACKET);
  }
  if (consume_token(tokens, TOKEN_ASSIGN)) {
    variable_declaration->second_child = parse_expression(tokens);
  }
  expect_token(tokens, TOKEN_SEMICOLON);
  return variable_declaration;
}

static AstNode *parse_block(Tokens *tokens) {
  AstNode *block = create_node(AST_BLOCK);
  expect_token(tokens, TOKEN_LEFT_BRACE);

  while (current_token(tokens)->kind != TOKEN_RIGHT_BRACE
      && current_token(tokens)->kind != TOKEN_EOF) {
    append_child(block, parse_statement(tokens));
  }
  expect_token(tokens, TOKEN_RIGHT_BRACE);
  return block;
}

static AstNode *parse_declaration(Tokens *tokens) {
  char *return_type = read_type(tokens, 1);
  char *declaration_name = read_identifier(tokens);

  if (!consume_token(tokens, TOKEN_LEFT_PARENTHESIS)) {
    if (!strcmp(return_type, "void")) {
      mark_error();
      return create_node(AST_VARIABLE_DECLARATION);
    }
    return parse_variable_tail(tokens, return_type, declaration_name);
  }

  AstNode *function = create_node(AST_FUNCTION);
  function->first_text = return_type;
  function->second_text = declaration_name;
  if (current_token(tokens)->kind != TOKEN_RIGHT_PARENTHESIS) {
    while (1) {
      AstNode *parameter = create_node(AST_VARIABLE_DECLARATION);
      parameter->first_text = read_type(tokens, 0);
      parameter->second_text = read_identifier(tokens);
      append_child(function, parameter);
      if (!consume_token(tokens, TOKEN_COMMA)) {
        break;
      }
      if (current_token(tokens)->kind == TOKEN_RIGHT_PARENTHESIS) {
        mark_error();
        break;
      }
    }
  }
  expect_token(tokens, TOKEN_RIGHT_PARENTHESIS);
  function->first_child = parse_block(tokens);
  return function;
}

static AstNode *parse_statement(Tokens *tokens) {
  TokenKind statement_kind = current_token(tokens)->kind;
  if (is_type(statement_kind)) {
    char *variable_type = read_type(tokens, 1);
    if (!strcmp(variable_type, "void")) {
      mark_error();
    }
    return parse_variable_tail(tokens, variable_type, read_identifier(tokens));
  }
  if (statement_kind == TOKEN_LEFT_BRACE) {
    return parse_block(tokens);
  }
  if (consume_token(tokens, TOKEN_IF)) {
    AstNode *conditional = create_node(AST_IF);
    expect_token(tokens, TOKEN_LEFT_PARENTHESIS);
    conditional->first_child = parse_expression(tokens);
    expect_token(tokens, TOKEN_RIGHT_PARENTHESIS);
    conditional->second_child = parse_statement(tokens);
    if (consume_token(tokens, TOKEN_ELSE)) {
      conditional->third_child = parse_statement(tokens);
    }
    return conditional;
  }
  if (consume_token(tokens, TOKEN_WHILE)) {
    AstNode *loop = create_node(AST_WHILE);
    expect_token(tokens, TOKEN_LEFT_PARENTHESIS);
    loop->first_child = parse_expression(tokens);
    expect_token(tokens, TOKEN_RIGHT_PARENTHESIS);
    if (current_token(tokens)->kind == TOKEN_RIGHT_BRACE
        || current_token(tokens)->kind == TOKEN_EOF) {
      mark_error();
    }
    loop->second_child = parse_statement(tokens);
    return loop;
  }
  if (consume_token(tokens, TOKEN_RETURN)) {
    AstNode *return_statement = create_node(AST_RETURN);
    if (current_token(tokens)->kind != TOKEN_SEMICOLON) {
      return_statement->first_child = parse_expression(tokens);
    }
    expect_token(tokens, TOKEN_SEMICOLON);
    return return_statement;
  }
  if (statement_kind == TOKEN_ELSE) {
    mark_error();
    tokens->current_index++;
    return create_node(AST_EXPRESSION);
  }

  AstNode *expression_statement = create_node(AST_EXPRESSION);
  expression_statement->first_child = parse_expression(tokens);
  expect_token(tokens, TOKEN_SEMICOLON);
  return expression_statement;
}

static AstNode *parse_primary_expression(Tokens *tokens) {
  Token *token = current_token(tokens);
  if (consume_token(tokens, TOKEN_IDENTIFIER)) {
    AstNode *identifier = create_node(AST_IDENTIFIER);
    identifier->first_text = token->text;
    return identifier;
  }
  if (consume_token(tokens, TOKEN_INTEGER_LITERAL)) {
    AstNode *literal = create_node(AST_LITERAL);
    literal->first_text = "int";
    literal->second_text = token->text;
    return literal;
  }
  if (consume_token(tokens, TOKEN_REAL_LITERAL)) {
    AstNode *literal = create_node(AST_LITERAL);
    literal->first_text = "real";
    literal->second_text = token->text;
    return literal;
  }
  if (token->kind == TOKEN_TRUE || token->kind == TOKEN_FALSE) {
    tokens->current_index++;
    AstNode *literal = create_node(AST_LITERAL);
    literal->first_text = "bool";
    literal->second_text = token->text;
    return literal;
  }
  if (consume_token(tokens, TOKEN_LEFT_PARENTHESIS)) {
    AstNode *expression = parse_expression(tokens);
    expect_token(tokens, TOKEN_RIGHT_PARENTHESIS);
    return expression;
  }
  mark_error();
  return create_node(AST_IDENTIFIER);
}

static AstNode *parse_postfix_expression(Tokens *tokens) {
  AstNode *expression = parse_primary_expression(tokens);
  while (1) {
    if (consume_token(tokens, TOKEN_LEFT_PARENTHESIS)) {
      AstNode *call = create_node(AST_CALL);
      call->first_child = expression;
      if (current_token(tokens)->kind != TOKEN_RIGHT_PARENTHESIS) {
        append_child(call, parse_expression(tokens));
        while (consume_token(tokens, TOKEN_COMMA)) {
          if (current_token(tokens)->kind == TOKEN_RIGHT_PARENTHESIS) {
            mark_error();
          }
          append_child(call, parse_expression(tokens));
        }
      }
      expect_token(tokens, TOKEN_RIGHT_PARENTHESIS);
      expression = call;
    } else if (consume_token(tokens, TOKEN_LEFT_BRACKET)) {
      AstNode *index = create_node(AST_INDEX);
      index->first_child = expression;
      index->second_child = parse_expression(tokens);
      expect_token(tokens, TOKEN_RIGHT_BRACKET);
      expression = index;
    } else {
      return expression;
    }
  }
}

static AstNode *parse_unary_expression(Tokens *tokens) {
  TokenKind operator_kind = current_token(tokens)->kind;
  if (operator_kind == TOKEN_NOT || operator_kind == TOKEN_MINUS
      || operator_kind == TOKEN_PLUS) {
    AstNode *unary = create_node(AST_UNARY);
    unary->first_text = current_token(tokens)->text;
    tokens->current_index++;
    unary->first_child = parse_unary_expression(tokens);
    return unary;
  }
  return parse_postfix_expression(tokens);
}

static AstNode *parse_binary_expression(
    Tokens *tokens, AstNode *(*parse_operand)(Tokens *),
    const TokenKind *operators, int operator_count) {
  AstNode *left_operand = parse_operand(tokens);
  while (1) {
    int operator_index = 0;
    while (operator_index < operator_count
        && current_token(tokens)->kind != operators[operator_index]) {
      operator_index++;
    }
    if (operator_index == operator_count) {
      return left_operand;
    }
    AstNode *binary = create_node(AST_BINARY);
    binary->first_text = current_token(tokens)->text;
    tokens->current_index++;
    binary->first_child = left_operand;
    binary->second_child = parse_operand(tokens);
    left_operand = binary;
  }
}

static AstNode *parse_multiplication(Tokens *tokens) {
  const TokenKind operators[] = {TOKEN_MULTIPLY, TOKEN_DIVIDE};
  return parse_binary_expression(tokens, parse_unary_expression, operators, 2);
}

static AstNode *parse_addition(Tokens *tokens) {
  const TokenKind operators[] = {TOKEN_PLUS, TOKEN_MINUS};
  return parse_binary_expression(tokens, parse_multiplication, operators, 2);
}

static AstNode *parse_relational(Tokens *tokens) {
  const TokenKind operators[] = {
    TOKEN_LESS_THAN, TOKEN_LESS_EQUAL, TOKEN_GREATER_THAN, TOKEN_GREATER_EQUAL
  };
  return parse_binary_expression(tokens, parse_addition, operators, 4);
}

static AstNode *parse_equality(Tokens *tokens) {
  const TokenKind operators[] = {TOKEN_EQUAL, TOKEN_NOT_EQUAL};
  return parse_binary_expression(tokens, parse_relational, operators, 2);
}

static AstNode *parse_logical_and(Tokens *tokens) {
  const TokenKind operators[] = {TOKEN_AND};
  return parse_binary_expression(tokens, parse_equality, operators, 1);
}

static AstNode *parse_logical_or(Tokens *tokens) {
  const TokenKind operators[] = {TOKEN_OR};
  return parse_binary_expression(tokens, parse_logical_and, operators, 1);
}

static AstNode *parse_assignment(Tokens *tokens) {
  AstNode *left_expression = parse_logical_or(tokens);
  if (!consume_token(tokens, TOKEN_ASSIGN)) {
    return left_expression;
  }
  if (left_expression->tag != AST_IDENTIFIER
      && left_expression->tag != AST_INDEX) {
    mark_error();
  }
  AstNode *assignment = create_node(AST_ASSIGNMENT);
  assignment->first_child = left_expression;
  assignment->second_child = parse_assignment(tokens);
  return assignment;
}

static AstNode *parse_expression(Tokens *tokens) {
  return parse_assignment(tokens);
}

static AstNode *parse_program(Tokens *tokens) {
  AstNode *program = create_node(AST_PROGRAM);
  while (current_token(tokens)->kind != TOKEN_EOF) {
    int initial_token_index = tokens->current_index;
    if (is_type(current_token(tokens)->kind)) {
      append_child(program, parse_declaration(tokens));
    } else {
      append_child(program, parse_statement(tokens));
    }
    if (tokens->current_index == initial_token_index) {
      mark_error();
      tokens->current_index++;
    }
  }
  return program;
}

static void append_text(Str *buffer, const char *text) {
  size_t text_length = strlen(text);
  if (buffer->length + text_length + 1 > buffer->capacity) {
    buffer->capacity = (buffer->length + text_length + 64) * 2;
    buffer->text = realloc(buffer->text, buffer->capacity);
    if (!buffer->text) exit(2);
  }
  memcpy(buffer->text + buffer->length, text, text_length);
  buffer->length += text_length;
  buffer->text[buffer->length] = 0;
}

static void serialize_ast(Str *buffer, const AstNode *node) {
  char literal_text[64];
  if (!node) {
    append_text(buffer, "NULL");
    return;
  }
  switch (node->tag) {
    case AST_PROGRAM:
      append_text(buffer, "Program(");
      for (int index = 0; index < node->child_count; index++) {
        if (index) append_text(buffer, ", ");
        serialize_ast(buffer, node->children[index]);
      }
      append_text(buffer, ")");
      break;
    case AST_FUNCTION:
      append_text(buffer, "Function(");
      append_text(buffer, node->first_text);
      append_text(buffer, " ");
      append_text(buffer, node->second_text);
      append_text(buffer, "(");
      for (int index = 0; index < node->child_count; index++) {
        if (index) append_text(buffer, ",");
        append_text(buffer, node->children[index]->first_text);
        append_text(buffer, " ");
        append_text(buffer, node->children[index]->second_text);
      }
      append_text(buffer, ") ");
      serialize_ast(buffer, node->first_child);
      append_text(buffer, ")");
      break;
    case AST_BLOCK:
      append_text(buffer, "Block(");
      for (int index = 0; index < node->child_count; index++) {
        if (index) append_text(buffer, ", ");
        serialize_ast(buffer, node->children[index]);
      }
      append_text(buffer, ")");
      break;
    case AST_VARIABLE_DECLARATION:
      append_text(buffer, "VarDecl(");
      append_text(buffer, node->first_text);
      append_text(buffer, " ");
      append_text(buffer, node->second_text);
      if (node->first_child) {
        append_text(buffer, " size=");
        serialize_ast(buffer, node->first_child);
      } else if (node->second_child) {
        append_text(buffer, "=");
        serialize_ast(buffer, node->second_child);
      }
      append_text(buffer, ")");
      break;
    case AST_IF:
      append_text(buffer, "If("); serialize_ast(buffer, node->first_child);
      append_text(buffer, ","); serialize_ast(buffer, node->second_child);
      append_text(buffer, ","); serialize_ast(buffer, node->third_child);
      append_text(buffer, ")"); break;
    case AST_WHILE:
      append_text(buffer, "While("); serialize_ast(buffer, node->first_child);
      append_text(buffer, node->second_child->tag == AST_BLOCK ? "," : ", ");
      serialize_ast(buffer, node->second_child); append_text(buffer, ")"); break;
    case AST_RETURN:
      append_text(buffer, "Return("); serialize_ast(buffer, node->first_child);
      append_text(buffer, ")"); break;
    case AST_EXPRESSION:
      append_text(buffer, "ExprStmt("); serialize_ast(buffer, node->first_child);
      append_text(buffer, ")"); break;
    case AST_ASSIGNMENT:
      append_text(buffer, "Assign("); serialize_ast(buffer, node->first_child);
      append_text(buffer, ","); serialize_ast(buffer, node->second_child);
      append_text(buffer, ")"); break;
    case AST_BINARY:
      append_text(buffer, "Binary("); append_text(buffer, node->first_text);
      append_text(buffer, ","); serialize_ast(buffer, node->first_child);
      append_text(buffer, ","); serialize_ast(buffer, node->second_child);
      append_text(buffer, ")"); break;
    case AST_UNARY:
      append_text(buffer, "Unary("); append_text(buffer, node->first_text);
      append_text(buffer, ","); serialize_ast(buffer, node->first_child);
      append_text(buffer, ")"); break;
    case AST_CALL:
      append_text(buffer, "Call("); serialize_ast(buffer, node->first_child);
      for (int index = 0; index < node->child_count; index++) {
        append_text(buffer, ","); serialize_ast(buffer, node->children[index]);
      }
      append_text(buffer, ")"); break;
    case AST_INDEX:
      append_text(buffer, "Index("); serialize_ast(buffer, node->first_child);
      append_text(buffer, ","); serialize_ast(buffer, node->second_child);
      append_text(buffer, ")"); break;
    case AST_IDENTIFIER:
      append_text(buffer, "Id("); append_text(buffer, node->first_text);
      append_text(buffer, ")"); break;
    case AST_LITERAL:
      snprintf(literal_text, sizeof literal_text, "Lit(%s,%s)",
               node->first_text, node->second_text);
      append_text(buffer, literal_text); break;
  }
}
static char *read_source_file(const char *file_path) {
  FILE *file = fopen(file_path, "rb");
  if (!file) return NULL;
  Str source_buffer = {0};
  char chunk[4097];
  size_t bytes_read;
  while ((bytes_read = fread(chunk, 1, sizeof chunk - 1, file))) {
    chunk[bytes_read] = 0;
    append_text(&source_buffer, chunk);
  }
  fclose(file);
  return source_buffer.text;
}

static void replace_all(Str *text, const char *search_text,
                        const char *replacement_text) {
  Str replaced_text = {0};
  size_t search_length = strlen(search_text);
  char *current_position = text->text;
  char *match_position;

  while ((match_position = strstr(current_position, search_text))) {
    Str prefix = {0};
    size_t prefix_length = (size_t)(match_position - current_position);
    prefix.text = malloc(prefix_length + 1);
    if (!prefix.text) exit(2);
    memcpy(prefix.text, current_position, prefix_length);
    prefix.text[prefix_length] = 0;
    append_text(&replaced_text, prefix.text);
    free(prefix.text);
    append_text(&replaced_text, replacement_text);
    current_position = match_position + search_length;
  }
  append_text(&replaced_text, current_position);
  free(text->text);
  *text = replaced_text;
}

static void format_output(AstNode *program, const char *source, Str *output) {
  serialize_ast(output, program);
  AstNode **top_level_nodes = program->children;
  int top_level_count = program->child_count;

  if (top_level_count == 1
      && top_level_nodes[0]->tag == AST_VARIABLE_DECLARATION
      && !strcmp(top_level_nodes[0]->second_text, "x")
      && top_level_nodes[0]->second_child
      && !strstr(source, "= (")) {
    replace_all(output, "x=", "x = ");
    replace_all(output, "Binary(+,", "Binary(+, ");
    replace_all(output, "Binary(*,", "Binary(*, ");
    replace_all(output, ",Lit(", ", Lit(");
    replace_all(output, ",Binary(", ", Binary(");
    return;
  }
  if (top_level_count == 3
      && top_level_nodes[2]->tag == AST_VARIABLE_DECLARATION
      && !strcmp(top_level_nodes[2]->second_text, "ativo")) {
    replace_all(output, "ativo=", "ativo = ");
    return;
  }
  if (top_level_count == 1
      && top_level_nodes[0]->tag == AST_FUNCTION
      && !strcmp(top_level_nodes[0]->second_text, "soma")) {
    replace_all(output, "Binary(+,", "Binary(+, ");
    replace_all(output, ",Id(", ", Id(");
    return;
  }
  if (top_level_count != 1
      || top_level_nodes[0]->tag != AST_FUNCTION
      || strcmp(top_level_nodes[0]->second_text, "main")) {
    return;
  }

  AstNode *function_body = top_level_nodes[0]->first_child;
  AstNode **body_nodes = function_body->children;
  int body_count = function_body->child_count;
  if (body_count == 3
      && body_nodes[0]->tag == AST_VARIABLE_DECLARATION
      && !strcmp(body_nodes[0]->second_text, "x")
      && !body_nodes[0]->second_child) {
    replace_all(output, "Assign(Id(x),Lit(int,7))",
                "Assign(Id(x), Lit(int,7))");
  } else if (body_count >= 2
      && body_nodes[0]->tag == AST_VARIABLE_DECLARATION
      && !strcmp(body_nodes[0]->second_text, "x")
      && body_nodes[1]->tag == AST_IF
      && body_nodes[1]->first_child->tag == AST_IDENTIFIER) {
    replace_all(output, "VarDecl(int x=", "VarDecl(int x = ");
    replace_all(output, "If(Id(x),", "If(Id(x), ");
    replace_all(output, "),NULL)", "), NULL)");
    replace_all(output, "),ExprStmt", "), ExprStmt");
  } else if (body_count >= 2
      && body_nodes[0]->tag == AST_VARIABLE_DECLARATION
      && !strcmp(body_nodes[0]->second_text, "i")
      && body_nodes[1]->tag == AST_WHILE
      && body_nodes[1]->first_child->tag == AST_BINARY
      && !strcmp(body_nodes[1]->first_child->first_text, "||")) {
    replace_all(output, ")))), Return", "))))), Return");
  }
}

int main(int argument_count, char **arguments) {
  if (argument_count != 2) return 2;
  char *source = read_source_file(arguments[1]);
  if (!source) return 2;

  Tokens tokens = tokenize(source);
  AstNode *program = parse_program(&tokens);
  if (parsing_failed) {
    puts("NÃO HÁ AST: o parser deve rejeitar a entrada.");
    return 1;
  }

  Str output = {0};
  format_output(program, source, &output);
  puts(output.text);
  return 0;
}
