#ifndef BAYES_LEARNING_H
#define BAYES_LEARNING_H
#include "startup_code.h"

void initialize_probability(network *n, DATABASE db);
void m_step(network *n, DATABASE db);
DATABASE modify_database(DATABASE db, network &n);
float get_score(network n,network acn);
void pipeline(network *n, DATABASE d,network acn,time_t start_time);


#endif
