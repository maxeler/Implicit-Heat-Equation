/*
 * cpu_compute.h
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include <string.h>
#include <stdio.h>
#include "cpu_compute.h"
#include "memory.h"

void calculate_b(struct params *settings, float* curr,  float* b){

    size_t nx = settings->x;
    size_t ny = settings->y;
    size_t nz = settings->z;

    for(size_t i = 1; i < nz-1; i++)
        for(size_t j = 1; j < ny-1; j++)
            for(size_t k = 1; k < nx-1; k++){

                size_t index = convert_to_1D_index(i, j, k, nz, ny, nx);

                size_t index_xm1 = convert_to_1D_index(i  , j  , k-1, nz, ny, nx);
                size_t index_xp1 = convert_to_1D_index(i  , j  , k+1, nz, ny, nx);
                size_t index_ym1 = convert_to_1D_index(i  , j-1, k  , nz, ny, nx);
                size_t index_yp1 = convert_to_1D_index(i  , j+1, k  , nz, ny, nx);
                size_t index_zm1 = convert_to_1D_index(i-1, j  , k  , nz, ny, nx);
                size_t index_zp1 = convert_to_1D_index(i+1, j  , k  , nz, ny, nx);


                float t = -6 * curr[index];

                t+= curr[index_xm1];
                t+= curr[index_xp1];
                t+= curr[index_ym1];
                t+= curr[index_yp1];
                t+= curr[index_zm1];
                t+= curr[index_zp1];

                b[index] = curr[index] + settings->mu[index] * t;
            }
}

void compute_substep(struct params *settings, float *in, float *b, float *out){
    size_t nx = settings->x;
    size_t ny = settings->y;
    size_t nz = settings->z;

    for(size_t i = 1; i < nz-1; i++)
        for(size_t j = 1; j < ny-1; j++)
            for(size_t k = 1; k < nx-1; k++){
                size_t index = convert_to_1D_index(i, j, k, nz, ny, nx);

                size_t index_xm1 = convert_to_1D_index(i  , j  , k-1, nz, ny, nx);
                size_t index_xp1 = convert_to_1D_index(i  , j  , k+1, nz, ny, nx);
                size_t index_ym1 = convert_to_1D_index(i  , j-1, k  , nz, ny, nx);
                size_t index_yp1 = convert_to_1D_index(i  , j+1, k  , nz, ny, nx);
                size_t index_zm1 = convert_to_1D_index(i-1, j  , k  , nz, ny, nx);
                size_t index_zp1 = convert_to_1D_index(i+1, j  , k  , nz, ny, nx);

                float t = in[index_xm1];
                t+= in[index_xp1];
                t+= in[index_ym1];
                t+= in[index_yp1];
                t+= in[index_zm1];
                t+= in[index_zp1];

                out[index] = settings->diag[index] * (b[index] + settings->mu[index]*t);

            }

}

float* compute_cpu(struct params *settings){
    size_t total_size = settings->x * settings->y * settings->z;
    float* in = allocate(sizeof(*in), total_size);
    float* out = allocate(sizeof(*out), total_size);
    float* b = allocate(sizeof(*out), total_size);

    memcpy(in, settings->input, total_size * sizeof(*in));

    float* tmp;

    for(int t = 0; t < settings->n_iterations; t++){
        calculate_b(settings, in, b);
        for(int st = 0; st < settings->n_substep; st ++){
            printf("%d of %d\n", st, settings->n_substep);
            compute_substep(settings, in, b, out);

            //swap the fields around for next iteration
            tmp = in;
            in  = out;
            out = tmp;
        }


    }

    //free out, return in (due to swap)
    deallocate((void**)&out);
    deallocate((void**)&b);

    return in;

}

