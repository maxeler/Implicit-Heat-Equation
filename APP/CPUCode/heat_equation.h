/*
 * heat_equation.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#ifndef HEAT_EQUATION_H_
#define HEAT_EQUATION_H_

typedef struct dfe_handle dfe_handle_t;

typedef struct params{
    int x;
    int y;
    int z;
    int n_iterations;
    int n_substep;

    float threshold;
    int has_input_file;
    int has_model_file;
    char* model_path;
    char* input_path;

    float alpha_value;

    float* input;
    float* mu;
    float* diag;
    float dt;

    int exit;

    dfe_handle_t *dfe_handle;


} params;

#endif /* HEAT_EQUATION_H_ */

int has_input_file;
