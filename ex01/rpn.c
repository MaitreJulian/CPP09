/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpn.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julian <julian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:01:21 by julian            #+#    #+#             */
/*   Updated: 2026/07/29 16:01:23 by julian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct s_stack
{
    long    *data;
    int     top;
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

static int is_operator(const char *token)
{
    return (token[1] == '\0' &&
            (token[0] == '+' || token[0] == '-' ||
             token[0] == '*' || token[0] == '/'));
}


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
        return (0);
    if (op == '+')
        res = a + b;
    else if (op == '-')
        res = a - b;
    else if (op == '*')
        res = a * b;
    else
    {
        if (b == 0)
            return (0);
        res = a / b;
    }
    stack_push(s, res);
    return (1);
}


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
            ok = 0;
        token = strtok(NULL, " \t");
    }


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
