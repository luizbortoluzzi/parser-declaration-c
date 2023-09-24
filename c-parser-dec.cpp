#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define false 0
#define true 1

#define TK_Id 1
#define TK_Void 2                  // void
#define TK_Int 3                   // int
#define TK_Float 4                 // float
#define TK_Comma 5                 // ,
#define TK_Colon 6                 // :
#define TK_Open_Round_Bracket 7    // (
#define TK_Close_Round_Bracket 8   // )
#define TK_Assignment 9            // =
#define TK_Semicolon 10            // ;
#define TK_Open_Curly_Bracket 11   // {
#define TK_Close_Curly_Bracket 12  // }
#define TK_Sum 13                  // +
#define TK_Increment 14            // ++
#define TK_Product 15              // *
#define TK_Char 16                 // char
#define TK_Subtraction 17          // -
#define TK_Open_Square_Bracket 18  // [
#define TK_Close_Square_Bracket 19 // ]
#define TK_Inc_Assignment 20       // +=
#define TK_Double 21               // double
#define TK_Int_Const 22            // Identify integer constant
#define TK_EOF 23                  // End Of File - Identifier
#define TK_Struct 24               // struct
#define TK_If 25                   // if
#define TK_Else 26                 // else
#define TK_Break 27                // break
#define TK_Continue 28             // continue
#define TK_Do 29                   // do
#define TK_For 30                  // for
#define TK_While 31                // while
#define TK_Decrement 32            // --
#define TK_Dec_Assignment 33       // -=
#define TK_Modulus 34              // %
#define TK_Division 35             // /
#define TK_Equal 36                // ==
#define TK_Switch 37               // switch
#define TK_Case 38                 // case
#define TK_Default 39              // default
#define TK_Less 40                 // <
#define TK_Less_Equal 41           // <=
#define TK_Greater 42              // >
#define TK_Greater_Equal 43        // >=
#define TK_Not 44                  // !
#define TK_Not_Equal 45            // !=
#define TK_Struct_Pointer 46       // ->
#define TK_Struct_Atribute 47      // .
#define TK_And 48                  // &&
#define TK_Bitwise_And 49          // &
#define TK_Or 50                   // ||
#define TK_Bitwise_Or 51           // |
#define TK_Shift_Left 52           // <<
#define TK_Shift_Right 53          // >>
#define TK_Bitwise_Exclusive_Or 54 // ^
#define TK_Ones_Complement 55      // ~
#define TK_Array 56                // array

#define MAX_SYMBOLS 100

char c;
char lex[20];
int line = 1;
int column = 1;
int pos = 0;
int token;
FILE *file;
FILE *output;
int current_type = -1;  // Invalid by default

struct reserved_keyword
{
    char word[20];
    int token;
};

struct symbol_entry
{
    char name[20];
    int token;
    int type;
    // Other relevant information (type, size, etc.)
};

struct symbol_entry symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

struct reserved_keyword words_list[] = {{"void", TK_Void},
                                        {"int", TK_Int},
                                        {"float", TK_Float},
                                        {"char", TK_Char},
                                        {"double", TK_Double},
                                        {"struct", TK_Struct},
                                        {"if", TK_If},
                                        {"else", TK_Else},
                                        {"break", TK_Break},
                                        {"continue", TK_Continue},
                                        {"do", TK_Do},
                                        {"while", TK_While},
                                        {"for", TK_For},
                                        {"switch", TK_Switch},
                                        {"case", TK_Case},
                                        {"default", TK_Default},
                                        {"end_of_table", TK_Id}};

void add_symbol(const char *name, int token, int type)
{
    if (symbol_count < MAX_SYMBOLS)
    {
        strcpy(symbol_table[symbol_count].name, name);
        symbol_table[symbol_count].token = token;
        symbol_table[symbol_count].type = type;
        symbol_count++;
    }
    else
    {
        printf("Error: Symbol table is full\n");
        fprintf(output, "Error: Symbol table is full\n");
    }
}

int find_symbol(const char *name)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(name, symbol_table[i].name) == 0)
        {
            return symbol_table[i].token;
        }
    }
    return -1; // Symbol not found
}

