//
// Created by Philipp Hofer on 26.03.21.
//

#ifndef GENERATOR_NUMBER_H
#define GENERATOR_NUMBER_H

typedef struct number_type
{
    int block;
    int number;
} number;

number new_number(int block, int number);

#endif //GENERATOR_NUMBER_H
