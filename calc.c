#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <regex.h>

#define HELP_PARAMTER_FORMAT "  %-50s %s\n"

#define DEC_OUT 0
#define HEX_OUT 1

#define FLOAT_TYPE 0
#define INT_TYPE 1

#define CALC_PARAM 0
#define HEX_PARAM 1
#define INT_PARAM 2
#define EXPRESSION_PARAM 3

regex_t operand_pattern;
regex_t operator_pattern;
regex_t unknown_argument_pattern;

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Calculator program\n");
        printf("For displaying more information use -h or --help\n");

        return 0;
    }

    int output_format = DEC_OUT;
    int operand_type = FLOAT_TYPE;
    int previous_argument = -1;

    char* operator;

    float operand1, operand2, result;

    int expression_current_argument = 0;

    // compile regex
    regcomp(&operand_pattern, "^[-]?[0-9]+[.]?[0-9]*$", REG_EXTENDED | REG_NOSUB);
    regcomp(&operator_pattern, "^[-+*/]|and|or$", REG_EXTENDED | REG_NOSUB);
    regcomp(&unknown_argument_pattern, "^[-]+.*$", REG_EXTENDED | REG_NOSUB);

    for (int arg = 1;arg < argc;arg++) {
        if (strcmp(argv[arg], "-h") == 0 || strcmp(argv[arg], "--help") == 0) {
            printf("Usage: calc [options]\n");
            printf("Options:\n");

            printf(HELP_PARAMTER_FORMAT, "-h, --help", "Show help");

            printf(HELP_PARAMTER_FORMAT, "-c <expression>, --calc <expression>", "Calculate expression of [operand1] [operator] [operand2] format, where two operands and operator are composed of several arguments.");
            printf(HELP_PARAMTER_FORMAT, "", "Allowed operand types are: integer and floating point for mathematical operations and \"1\" and \"0\" for logical operations.");
            printf(HELP_PARAMTER_FORMAT, "", "Allowed operators are: +, -, *, / and %% for mathematical operations and \"and\" and \"or\" for logical operations.");

            printf(HELP_PARAMTER_FORMAT, "-x, --hex", "Output answer in a hex format.");

            printf(HELP_PARAMTER_FORMAT, "-i, --int", "Specify operands' format as int. By default operands are treated as floating point numbers.");

            goto exit_sequence;
        }
        else if (strcmp(argv[arg], "-x") == 0 || strcmp(argv[arg], "--hex") == 0) {
            if (previous_argument == CALC_PARAM) {
                printf("Error in parameter order!\n");
                printf("Use -h or --help for more information.\n");

                goto exit_sequence;
            }

            output_format = HEX_OUT;

            previous_argument = HEX_PARAM;
        }
        else if (strcmp(argv[arg], "-i") == 0 || strcmp(argv[arg], "--int") == 0) {
            if (previous_argument == CALC_PARAM) {
                printf("Error in parameter order!\n");
                printf("Use -h or --help for more information.\n");

                goto exit_sequence;
            }

            operand_type = INT_TYPE;

            previous_argument = INT_PARAM;
        }
        else if (strcmp(argv[arg], "-c") == 0 || strcmp(argv[arg], "--calc") == 0) {
            previous_argument = CALC_PARAM;
        }
        else {
            if (previous_argument != CALC_PARAM && previous_argument != EXPRESSION_PARAM) {
                if (regexec(&unknown_argument_pattern, argv[arg], 0, NULL, 0) == 0) {
                    printf("Unknown argument passed.\n");
                    printf("Use -h or --help for more information.\n");
                }
                else 
                printf("Error in argument order. Expression must come after -c (--calc) argument\n");

                goto exit_sequence;
            }

            previous_argument = EXPRESSION_PARAM;

            if (expression_current_argument % 2 == 0) {
                if (regexec(&operand_pattern, argv[arg], 0, NULL, 0) == REG_NOMATCH) {
                    printf("Wrong expression format!\n");
                    printf("Use -h or --help for more information.\n");

                    goto exit_sequence;
                }

                if (expression_current_argument == 0) operand1 = atof(argv[arg]);
                else if (expression_current_argument == 2) operand2 = atof(argv[arg]);
                else {
                    printf("Too much arguments passed after -c (--calc) flag!\n");
                    printf("Use -h or --help for more information.\n");

                    goto exit_sequence;
                }
            }
            else {
                if (regexec(&operator_pattern, argv[arg], 0, NULL, 0) == REG_NOMATCH) {
                    printf("Wrong expression format!\n");
                    printf("Use -h or --help for more information.\n");

                    goto exit_sequence;
                }

                if (expression_current_argument == 1) operator = argv[arg];
                else {
                    printf("Too much arguments passed after -c (--calc) flag!\n");
                    printf("Use -h or --help for more information.\n");

                    goto exit_sequence;
                }
            }

            expression_current_argument++;
        }
    }

    if (operand_type == INT_TYPE) {
        if (operand1 != (int)operand1 || operand2 != (int)operand2) {
            printf("Error in operands. Operand does not match specified by -i parameter format.\n");
            printf("Use -h or --help for more information.\n");

            goto exit_sequence;
        }
        operand1 = (int)operand1;
        operand2 = (int)operand2;
    }

    if (strcmp(operator, "+") == 0) {
        result = operand1 + operand2;
    }
    else if (strcmp(operator, "-") == 0) {
        result = operand1 - operand2;
    }
    else if (strcmp(operator, "*") == 0) {
        result = operand1 * operand2;
    }
    else if (strcmp(operator, "/") == 0) {
        result = operand1 / operand2;
    }
    else if (strcmp(operator, "%") == 0) {
        result = (int)operand1 % (int)operand2;
    }
    else if (strcmp(operator, "and") == 0) {
        if (operand1 != 1 && operand1 != 0 || operand2 != 1 && operand2 != 0) {
            printf("Error in operands. Logic operations are only allow on binary types.\n");
            printf("Use -h or --help for more information.\n");

            goto exit_sequence;
        }
        result = (int)operand1 && (int)operand2;
    }
    else if (strcmp(operator, "or") == 0) {
        result = (int)operand1 || (int)operand2;
    }
    else {
        printf("Unknown operator %s !\n", operator);
        printf("Use -h or --help for more information.\n");
        
        goto exit_sequence;
    }

    if (operand_type == INT_TYPE) {
        if (output_format == DEC_OUT) {
            printf("%d\n", (int)result);
        }
        else {
            if (result >= 0) printf("%x\n", (int)result);
            else printf("-%x\n", (int)result * -1);
        }
    }
    else {
        if (output_format == DEC_OUT) {
            printf("%f\n", result);
        }
        else {
            printf("%a\n", result);
            // if (result >= 0) {
            //     printf("%x\n", (unsigned int)result);
            // }
            // else {
            //     printf("-%x\n", (unsigned int)result * -1);
            // }
        }
    }

    exit_sequence:
    regfree(&operand_pattern);
    regfree(&operator_pattern);

    return 0;
}