int reserved_keyword(char lex[])
{
    int postab = 0;
    while (strcmp("end_of_table", words_list[postab].word) != 0)
    {
        if (strcmp(lex, words_list[postab].word) == 0)
            return words_list[postab].token;
        postab++;
    }
    return TK_Id;
}

void next_char()
{
    if (feof(file))
    {
        c = -1;
        return;
    }
    fread(&c, 1, 1, file);
    column++;
}

void get_token()
{
    int state = 0;
    int end = 0;
    int posl = 0;
    while (!end)
    {
        lex[posl++] = c;
        switch (state)
        {
        case 0:
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
            {
                next_char();
                state = 1;
                break;
            }
            if (c >= '0' && c <= '9')
            {
                while (c >= '0' && c <= '9')
                    next_char();
                lex[posl] = '\0';
                token = TK_Int_Const;
                return;
            }
            if (c == '=')
            {
                next_char();
                if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Equal;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Assignment;
                    return;
                }
            }
            if (c == '&')
            {
                next_char();
                if (c == '&')
                {
                    lex[posl++] = '&';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_And;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Bitwise_And;
                    return;
                }
            }
            if (c == '|')
            {
                next_char();
                if (c == '|')
                {
                    lex[posl++] = '|';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Or;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Bitwise_Or;
                    return;
                }
            }
            if (c == '<')
            {
                next_char();
                if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Less_Equal;
                    return;
                }
                else if (c == '<')
                {
                    lex[posl++] = '<';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Shift_Left;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Less;
                    return;
                }
            }
            if (c == '>')
            {
                next_char();
                if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Greater_Equal;
                    return;
                }
                else if (c == '>')
                {
                    lex[posl++] = '>';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Shift_Right;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Greater;
                    return;
                }
            }
            if (c == '!')
            {
                next_char();
                if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Not_Equal;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Not;
                    return;
                }
            }
            if (c == '+')
            {
                next_char();
                if (c == '+')
                {
                    lex[posl++] = '+';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Increment;
                    return;
                }
                else if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Inc_Assignment;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    token = TK_Sum;
                    return;
                }
            }
            if (c == '^')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Bitwise_Exclusive_Or;
                return;
            }
            if (c == '~')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Ones_Complement;
                return;
            }
            if (c == '*')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Product;
                return;
            }
            if (c == '.')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Struct_Atribute;
                return;
            }
            if (c == '[')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Open_Square_Bracket;
                return;
            }
            if (c == ']')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Close_Square_Bracket;
                return;
            }
            if (c == '(')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Open_Round_Bracket;
                return;
            }
            if (c == ')')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Close_Round_Bracket;
                return;
            }
            if (c == '{')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Open_Curly_Bracket;
                return;
            }
            if (c == '}')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Close_Curly_Bracket;
                return;
            }
            if (c == ',')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Comma;
                return;
            }
            if (c == ';')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Semicolon;
                return;
            }
            if (c == ':')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Colon;
                return;
            }
            if (c == '%')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Modulus;
                return;
            }
            if (c == '/')
            {
                lex[posl] = '\0';
                next_char();
                token = TK_Division;
                return;
            }
            if (c == '-')
            {
                next_char();
                if (c == '-')
                {
                    lex[posl++] = '-';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Decrement;
                    return;
                }
                else if (c == '=')
                {
                    lex[posl++] = '=';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Dec_Assignment;
                    return;
                }
                else if (c == '>')
                {
                    lex[posl++] = '>';
                    lex[posl] = '\0';
                    next_char();
                    token = TK_Struct_Pointer;
                    return;
                }
                else
                {
                    lex[posl] = '\0';
                    token = TK_Subtraction;
                    return;
                }
            }
            if (c == -1)
            {
                lex[posl] = '\0';
                next_char();
                token = TK_EOF;
                return;
            }
            if (c == ' ' || c == '\t' || c == '\r')
            {
                next_char();
                posl--;
                break;
            }
            if (c == '\n')
            {
                column = 0;
                line++;
                next_char();
                posl--;
                break;
            }

            if (c == '\0')
            {
                token = -1;
                return;
            }
            printf("Lexical Parser Error: Character %c found in (%d)", c, c);
            while (c != '\n')
                next_char();
            break;
        case 1:
            if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9')
            {
                next_char();
                break;
            }
            lex[--posl] = '\0';
            token = reserved_keyword(lex);
            return;
        case 2:
            if (c >= '0' && c <= '9')
            {
                lex[pos++] = c;
                next_char();
                break;
            }
            else
            {
                state = 0;
                lex[pos] = '\0';
                token = TK_Int_Const;
                return;
            }
        }
    }
}

