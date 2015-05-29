/**\file */
#ifndef SLIC_DECLARATIONS_ImplicitHeatEquation_H
#define SLIC_DECLARATIONS_ImplicitHeatEquation_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ImplicitHeatEquation_PCIE_ALIGNMENT (16)
/**
 * \brief Initialise a maxfile.
 */
max_file_t* ImplicitHeatEquation_init(void);

/* Error handling functions */
int ImplicitHeatEquation_has_errors(void);
const char* ImplicitHeatEquation_get_errors(void);
void ImplicitHeatEquation_clear_errors(void);
/* Free statically allocated maxfile data */
void ImplicitHeatEquation_free(void);
/* returns: -1 = error running command; 0 = no error reported */
int ImplicitHeatEquation_simulator_start(void);
/* returns: -1 = error running command; 0 = no error reported */
int ImplicitHeatEquation_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_ImplicitHeatEquation_H */

