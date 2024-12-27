#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "domino.h"
#include "domino_line.h"

struct _s_line {
    domino tile;
    domino_line next;
    domino_line previous;
};

domino_line line_new(domino first) {
    domino_line new = malloc(sizeof(struct _s_line));
    if (new != NULL) {
        new->tile = first;
        new->next = NULL;
        new->previous = NULL;
    } else {
        printf("Memory Error\n");
        exit(EXIT_FAILURE);
    }

    return new;
}

domino_line line_add(domino_line line, domino t) {
    assert(line != NULL && t != NULL);
    domino_line aux = line_new(t);
    aux->previous = line;
    line->next = aux;
    line = aux;
    return line;
}

unsigned int line_length(domino_line line) {
    assert(line != NULL);
    domino_line aux = line;
    unsigned int res = 0u;
    while (aux != NULL) {
        aux = aux->previous;
        res++;
    }
    return res;
}

bool line_n_correct(domino_line line, unsigned int n) {
    unsigned int size = line_length(line);
    assert(n < size);
    bool res = true;
    domino_line n_tile = line;
    while (size - n - 1 > 0 && n_tile->previous != NULL) {
        n_tile = n_tile->previous;
        n++;
    }
    if (n_tile->previous != NULL) {
        res = res && domino_matches(n_tile->previous->tile, n_tile->tile);
    }
    if (n_tile->next != NULL) {
        res = res && domino_matches(n_tile->tile, n_tile->next->tile);
    }
    return res;
}

int line_total_points(domino_line line) {
    int res = 0;
    domino_line aux = line;
    while (aux != NULL) {
        res = res + domino_up(aux->tile) + domino_down(aux->tile);
        aux = aux->previous;
    }
    return res;
}

domino* line_to_array(domino_line line) {
    assert(line != NULL);
    unsigned int size = line_length(line);
    unsigned int i = 0u;
    domino* array = malloc(size * sizeof(domino));
    domino_line aux = line;
    while (aux != NULL && size > i) {
        array[size - i - 1] = aux->tile;
        aux = aux->previous;
        i++;
    }
    return array;
}

void line_dump(domino_line line) {
    domino* array = line_to_array(line);
    for (unsigned int i = 0u; i < line_length(line); i++) {
        domino_dump(array[i]);
    }
    free(array);
}

domino_line line_destroy(domino_line line) {
    domino_line aux;
    while (line != NULL) {
        aux = line;
        line = line->previous;
        aux->tile = domino_destroy(aux->tile);
        free(aux);
        aux = NULL;
    }
    return line;
}
