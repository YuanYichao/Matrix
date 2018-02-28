#ifndef PARSER_TYPES
#define PARSER_TYPES
#include <string>

enum token_type : unsigned char {
  VALUE = 1 << 0,
  NAME = 1 << 1,
  OPERATOR = 1 << 2,
  SEQUENCE_BEG = 1 << 3,
  SEQUENCE_END = 1 << 4
};
enum op_types :unsigned char{ MULTI, DIVIDE, PLUS, MINUS, NEGATIVE };
enum op_level :unsigned char{ PLS, MUL, UNA };
const std::string operator_table[4] = {"+", "-", "*", "/"};
#endif