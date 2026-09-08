#include "Parser.hpp"
#include "NodeFactory.hpp"
#include "error/ErrorHandler.hpp"
#include "lexer/Token.hpp"
#include "tools/AST.hpp"
#include "tools/OwnershipMod.hpp"
#include "tools/ParseError.hpp"
#include "tools/SourceLocation.hpp"
#include "tools/VisMod.hpp"
#include <expected>
#include <memory>
#include <utility>
#include <vector>

/**
 * @brief forward facing method to begin parsing process
 *
 * @return the head of the newly created abstract syntax tree
 */
std::unique_ptr<AST> Parser::parse() {
  return parse_program(get_loc()).value();
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_program(SourceLocation loc) {
  std::vector<std::unique_ptr<AST>> decls;
  while (!is_at_end()) {
    auto top = parse_top_level_decl(loc);

    if (!top)
      handle_parser_error(top.error(), curr_token);
    decls.push_back(std::move(*top));
  }

  return make_program_node(loc, std::move(decls));
}

/**
 * @brief parses a top level declaration
 *
 * A top level declaration is defined as any declaration or definition that
 * takes place outside of a function, struct, or any other body.
 *
 * @return AST node for the top level declaration
 *
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_top_level_decl(SourceLocation loc) {
  VisMod vis_mod = VisMod::PRIV;
  if (auto pub = consume(TokenType::PUB); pub)
    vis_mod = VisMod::PUB;
  else
    auto priv = consume(TokenType::PRIV);

  if (consume(TokenType::IMPORT))
    return parse_import(loc);
  if (consume(TokenType::ATTRIBUTE))
    return parse_attribute(loc);
  if (consume(TokenType::FUNC))
    return parse_function_definition(loc, vis_mod);
  if (consume(TokenType::STRUCT))
    return parse_struct_definition(loc, vis_mod);
  if (consume(TokenType::ENUM))
    return parse_enum_definition(loc, vis_mod);
  if (consume(TokenType::TRAIT))
    return parse_trait_definition(loc, vis_mod);
  if (consume(TokenType::IMPL))
    return parse_impl_definition(loc, vis_mod);
  if (auto owner = consume(TokenType::OWNED); owner)
    return parse_variable_definition(loc, vis_mod);
  if (auto owner = consume(TokenType::STATIC); owner)
    return parse_variable_definition(loc, vis_mod,
                                     convert_ownership(owner->type));
  if (auto owner = consume(TokenType::REF); owner)
    return parse_variable_definition(loc, vis_mod,
                                     convert_ownership(owner->type));
  if (auto owner = consume(TokenType::SHARED); owner)
    return parse_variable_definition(loc, vis_mod,
                                     convert_ownership(owner->type));
  if (auto owner = consume(TokenType::CONST); owner)
    return parse_variable_definition(loc, vis_mod,
                                     convert_ownership(owner->type));
  if (expect(TokenType::IDENTIFIER))
    return parse_variable_definition(loc, vis_mod);
  if (expect(TokenType::END_OF_FILE))
    return std::unexpected(ParseError::UnexpectedEOF);
  return std::unexpected(ParseError::UnexpectedToken);
}

/**
 * @brief parses an import statement
 *
 * @astfields
 * module (string):  name of the module being imported
 *
 * @return import statement node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_import(SourceLocation loc) {
  auto import_name = consume(TokenType::IDENTIFIER);
  if (!import_name)
    return std::unexpected(import_name.error());

  return make_import_node(loc, import_name->lexeme);
}

/**
 * @brief parse attribute
 * Attributes are tied to specific functions.
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_attribute(SourceLocation loc) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  return make_attribute_node(loc, name->lexeme);
}

/**
 * @brief parses a function definition
 *
 * @astfields
 * attributes (vector<AST>):  a list of attribute nodes
 * vis_mod (VisMod):          the visibility of the function
 * name (string):             name of the function
 * generics (vector<AST>):    list of generic nodes
 * param_list (vector<AST>):  list of param nodes
 * function_return (AST):     type that the function returns
 * block (vector<AST>):       list of statements made within the function
 *
 * @return function definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_definition(SourceLocation loc, VisMod vis_mod) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  auto gen_dec = parse_generic_declaration(get_loc());
  if (!gen_dec)
    return std::unexpected(gen_dec.error());

  auto param_list = parse_param_list(get_loc());
  if (!param_list)
    return std::unexpected(param_list.error());

  auto ret = parse_function_return(get_loc());
  if (!ret)
    return std::unexpected(ret.error());

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  return make_func_def_node(loc, vis_mod, name->lexeme, std::move(*gen_dec),
                            std::move(*param_list), std::move(*ret),
                            std::move(*block));
}

/**
 * @brief parses function declaration
 *
 * @astfields
 * vis_mod (VisMod):          visibility modifier
 * name (string):             name of the function
 * generics (AST):            list of function generic types
 * param_list (vector<AST>):  list of parameters
 * function_return (AST):     type the function returns
 *
 * @return function declaration AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_declaration(SourceLocation loc, VisMod vis_mod) {
  auto func = consume(TokenType::FUNC);
  if (!func)
    return std::unexpected(func.error());

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  auto gen_dec = parse_generic_declaration(get_loc());
  if (!gen_dec)
    return std::unexpected(gen_dec.error());

  auto param_list = parse_param_list(get_loc());
  if (!param_list)
    return std::unexpected(param_list.error());

  auto function_return = parse_function_return(get_loc());
  if (!function_return)
    return std::unexpected(function_return.error());

  return make_func_decl_node(loc, vis_mod, name->lexeme, std::move(*gen_dec),
                             std::move(*param_list),
                             std::move(*function_return));
}

/**
 * @brief parses a function return
 *
 * @astfields
 * ownership (OwnershipMod):  the ownership type of the return value
 * type (Type):               the type the return value must be
 *
 * @return function return AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_function_return(SourceLocation loc) {
  auto point = consume(TokenType::RETURN_POINT);
  if (!point)
    return std::unexpected(point.error());

  OwnershipMod owner = get_ownership();

  auto ret_type = get_type();

  if (!ret_type)
    return std::unexpected(ParseError::UnexpectedToken);

  return make_function_return_node(loc, owner, *ret_type);
}

/**
 * @brief parses parameters from a function definition/declaration
 *
 * @return a list of param AST nodes
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_param_list(SourceLocation loc) {
  std::vector<std::unique_ptr<AST>> param_list;

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  if (consume(TokenType::RPAREN))
    return param_list;

  do {
    auto param = parse_param(get_loc());
    if (!param)
      return param_list;
    param_list.push_back(std::move(*param));
  } while (consume(TokenType::COMMA));

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  return param_list;
}

/**
 * @brief parses a parameter
 *
 * @astfields
 * ownership (OwnershipMod):  modifies how this parameter is owned
 * name (string):             name of the parameter
 * is_array (boolean):        signifies if this parameter is an array type
 * type (Type):               type of the parameter. If an array, then type that
 *                            the array contains
 *
 * @return a param AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_param(SourceLocation loc) {
  OwnershipMod owner = get_ownership();

  auto name = consume(TokenType::IDENTIFIER);

  bool is_array = false;
  if (consume(TokenType::LBRACKET)) {
    is_array = true;
    if (!consume(TokenType::RBRACKET))
      return std::unexpected(ParseError::UnexpectedToken);
  }

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  auto param_type = get_type();
  if (!param_type)
    return std::unexpected(ParseError::UnexpectedToken);

  return make_param_node(loc, owner, name->lexeme, is_array, *param_type);
}

/**
 * @brief parse a struct definition
 *
 * @astfields
 * vis_mod (VisMod):        visibility modefier for struct
 * name (string):           name of the struct
 * generics (vector<AST>):  generic types in struct
 * fields (vector<AST>):    fields associated with the struct
 *
 * @return struct definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_struct_definition(SourceLocation loc, VisMod vis_mod) {

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  auto gen_dec = parse_generic_declaration(get_loc());
  if (!gen_dec)
    return std::unexpected(gen_dec.error());

  auto fields = parse_struct_block(get_loc());
  if (!fields)
    return std::unexpected(fields.error());

  return make_struct_node(loc, vis_mod, name->lexeme, std::move(*gen_dec),
                          std::move(*fields));
}

/**
 * @brief parse a struct block
 *
 * @return a vector of field AST
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_struct_block(SourceLocation loc) {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  auto fields = parse_struct_fields(loc);
  if (!fields)
    return std::unexpected(fields.error());

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return fields;
}

/**
 * @brief parse struct fields
 *
 * @return vector of field ASTs
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_struct_fields(SourceLocation loc) {
  std::vector<std::unique_ptr<AST>> fields;

  do {
    auto field = parse_struct_field(loc);
    if (!field)
      return std::unexpected(field.error());
    fields.push_back(std::move(*field));
    loc = get_loc();
  } while (!expect(TokenType::RBRACE));

  return fields;
}

/**
 * @brief parse a struct field
 *
 * @astfields
 * vis_mod (VisMod):          visibility modifier for the field
 * ownership (OwnershipMod):  ownership modifier for the field
 * name (string):             name of the field
 * is_array (boolean):        signals if field is an array type
 * type (Type):               type of the field
 *
 * @return field AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_struct_field(SourceLocation loc) {
  auto vis_mod = get_visibility();
  auto ownership = get_ownership();

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  bool is_array = true;
  if (auto lbracket = consume(TokenType::LBRACKET); !lbracket) {
    is_array = false;
    if (auto rbracket = consume(TokenType::RBRACKET); !rbracket)
      return std::unexpected(rbracket.error());
  }

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  auto type = get_type();
  if (!type)
    return std::unexpected(ParseError::UnexpectedEOF);

  return make_struct_field_node(loc, vis_mod, ownership, name->lexeme, is_array,
                                *type);
}

/**
 * @brief parse an enum definition
 *
 * @astfields
 * vis_mod (VisMod):        visibility modifier for the enum
 * name (string):           name of the enum
 * generics (vector<AST>):  generics associated with the enum
 * enum_vals (vector<AST>): values defined within the enum
 *
 * @return enum definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_enum_definition(SourceLocation loc, VisMod vis_mod) {

  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  auto gen_dec = parse_generic_declaration(get_loc());
  if (!gen_dec)
    return std::unexpected(gen_dec.error());

  auto enum_values = parse_enum_block(get_loc());
  if (!enum_values)
    return std::unexpected(enum_values.error());

  return make_enum_node(loc, vis_mod, name->lexeme, std::move(*gen_dec),
                        std::move(*enum_values));
}

/**
 * @brief parses an enum block
 *
 * @return vector of enum values
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_enum_block(SourceLocation loc) {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> values;

  do {
    auto value = parse_enum_value(loc);
    if (!value)
      return std::unexpected(value.error());
    values.push_back(std::move(*value));
    loc = get_loc();
  } while (consume(TokenType::COMMA));

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());
  return values;
}

/**
 * @brief parses an enum value
 *
 * @astfields
 * name (string):         name of the enum value
 * fields (vector<AST>):  vector of field AST nodes
 *
 * @return enum value AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_enum_value(SourceLocation loc) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  std::vector<std::unique_ptr<AST>> fields;
  if (consume(TokenType::LBRACE)) {
    do {
      auto field = parse_enum_field(get_loc());
      if (!field)
        return std::unexpected(field.error());
      fields.push_back(std::move(*field));
    } while (consume(TokenType::COMMA));

    if (auto brace = consume(TokenType::RBRACE); !brace)
      return std::unexpected(brace.error());
  }

  return make_enum_value_node(loc, name->lexeme, std::move(fields));
}

/**
 * @brief parse an enum value field
 *
 * @astfields
 * name (string):   name of the field
 * type (Type):     field type
 *
 * @return an enum value field AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_enum_field(SourceLocation loc) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  auto type = get_type();
  if (!type)
    return std::unexpected(ParseError::UnexpectedToken);

  return make_value_field_node(loc, name->lexeme, *type);
}

/**
 * @brief parse trait definition
 *
 * @astfields
 * vis_mod (VisMod):      visibility modifier for trait
 * name (string):         name of the trait
 * inherits (AST):        trait that this trait inherits from
 * block (vector<AST>):   body of the trait
 *
 * @return trait definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_trait_definition(SourceLocation loc, VisMod vis_mod) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  std::expected<std::unique_ptr<AST>, ParseError> inherits;
  if (consume(TokenType::COLON)) {
    inherits = parse_inherits(get_loc());
    if (!inherits)
      return std::unexpected(inherits.error());
  }

  auto body = parse_trait_block(get_loc());
  if (!body)
    return std::unexpected(body.error());

  return make_trait_node(loc, vis_mod, name->lexeme, std::move(*inherits),
                         std::move(*body));
}

/**
 * @brief parse trait block
 *
 * @return vector of AST nodes. Trait blocks will be made of function
 * declarations
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_trait_block(SourceLocation loc) {
  std::vector<std::unique_ptr<AST>> block;

  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  do {
    if (consume(TokenType::ATTRIBUTE)) {
      auto attribute = parse_attribute(loc);
      if (!attribute)
        return std::unexpected(attribute.error());
      block.push_back(std::move(*attribute));
      continue;
    }
    if (auto keyword = consume(TokenType::FUNC); !keyword)
      return std::unexpected(keyword.error());
    auto func = parse_function_declaration(loc);
    if (!func)
      return std::unexpected(func.error());
    block.push_back(std::move(*func));
    if (auto semicolon = consume(TokenType::SEMICOLON); !semicolon)
      return std::unexpected(semicolon.error());
    loc = get_loc();
  } while (consume(TokenType::FUNC) || peek().type == TokenType::PRIV ||
           peek().type == TokenType::PUB);

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return block;
}

/**
 * @brief parse implementation definition
 * implementations implement functions for any type, including implementing
 * traits for specific types
 *
 * @astfields
 * vis_mod (VisMod):          visibility modifier for the implementation
 * name (string):             name of the type being implemented
 * trait (string):            name of the trait being implemented
 * generics (vector<AST>):    generics declared or defined for implementation
 * impl_block (vector<AST>):  block for implementation
 *
 * @return implementation definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_impl_definition(SourceLocation loc, VisMod vis_mod) {
  auto id_trait = consume(TokenType::IDENTIFIER);
  if (!id_trait)
    return std::unexpected(id_trait.error());

  bool with_trait = false;
  if (consume(TokenType::FOR)) {
    with_trait = true;
  }
  auto id_name = consume(TokenType::IDENTIFIER);
  if (with_trait) {
    if (!id_name)
      return std::unexpected(id_name.error());
  }

  auto generics = parse_generic_declaration(get_loc());
  if (!generics)
    return std::unexpected(generics.error());

  auto block = parse_impl_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  return make_impl_node(loc, vis_mod, id_name->lexeme, id_trait->lexeme,
                        std::move(*generics), std::move(*block), with_trait);
}

/**
 * @brief parse implementation block
 * block will only contain function definitions
 *
 * @return vector of function definition AST nodes
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_impl_block(SourceLocation loc) {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> implementations;

  do {
    if (consume(TokenType::ATTRIBUTE)) {
      auto attribute = parse_attribute(loc);
      if (!attribute)
        return std::unexpected(attribute.error());
      implementations.push_back(std::move(*attribute));
      continue;
    }
    if (auto keyword = consume(TokenType::FUNC); !keyword)
      return std::unexpected(keyword.error());
    auto func = parse_function_definition(loc);
    if (!func)
      return std::unexpected(func.error());
    implementations.push_back(std::move(*func));
    loc = get_loc();
  } while (consume(TokenType::FUNC) || peek().type == TokenType::PRIV ||
           peek().type == TokenType::PUB);

  if (auto brace = consume(TokenType::RBRACE); !brace)
    return std::unexpected(brace.error());

  return implementations;
}

/**
 * @brief parse variable definition
 *
 * @astfields
 * decl (AST):        variable declaration AST node
 * expression (AST):  expression AST node. evaluates to the value being assigned
 *
 * @return variable definition AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_variable_definition(SourceLocation loc, VisMod vis_mod,
                                  OwnershipMod ownership) {
  auto decl = parse_variable_declaration(loc, vis_mod, ownership);
  if (!decl)
    return std::unexpected(decl.error());

  // if the statement ends after the declaration, then return a variable
  // declaration instead of a definition
  if (consume(TokenType::SEMICOLON))
    return decl;

  if (auto assign = consume(TokenType::ASSIGN); !assign)
    return std::unexpected(assign.error());

  auto expression = parse_expression(get_loc());
  if (!expression)
    return std::unexpected(expression.error());

  return make_var_def_node(loc, std::move(*decl), std::move(*expression));
}

/**
 * @brief parse variable declaration
 *
 * @astfields
 * vis_mod (VisMod):            visibility modifier for the variable
 * ownership (OwnershipMod):    ownership modifier for the variable
 * name (string):               name of the variable
 * array_size (integer):        if the variable is an array, this will hold the
 * size type (Type):                 the variable's type
 *
 * @return variable declaration AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_variable_declaration(SourceLocation loc, VisMod vis_mod,
                                   OwnershipMod ownership) {
  auto name = consume(TokenType::IDENTIFIER);
  if (!name)
    return std::unexpected(name.error());

  bool is_array = false;
  auto array_decl = parse_array_def(get_loc());
  if (array_decl)
    is_array = true;

  auto var_type = get_type();
  if (!var_type)
    return std::unexpected(ParseError::UnexpectedToken);

  if (is_array)
    return make_var_decl_node(loc, vis_mod, ownership, name->lexeme,
                              std::move(*array_decl), *var_type);
  return make_var_decl_node(loc, vis_mod, ownership, name->lexeme, nullptr,
                            *var_type);
}

/**
 * @brief parse block body
 *
 * @return vector of statement AST nodes
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_block(SourceLocation loc) {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  std::vector<std::unique_ptr<AST>> block;

  while (!consume(TokenType::RBRACE)) {
    auto statement = parse_statement(loc);
    if (!statement)
      return std::unexpected(statement.error());
    block.push_back(std::move(*statement));
    loc = get_loc();
  }

  return block;
}

/**
 * @brief parse statement
 * determines the statement type and calls appropriate function
 *
 * @return statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_statement(SourceLocation loc) {
  if (consume(TokenType::IF))
    return parse_if_statement(loc);
  if (consume(TokenType::WHILE))
    return parse_while_statement(loc);
  if (consume(TokenType::DO))
    return parse_do_while_statement(loc);
  if (consume(TokenType::FOR))
    return parse_for_statement(loc);
  if (consume(TokenType::LOOP))
    return parse_loop_statement(loc);
  if (consume(TokenType::ASM))
    return parse_assembly_statement(loc);
  if (consume(TokenType::MATCH))
    return parse_match_statement(loc);
  return parse_simple_statement(loc);
}

/**
 * @brief parse if statement
 *
 * @astfields
 * condition (AST):     an expression that evaluates to True or False
 * block (vector<AST>): body of the if statement
 *
 * @return if statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_if_statement(SourceLocation loc) {
  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto condition = parse_expression(get_loc());
  if (!condition)
    return std::unexpected(condition.error());

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  return make_if_node(loc, std::move(*condition), std::move(*block));
}

/**
 * @brief parse while statement
 *
 * @astfields
 * do_while (boolean):          determines if loop is a do-while or normal while
 * loop_condition (AST):        expression that evaluates to True or False
 * block (vector<AST>):         body of the while statement
 *
 * @return while statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_while_statement(SourceLocation loc) {
  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto condition = parse_expression(get_loc());
  if (!condition)
    return std::unexpected(condition.error());

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  return make_while_node(loc, false, std::move(*condition), std::move(*block));
}

/**
 * @brief parse do while statement
 *
 * @astfields
 * do_while (boolean):      determines if loop is do-while. set to true
 * block (vector<AST>):     body of the do while loop
 * loop_condition (AST):    expression that evaluates to True or False
 *
 * @return while statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_do_while_statement(SourceLocation loc) {
  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  if (auto keyword = consume(TokenType::WHILE); !keyword)
    return std::unexpected(keyword.error());

  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto expr = parse_expression(get_loc());
  if (!expr)
    return std::unexpected(expr.error());

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  return make_while_node(loc, true, std::move(*expr), std::move(*block));
}

/**
 * @brief parse for loop
 *
 * @astfields
 * loop_condition (AST):  either ranged or foreach AST node
 * block (vector<AST>):   body of the for loop
 *
 * @return for loop AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_for_statement(SourceLocation loc) {
  if (auto paren = consume(TokenType::LPAREN); !paren)
    return std::unexpected(paren.error());

  auto condition = parse_ranged(get_loc());
  if (!condition) {
    condition = parse_foreach(get_loc());
    if (!condition)
      return std::unexpected(condition.error());
  }

  if (auto paren = consume(TokenType::RPAREN); !paren)
    return std::unexpected(paren.error());

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());

  return make_for_node(loc, std::move(*condition), std::move(*block));
}

/**
 * @brief parse range for loop condition
 *
 * @astfields
 * inclusive (boolean):   if the max expression should be included then true
 * max_exp (AST):         expression that evaluates to an integer
 *
 * @return ranged AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_ranged(SourceLocation loc) {
  auto var = parse_variable_definition(loc);
  if (!var)
    return std::unexpected(var.error());

  auto min_expr = parse_expression(get_loc());
  if (!min_expr)
    return std::unexpected(min_expr.error());

  auto range_type = consume(TokenType::RANGE);

  bool inclusive;
  if (!range_type) {
    range_type = consume(TokenType::RANGE_INCLUSIVE);
    if (!range_type)
      return std::unexpected(range_type.error());
    inclusive = true;
  } else {
    inclusive = false;
  }

  auto max_expr = parse_expression(get_loc());
  if (!max_expr)
    return std::unexpected(max_expr.error());

  return make_ranged_node(loc, inclusive, std::move(*max_expr));
}

/**
 * @brief parse for each
 *
 * @astfields
 * var_decl (AST):      variable declaration used in each iteration
 * mut (AST):           mutable being iterated over
 *
 * @return foreach AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_foreach(SourceLocation loc) {
  auto for_each = std::make_unique<ForEach>(peek().loc);

  auto var = parse_variable_declaration(loc);
  if (!var)
    return std::unexpected(var.error());
  for_each->var_decl = std::move(*var);

  if (auto colon = consume(TokenType::COLON); !colon)
    return std::unexpected(colon.error());

  auto mut = parse_mutable(get_loc());
  if (!mut)
    return std::unexpected(mut.error());
  for_each->mut = std::move(*mut);

  return for_each;
}

/**
 * @brief parse loop statement
 *
 * @astfields
 * block (vector<AST>): body of loop statement
 *
 * @return loop AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_loop_statement(SourceLocation loc) {
  if (auto keyword = consume(TokenType::LOOP); !keyword)
    return std::unexpected(keyword.error());

  auto loop = std::make_unique<LoopStmt>(peek().loc);

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());
  loop->block = std::move(*block);

  return loop;
}

/**
 * @brief parse assembly statement
 *
 * @astfields
 * body (vector<AST>): body of assembly statement
 *
 * @return assembly statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_assembly_statement(SourceLocation loc) {
  if (auto keyword = consume(TokenType::ASM); !keyword)
    return std::unexpected(keyword.error());

  auto asm_stmt = std::make_unique<AsmStmt>(peek().loc);

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());
  asm_stmt->block = std::move(*block);

  return asm_stmt;
}

/**
 * @brief parse match statement
 *
 * @astfields
 * mut (AST):             mutable being matched against
 * block (vector<AST>):   match body
 *
 * @return match statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_match_statement(SourceLocation loc) {
  if (auto keyword = consume(TokenType::MATCH); !keyword)
    return std::unexpected(keyword.error());

  auto match = std::make_unique<MatchStmt>(peek().loc);

  auto mut = parse_mutable(get_loc());
  if (!mut)
    return std::unexpected(mut.error());
  match->mut = std::move(*mut);

  auto block = parse_match_block(get_loc());
  if (!block)
    return std::unexpected(block.error());
  match->block = std::move(*block);

  return match;
}

/**
 * @brief parse match block
 *
 * @return vector of match option AST nodes
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_match_block(SourceLocation loc) {
  if (auto brace = consume(TokenType::LBRACE); !brace)
    return std::unexpected(brace.error());

  auto options = parse_match_options(get_loc());
  if (!options)
    return std::unexpected(options.error());

  return options;
}

/**
 * @brief parse match options
 *
 * @return vector of match option AST nodes
 */
std::expected<std::vector<std::unique_ptr<AST>>, ParseError>
Parser::parse_match_options(SourceLocation loc) {
  std::vector<std::unique_ptr<AST>> options;

  do {
    auto option = parse_match_option(loc);
    if (!option)
      return std::unexpected(option.error());
    options.push_back(std::move(*option));
    loc = get_loc();
  } while (!consume(TokenType::RBRACE));

  return options;
}

/**
 * @brief parse match option
 *
 * @astfields
 * comp (AST):            the expression being matched
 * block (vector<AST>):   body of the match option
 *
 * @return match option AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_match_option(SourceLocation loc) {
  auto option = std::make_unique<MatchOption>(peek().loc);

  auto val = parse_expression(get_loc());
  if (!val)
    return std::unexpected(val.error());
  option->comp = std::move(*val);

  if (auto arrow = consume(TokenType::MATCH_ARROW); !arrow)
    return std::unexpected(arrow.error());

  auto block = parse_block(get_loc());
  if (!block)
    return std::unexpected(block.error());
  option->block = std::move(*block);

  return option;
}

/**
 * @brief parse simple statement
 * a simple statement does not have a body
 *
 * @return simple statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_simple_statement(SourceLocation loc) {
  std::unique_ptr<AST> decl;
  switch (peek().type) {
  case TokenType::CONST:
  case TokenType::REF:
  case TokenType::SHARED:
  case TokenType::OWNED:
    decl = std::move(*parse_variable_definition(loc));
    break;
  case TokenType::IDENTIFIER:
    if (look_ahead().type == TokenType::COLON)
      decl = std::move(*parse_variable_definition(loc));
    else {
      switch (look_ahead().type) {
      case TokenType::ASSIGN:
      case TokenType::PLUS_EQUALS:
      case TokenType::MINUS_EQUALS:
      case TokenType::MULT_EQUALS:
      case TokenType::DIVIDE_EQUALS:
        decl = std::move(*parse_assignment(loc));
        break;
      default:
        break;
      }
    }
    break;
  case TokenType::RETURN:
    decl = std::move(*parse_return_statement(loc));
    break;
  case TokenType::CONTINUE:
    decl = std::make_unique<Continue>(peek().loc);
    if (auto keyword = consume(TokenType::CONTINUE); !keyword)
      return std::unexpected(keyword.error());
    break;
  case TokenType::BREAK:
    decl = std::move(*parse_break_statement(loc));
    break;
  default:
    decl = std::move(*parse_expression(loc));
    break;
  }

  if (auto semicolon = consume(TokenType::SEMICOLON); !semicolon)
    return std::unexpected(semicolon.error());

  return decl;
}

/**
 * @brief parse assignment
 *
 * @astfields
 * op (AssignOp):     type of assignment
 * mut (AST):         mutable being assigned TokenType
 * expression (AST):  expression being assigned
 *
 * @return assignment AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_assignment(SourceLocation loc) {
  auto assign = std::make_unique<Assignment>(peek().loc);

  auto mut = parse_mutable(get_loc());
  if (!mut)
    return std::unexpected(mut.error());
  assign->mut = std::move(*mut);

  auto op = get_assign_op();
  if (!op)
    return std::unexpected(op.error());
  assign->op = *op;

  auto expr = parse_expression(get_loc());
  if (!expr)
    return std::unexpected(expr.error());
  assign->expression = std::move(*expr);

  return assign;
}

/**
 * @brief parse return statement
 *
 * @astfields
 * return_value (AST): value to be returned as an expression AST node
 *
 * @return return statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_return_statement(SourceLocation loc) {
  if (auto keyword = consume(TokenType::RETURN); !keyword)
    return std::unexpected(keyword.error());

  auto ret_stmt = std::make_unique<Return>(peek().loc);

  auto ret_value = parse_expression(get_loc());
  if (ret_value)
    ret_stmt->return_value = std::move(*ret_value);

  return ret_stmt;
}

/**
 * @brief parse break statement
 * Break statements in drift allow a follow-up expression for improved error
 * handling. When a value is provided, it's shared outside of the broken loop
 *
 * @astfields
 * break_value value to be carried outside loop
 *
 * @return break statement AST node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_break_statement(SourceLocation loc) {
  if (auto keyword = consume(TokenType::BREAK); !keyword)
    return std::unexpected(keyword.error());

  auto break_stmt = std::make_unique<Break>(peek().loc);

  auto break_value = parse_expression(get_loc());
  if (break_value)
    break_stmt->break_value = std::move(*break_value);

  return break_stmt;
}

/**
 * @brief parse expression
 * Here begins the order of operations
 *
 * @return AST node representing the head of the exression. Could only have one
 * node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_expression(SourceLocation loc) {
  auto left = parse_or_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_compare_op();
  while (op) {
    auto right = parse_or_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_compare_op();
  }

  return left;
}

/**
 * @brief parse or expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_or_expression(SourceLocation loc) {
  auto left = parse_and_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::OR);
  while (op) {
    auto right = parse_and_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::OR);
  }

  return left;
}

/**
 * @brief parse and expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_and_expression(SourceLocation loc) {
  auto left = parse_bitor_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::AND);
  while (op) {
    auto right = parse_bitor_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::AND);
  }

  return left;
}

/**
 * @brief parse bitwise or expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_bitor_expression(SourceLocation loc) {
  auto left = parse_bitx_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::BIT_OR);
  while (op) {
    auto right = parse_bitx_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::BIT_OR);
  }

  return left;
}

/**
 * @brief parse bitwise xor expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_bitx_expression(SourceLocation loc) {
  auto left = parse_bitand_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::BIT_XOR);
  while (op) {
    auto right = parse_bitand_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::BIT_XOR);
  }

  return left;
}

/**
 * @brief parse bitwise and expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_bitand_expression(SourceLocation loc) {
  auto left = parse_bitshift_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::BIT_AND);
  while (op) {
    auto right = parse_bitshift_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::BIT_AND);
  }

  return left;
}

/**
 * @brief parse bit shift expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_bitshift_expression(SourceLocation loc) {
  auto left = parse_sum_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::LBIT_SHIFT);
  if (!op)
    op = get_binary_op(TokenType::RBIT_SHIFT);
  while (op) {
    auto right = parse_sum_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::LBIT_SHIFT);
    if (!op)
      op = get_binary_op(TokenType::RBIT_SHIFT);
  }

  return left;
}

/**
 * @brief parse sum expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_sum_expression(SourceLocation loc) {
  auto left = parse_mult_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::PLUS);
  if (!op)
    op = get_binary_op(TokenType::MINUS);
  while (op) {
    auto right = parse_mult_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::PLUS);
    if (!op)
      get_binary_op(TokenType::MINUS);
  }

  return left;
}

/**
 * @brief parse mult expression
 *
 * @astfields
 * left (AST):    expression from the left side of the binary operation. can be
 *                alone
 * op (BinaryOp): Operator for the binary expression. optional
 * left (AST):    expression from the right side of the binary operation. must
 *                exist if operator exists
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_mult_expression(SourceLocation loc) {
  auto left = parse_unary_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::STAR);
  if (!op)
    op = get_binary_op(TokenType::SLASH);
  if (!op)
    op = get_binary_op(TokenType::MODULO);
  while (op) {
    auto right = parse_unary_expression(get_loc());
    if (!right)
      return std::unexpected(right.error());
    left = make_binary_node(std::move(*left), std::move(*right), *op);
    op = get_binary_op(TokenType::STAR);
    if (!op)
      op = get_binary_op(TokenType::SLASH);
    if (!op)
      op = get_binary_op(TokenType::MODULO);
  }

  return left;
}

/**
 * @brief parse unary expression
 *
 * @astfields
 * operand (AST):     expression
 * op (UnaryOp):      Operator for the unary expression. optional
 * prefix (boolean):  determines if post or prefix unary expression
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_unary_expression(SourceLocation loc) {
  auto op = get_pre_unary_op();
  auto operand = parse_post_unary_expression(get_loc());
  if (!op)
    return operand;

  operand = make_unary_node(std::move(*operand), *op, true);
  return operand;
}

/**
 * @brief parse post unary expression
 *
 * @astfields
 * operand (AST):     expression
 * op (UnaryOp):      operator for the unary expression. optional
 * prefix (boolean):  determins if post or prefix unary expression
 *
 * @return AST expression node
 */
std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_post_unary_expression(SourceLocation loc) {
  auto operand = parse_power_expression(loc);
  auto op = get_post_unary_op();
  if (!op)
    return operand;

  operand = make_unary_node(std::move(*operand), *op, false);
  return operand;
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_power_expression(SourceLocation loc) {
  auto left = parse_paren_expression(loc);
  if (!left)
    return std::unexpected(left.error());
  auto op = get_binary_op(TokenType::POWER);
  if (!op)
    return left;
  auto right = parse_power_expression(get_loc());
  if (!right)
    return std::unexpected(right.error());

  return make_binary_node(std::move(*left), std::move(*right), *op);
}

std::expected<std::unique_ptr<AST>, ParseError>
Parser::parse_paren_expression(SourceLocation loc) {
  if (consume(TokenType::LPAREN)) {
    auto expression = parse_expression(get_loc());
    if (auto paren = consume(TokenType::RPAREN); !paren)
      return std::unexpected(paren.error());
    return expression;
  }

  auto expression = parse_mutable(get_loc());
  if (!expression)
    expression = parse_immutable(get_loc());
  if (!expression)
    expression = parse_enum_construction(get_loc());
  if (!expression)
    expression = parse_function_call(get_loc());
  if (!expression)
    return std::unexpected(expression.error());

  return expression;
}
