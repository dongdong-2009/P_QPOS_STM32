/*
 * ph_EmcCoMod.h
 *
 *  Created on: 4 oct. 2011
 *      Author: frq04766
 */

#ifndef PH_EMVCOREFS_H_
#define PH_EMVCOREFS_H_

#define ENABLE_EMD_SUPP		0x08

/* Generic Reference for all Digital EMVCo modifications */
#define NXPBUILD__PH_EMV

/* Reference for all EMV DEBUG print */
/* Debug prints are needed only for EMV RF Analog tests */
/* Do not activate this define for Digital EMV tests !!! */
/* because Debug prints cause execution delays */
//#define NXPBUILD__PH_EMV_DEBUG_PRINT */

/* Reference for type A */
/*
#define TA_OPTI_1
#define TA_OPTI_2
#define TA001
#define TA101_X
#define TA201_X
#define TA202_X
#define TA204
#define TA205
#define TA302_XX
#define TA302_10
#define TA304_00
#define TA304_11
#define TA306_XX
#define TA307_X
#define TA310
#define TA311_X
#define TA312
#define TA335_X
#define TA401_XX
#define TA401_15
#define TA404_4
#define TA415_X
#define TA421
*/

/* Reference for type B */
/*
#define TB_OPTI_1
#define TB000
#define TB001
#define TB107_X
#define TB201_X
#define TB202_X
#define TB204
#define TB305_X
#define TB306_10
#define TB311_X
#define TB404_X
*/

#endif /* PH_EMVCOREFS_H_ */
