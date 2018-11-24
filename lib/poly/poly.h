#ifndef POLY_H
# define POLY_H

void ploy_ins_add(char *data, size_t *i, char *ins, size_t ins_size);
void ploy_ins_add_end(char *data, size_t *i, char *ins, size_t ins_size);

int poly_generate(void **crypt, void **decrypt);
void poly_new_start(void *start);

extern uint32_t ft_rand();

#endif
