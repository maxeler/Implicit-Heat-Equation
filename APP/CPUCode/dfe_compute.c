/*
 * dfe_compute.c
 *
 *  Created on: 30 Apr 2015
 *      Author: carmen
 */

#include "dfe_compute.h"
#include "memory.h"
#include <maxlibfd.h>

struct dfe_handle{
    maxlib_context maxlib;
    maxlib_lmem_array lmem_wave_in;
    maxlib_lmem_array lmem_wave_midStep;
    maxlib_lmem_array lmem_wave_out;
    maxlib_lmem_array lmem_model;

};

float* compute_dfe(struct params *settings){
    struct dfe_handle *handle = settings->dfe_handle;

    size_t total_size = settings->x * settings->y * settings->z;
    float* result     = allocate(sizeof(*result), total_size);
    int numSubStep;

    maxlib_get_user_parameter("numSubStep", &numSubStep);

    for (int t = 0; t < settings->n_iterations; t++){
        for(int st = 0; st < settings->n_substep; st+=numSubStep){

            maxlib_stream_from_lmem(handle->maxlib, "x" , handle->lmem_wave_in);
            if(st == 0)
                maxlib_stream_from_lmem(handle->maxlib, "x_k" , handle->lmem_wave_in);
            else
                maxlib_stream_from_lmem(handle->maxlib, "x_k" , handle->lmem_wave_midStep);

            maxlib_stream_to_lmem  (handle->maxlib, "x_kn", handle->lmem_wave_out);

            maxlib_stream_earthmodel_from_lmem(handle->maxlib, handle->lmem_model);

            maxlib_stream_region_to_host(handle->maxlib, "receiver",
                    t==settings->n_iterations-1 && (st+=numSubStep >= settings->n_substep)? result : NULL,
                    0, 0, 0, settings->x, settings->y, settings->z);

            maxlib_run(handle->maxlib);

            maxlib_lmem_array tmp;
            tmp = handle->lmem_wave_midStep;
            handle->lmem_wave_midStep  = handle->lmem_wave_out;
            handle->lmem_wave_out = tmp;
        }

        maxlib_lmem_array tmp;
        tmp = handle->lmem_wave_in;
        handle->lmem_wave_in  = handle->lmem_wave_midStep;
        handle->lmem_wave_midStep = tmp;
    }

    return result;


}

void initialise_dfe(struct params *settings){
    struct dfe_handle *handle = allocate(sizeof(struct dfe_handle), 1);
    handle->maxlib = maxlib_open(settings->x, settings->y, settings->z);
    maxlib_print_status(handle->maxlib);

    handle->lmem_wave_in       = maxlib_lmem_alloc_wavefield (handle->maxlib);
    handle->lmem_wave_midStep  = maxlib_lmem_alloc_wavefield (handle->maxlib);
    handle->lmem_wave_out      = maxlib_lmem_alloc_wavefield (handle->maxlib);
    handle->lmem_model         = maxlib_lmem_alloc_earthmodel(handle->maxlib);

    maxlib_lmem_load_wavefield(handle->maxlib, handle->lmem_wave_in, settings->input);
    maxlib_lmem_load_wavefield_with_zeros(handle->maxlib, handle->lmem_wave_out);
    maxlib_lmem_load_wavefield_with_zeros(handle->maxlib, handle->lmem_wave_midStep);

    maxlib_earthmodel em =  maxlib_earthmodel_create_in_memory(settings->x, settings->y, settings->z);

    maxlib_earthmodel_set_data(em, "diag", settings->diag);
    maxlib_earthmodel_set_data(em, "mu", settings->mu);
    maxlib_lmem_load_earthmodel(handle->maxlib, handle->lmem_model, em);
    maxlib_earthmodel_release(em);


    settings->dfe_handle = handle;
}

void cleanup_dfe(struct params *settings){
    maxlib_close(settings->dfe_handle->maxlib);

}
