/*
** RPN - Reverse Polish Notation calculator
**
** Usage: ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
**
** Uses a dynamically-growing stack (the required "container") to
** evaluate the expression token by token.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------ */
/*  Stack container                                                    */
/* ------------------------------------------------------------------ */

typedef struct s_stack
{
    long    *data;
    int     top;       /* number of elements currently stored */
    int     capacity;
}   t_stack;

static void stack_init(t_stack *s)
{
    s->capacity = 16;
    s->top = 0;
    s->data = malloc(sizeof(long) * s->capacity);
    if (!s->data)
    {
        fprintf(stderr, "Error\n");
        exit(EXIT_FAILURE);
    }
}

static void stack_push(t_stack *s, long value)
{
    if (s->top >= s->capacity)
    {
        s->capacity *= 2;
        s->data = realloc(s->data, sizeof(long) * s->capacity);
        if (!s->data)
        {
            fprintf(stderr, "Error\n");
            exit(EXIT_FAILURE);
        }
    }
    s->data[s->top] = value;
    s->top++;
}

/* returns 1 on success, 0 if the stack was empty (error condition) */
static int stack_pop(t_stack *s, long *value)
{
    if (s->top <= 0)
        return (0);
    s->top--;
    *value = s->data[s->top];
    return (1);
}

static void stack_free(t_stack *s)
{
    free(s->data);
    s->data = NULL;
    s->top = 0;
    s->capacity = 0;
}

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

static int is_operator(const char *token)
{
    return (token[1] == '\0' &&
            (token[0] == '+' || token[0] == '-' ||
             token[0] == '*' || token[0] == '/'));
}

/* A valid number token per subject rules: a single digit (0-9). */
static int is_number(const char *token)
{
    return (token[1] == '\0' && isdigit((unsigned char)token[0]));
}

static int apply_operator(t_stack *s, char op)
{
    long    a;
    long    b;
    long    res;

    if (!stack_pop(s, &b) || !stack_pop(s, &a))
        return (0); /* not enough operands */
    if (op == '+')
        res = a + b;
    else if (op == '-')
        res = a - b;
    else if (op == '*')
        res = a * b;
    else /* '/' */
    {
        if (b == 0)
            return (0); /* division by zero */
        res = a / b;
    }
    stack_push(s, res);
    return (1);
}

/* ------------------------------------------------------------------ */
/*  Main                                                                */
/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    t_stack s;
    char    *token;
    char    *expr;
    long    result;
    int     ok;

    if (argc != 2 || argv[1][0] == '\0')
    {
        fprintf(stderr, "Error\n");
        return (EXIT_FAILURE);
    }

    expr = strdup(argv[1]);
    if (!expr)
    {
        fprintf(stderr, "Error\n");
        return (EXIT_FAILURE);
    }

    stack_init(&s);
    ok = 1;

    token = strtok(expr, " \t");
    while (token != NULL && ok)
    {
        if (is_number(token))
            stack_push(&s, token[0] - '0');
        else if (is_operator(token))
        {
            if (!apply_operator(&s, token[0]))
                ok = 0;
        }
        else
            ok = 0; /* unknown token: letters, brackets, multi-digit, etc. */
        token = strtok(NULL, " \t");
    }

    /* At the end, exactly one value must remain on the stack */
    if (ok && s.top == 1)
    {
        result = s.data[0];
        printf("%ld\n", result);
        stack_free(&s);
        free(expr);
        return (EXIT_SUCCESS);
    }

    fprintf(stderr, "Error\n");
    stack_free(&s);
    free(expr);
    return (EXIT_FAILURE);
}
