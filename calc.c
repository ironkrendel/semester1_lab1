#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define HELP_PARAMTER_FORMAT "  %-50s %s\n"

#define DEC_OUT 0
#define HEX_OUT 1

#define FLOAT_TYPE 0
#define INT_TYPE 1

#define CALC_PARAM 0
#define HEX_PARAM 1
#define INT_PARAM 2
#define EXPRESSION_PARAM 3

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Calculator program\n");
        printf("For displaying more information use -h or --help\n");

        return 0;
    }

    int output_format = DEC_OUT;
    int operand_type = FLOAT_TYPE;
    int previous_parameter = -1;

    char* operator = malloc(10);

    float operand1, operand2, result;

    for (int arg = 1;arg < argc;arg++) {
        if (strcmp(argv[arg], "-h") == 0 || strcmp(argv[arg], "--help") == 0) {
            printf("Usage: calc [options]\n");
            printf("Options:\n");

            printf(HELP_PARAMTER_FORMAT, "-h, --help", "Show help");

            printf(HELP_PARAMTER_FORMAT, "-c <expression>, --calc <expression>", "Calculate expression of \"[operand1] [operator] [operand2]\" format.");
            printf(HELP_PARAMTER_FORMAT, "", "Allowed operand types are: integer and floating point for mathematical operations and \"1\" and \"0\" for logical operations.");
            printf(HELP_PARAMTER_FORMAT, "", "Allowed operators are: +, -, *, / and %% for mathematical operations and \"and\" and \"or\" for logical operations.");

            printf(HELP_PARAMTER_FORMAT, "-x, --hex", "Output answer in a hex format.");

            printf(HELP_PARAMTER_FORMAT, "-i, --int", "Specify operands' format as int. By default operands are treated as floating point numbers.");

            return 0;
        }
        else if (strcmp(argv[arg], "-x") == 0 || strcmp(argv[arg], "--hex") == 0) {
            if (previous_parameter == CALC_PARAM) {
                printf("Error in parameter order!\n");
                printf("Use \"calc -h\" or \"calc --help\" for more information.\n");

                return 1;
            }

            output_format = HEX_OUT;

            previous_parameter = HEX_PARAM;
        }
        else if (strcmp(argv[arg], "-i") == 0 || strcmp(argv[arg], "--int") == 0) {
            if (previous_parameter == CALC_PARAM) {
                printf("Error in parameter order!\n");
                printf("Use \"calc -h\" or \"calc --help\" for more information.\n");

                return 1;
            }

            operand_type = INT_TYPE;

            previous_parameter = INT_PARAM;
        }
        else if (strcmp(argv[arg], "-c") == 0 || strcmp(argv[arg], "--calc") == 0) {
            previous_parameter = CALC_PARAM;
        }
        else {
            if (argv[arg][0] == '-' && sizeof(argv[arg]) > 1)
                if (!(argv[arg][1] >= '0' && argv[arg][1] <= '9')) {
                    printf("Unknown parameter!\n");
                    printf("Use \"calc -h\" or \"calc --help\" for more information.\n");

                    return 1;
                } 

            if (previous_parameter != CALC_PARAM) continue;

            previous_parameter = EXPRESSION_PARAM;

            char buffer[100] = "";
            
            int i = 0;
            int j = 0;

            // operand 1
            while ((argv[arg][i] >= '0' && argv[arg][i] <= '9') || argv[arg][i] == '-' || argv[arg][i] == '.') {
                buffer[j] = argv[arg][i];
                i++;j++;
            }
            buffer[j] = '\0';
            operand1 = atof(buffer);

            // skip to operator
            while (argv[arg][i] == ' ') i++;
            
            // operator
            j = 0;
            while (argv[arg][i] != ' ') {
                buffer[j] = argv[arg][i];
                i++;
                j++;
            }
            buffer[j] = '\0';
            strcpy(operator, buffer);

            // skip to operand 2
            while (!((argv[arg][i] >= '0' && argv[arg][i] <= '9') || argv[arg][i] == '-' || argv[arg][i] == '.')) i++;

            // operand 2
            j = 0;
            while ((argv[arg][i] >= '0' && argv[arg][i] <= '9') || argv[arg][i] == '-' || argv[arg][i] == '.') {
                buffer[j] = argv[arg][i];
                i++;j++;
            }
            buffer[j] = '\0';
            operand2 = atof(buffer);
        }
    }

    if (operand_type == INT_TYPE) {
        if (operand1 != (int)operand1 || operand2 != (int)operand2) {
            printf("Error in operands. Operand does not match specified by \"-i\" parameter format.\n");
            printf("Use \"calc -h\" or \"calc --help\" for more information.\n");

            return 1;
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
        result = (int)operand1 && (int)operand2;
    }
    else if (strcmp(operator, "or") == 0) {
        result = (int)operand1 || (int)operand2;
    }
    else {
        printf("Unknown operator \"%s\" !\n", operator);
        printf("Use \"calc -h\" or \"calc --help\" for more information.\n");

        return 1;
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
            if (result >= 0) {
                printf("%x\n", result);
            }
            else {
                printf("-%x\n", result * -1);
            }
        }
    }

    free(operator);

    return 0;
}