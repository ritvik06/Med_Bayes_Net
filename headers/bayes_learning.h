#ifndef BAYES_LEARNING_H
#define BAYES_LEARNING_H
#include "startup_code.h"

void initialize_probability(network n, DATABASE db);
DATABASE modify_database(DATABASE db, network n);

#endif