/**
 * Recognizes and processes type declarations from the input source code.
 * The function handles types such as int, float, char, double, and struct.
 * The recognized type is stored globally for later usage in the program.
 * 
 * Grammar: type -> "int" | "float" | "char" | "double" | "struct"
 * 
 */
void type()
{
    if (token == TK_Int || token == TK_Float || token == TK_Char || token == TK_Double)
    {
        current_type = token;  // Update the current type
        printf("Type: %s\n", lex);
        fprintf(output, "Type: %s\n", lex);
        get_token(); // Get the next token
    }
    else if (token == TK_Struct)
    {
        current_type = token;  // Update the current type
        printf("Type: struct\n");
        fprintf(output, "Type: struct\n");
        get_token(); // Get the next token
    }
    else
    {
        printf("Line: %d Column: %d - Syntax error: Expecting a type\n", line, column);
        fprintf(output, "Syntax error: Expecting a type\n");
    }
}

/**
 * Parses a list of identifiers separated by commas.
 * Checks if the identifier already exists in the symbol table.
 * If it does not exist, the identifier is added to the symbol table.
 *  
 * Grammar: <identifier_list> -> id | id , <identifier_list>
 * 
 */
void identifier_list()
{
    if (token == TK_Id)
    {
        int existing_token = find_symbol(lex);
        if (existing_token != -1)
        {
            printf("Line: %d Column: %d - Error: Identifier %s already declared\n", line, column, lex);
            fprintf(output, "Error: Identifier %s already declared\n", lex);
        }
        else
        {
            add_symbol(lex, token, current_type);
        }

        get_token(); // Get the next token
        if (token == TK_Comma)
        {
            get_token();       // Get the next token
            identifier_list(); // Recursive call for multiple identifiers
        }
    }
    else
    {
        printf("Line: %d Column: %d - Syntax error: Expecting an identifier\n", line, column);
        fprintf(output, "Syntax error: Expecting an identifier\n");
    }
}


/**
 * Parses a struct declaration in the input source code.
 * Handles fields within the struct and ensures the correct ending of the struct declaration.
 * If a valid identifier follows the struct declaration, it is parsed and treated as a struct variable.
 * 
 * Grammar: <struct_declaration> -> { <type> <identifier_list> ; } <identifier_list> ;
 */
void struct_declaration()
{
    get_token(); // Consume the {

    while (token != TK_Close_Curly_Bracket && token != TK_EOF)
    {
        // Parse struct fields and add them to the symbol table
        type();
        identifier_list();
        if (token == TK_Semicolon)
        {
            printf("Line: %d Column: %d - Valid struct field declaration\n", line, column);
            fprintf(output, "Valid struct field declaration\n");
            get_token(); // Get the next token
        }
        else
        {
            printf("Line: %d Column: %d - Syntax error: Expecting ;\n", line, column);
            fprintf(output, "Syntax error: Expecting ;\n");
            // Handle the error or recovery mechanism
        }
    }

    if (token == TK_Close_Curly_Bracket)
    {
        printf("Line: %d Column: %d - End of struct declaration\n", line, column);
        fprintf(output, "End of struct declaration\n");
        get_token(); // Consume the }

        // Check if there's an identifier after the struct definition.
        if (token == TK_Id)
        {
            identifier_list();

            if (token != TK_Semicolon)
            {
                printf("Line: %d Column: %d - Syntax error: Expecting ;\n", line, column);
                fprintf(output, "Syntax error: Expecting ;\n");
                return;
            }
            else
            {
                printf("Line: %d Column: %d - Valid struct variable declaration\n", line, column);
                fprintf(output, "Valid struct variable declaration\n");
                get_token(); // Consume the ;
            }
        }
    }
    else
    {
        printf("Line: %d Column: %d - Syntax error: Expecting }\n", line, column);
        fprintf(output, "Syntax error: Expecting }\n");
    }
}


