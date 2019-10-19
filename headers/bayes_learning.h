#ifndef BAYES_LEARNING_H
#define BAYES_LEARNING_H
#include "startup_code.h"

DATABASE modify_database(DATABASE db, network &n);
void initialize_probability(network* n, DATABASE db);

#endif
