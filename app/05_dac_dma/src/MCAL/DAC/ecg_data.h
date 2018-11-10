/****************************************************************************************************/
/**
\file       ecg_data.h
\brief      ECG data resampled.
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       14/October/2016
*/
/****************************************************************************************************/

#ifndef __ECG_DATA_H        /*prevent duplicated includes*/
#define __ECG_DATA_H

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core modules */
#include "compiler.h"
#include "typedefs.h"

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/


/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/


/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/

/** DAC Initialization function */
const uint16_t ecg_resampled_integer[27778];

/****************************************************************************************************/

#endif /* __DAC_H */