/**
 * Parses an array declaration from the input source code.
 * Ensures the correct format and size specifications for multi-dimensional arrays.
 * 
 * Grammar: <array_declaration> -> [const] ; | [const] <array_declaration> ;
 * 
 */
void array_declaration()
{

    get_token();

    if (token != TK_Int_Const)
    {
        printf("Line: %d Column: %d - Syntax error: Expecting a number for array size\n", line, column);
        fprintf(output, "Syntax error: Expecting a number for array size\n");
        return;
    }

    get_token();

    if (token != TK_Close_Square_Bracket)
    {
        printf("Line: %d Column: %d - Syntax error: Expecting ']'\n", line, column);
        fprintf(output, "Syntax error: Expecting ']'\n");
        return;
    }

    get_token(); 


    while (token == TK_Open_Square_Bracket)
    {
        get_token();

        if (token != TK_Int_Const)
        {
            printf("Line: %d Column: %d - Syntax error: Expecting a number for array size\n", line, column);
            fprintf(output, "Syntax error: Expecting a number for array size\n");
            return;
        }

        get_token();

        if (token != TK_Close_Square_Bracket)
        {
            printf("Line: %d Column: %d - Syntax error: Expecting ']'\n", line, column);
            fprintf(output, "Syntax error: Expecting ']'\n");
            return;
        }

        get_token();
    }

    if (token != TK_Semicolon)
    {
        printf("Line: %d Column: %d - Syntax error: Expecting ';'\n", line, column);
        fprintf(output, "Syntax error: Expecting ';'\n");
    }
    else
    {
        printf("Line: %d Column: %d - Valid array declaration\n", line, column);
        fprintf(output, "Valid array declaration\n");
        get_token();
    }
}

/**
 * Recognizes and processes declarations in the input source code.
 * Handles type, struct, and array declarations by invoking appropriate functions.
 * 
 * Grammar: declaration -> type identifier_list ; | type identifier_list struct_declaration | type identifier_list array_declaration  
 * 
 */
void declaration()
{
    type();
    identifier_list();

    while (token == TK_Comma)
    {
        get_token();
        identifier_list();
    }

    if (token == TK_Semicolon)
    {
        printf("Line: %d Column: %d - Valid declaration\n", line, column);
        fprintf(output, "Valid declaration\n");
        get_token();
    }
    else if (token == TK_Open_Curly_Bracket)
    {
        printf("Line: %d Column: %d - Valid struct declaration\n", line, column);
        fprintf(output, "Valid struct declaration\n");
        struct_declaration();
    }
    else if (token == TK_Open_Square_Bracket)
    {
        array_declaration();
    }
    else
    {
        printf("Line: %d Column: %d - Syntax error: Expecting ; or {\n", line, column);
        fprintf(output, "Syntax error: Expecting ; or {\n");
    }
}

/**
 * Main entry point of the program.
 * Opens the input source code file and an output file.
 * Processes declarations in the source code and prints recognized tokens and errors to the output.
 * Finally, the symbol table is displayed to the console.
 */
int main()
{
    file = fopen("program.c", "rb");
    output = fopen("output.txt", "wb");
    if (!file)
    {
        printf("Error while opening file.\n");
        return 0;
    }
    if (!output)
    {
        printf("Error while opening output file.\n");
        return 0;
    }
    next_char(); // Gets the first character of file
    get_token(); // Read the first token
    while (token != TK_EOF)
    {
        printf("%d %s\n", token, lex);
        printf("Line: %d Column %d\n", line, column);
        fprintf(output, "%s \n Line: %d  Column: %d \n", lex, line, column);
        declaration();
    }

    printf("Symbol Table:\n");
    for (int i = 0; i < symbol_count; i++)
    {
        printf("%s - Token: %d - Type: %d\n", symbol_table[i].name, symbol_table[i].token, symbol_table[i].type);
    }

    fclose(file);
    fclose(output);
    printf("Recorded Data Successfuly!!\n");
}