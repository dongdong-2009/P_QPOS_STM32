/* /////////////////////////////////////////////////////////////////////////////////////////////////
//                     Copyright (c) NXP Semiconductors
//
//                        
//         All rights are reserved. Reproduction in whole or in part is
//        prohibited without the written consent of the copyright owner.
//    NXP reserves the right to make changes without notice at any time.
//   NXP makes no warranty, expressed, implied or statutory, including but
//   not limited to any implied warranty of merchantability or fitness for any
//  particular purpose, or that the use will not infringe any third party patent,
//   copyright or trademark. NXP must not be liable for any loss or damage
//                            arising from its use.
///////////////////////////////////////////////////////////////////////////////////////////////// */

/*! 
 * \if cond_generate_filedocu 
 * \file phcsBflRegCtl.h
 *
 * Project: Object Oriented Library Framework Register Control Component.
 *
 *  Source: phcsBflRegCtl.h
 * $Author: mha $
 * $Revision: 1.3.1.1 $
 * $Date: Fri Jun 30 14:32:25 2006 $
 *
 * Comment:
 *  This is the file to glue together all RegCtl implementations for all different hardware variants.
 *
 * History:
 *  GK:  Generated 18. March 2003
 *  MHa: Migrated to MoReUse September 2005
 * \endif
 * 
 */


#ifndef PHCSBFLREGCTL_H
#define PHCSBFLREGCTL_H

/* Type defintions */
#include "ph_Status.h"
#include "ph_TypeDefs.h"

/*! \ingroup regctl
 *  \brief Parameter structure for SetRegister functionality.
 */
typedef struct 
{
    uint8_t     address;             /*!< \brief [in] References a register location within the RF chip.
                                              This member is not modified by the function. */
    uint8_t     reg_data;            /*!< \brief [in,out] Data to write to the specified register. */
    void        *self;                /*!< \brief [in] Pointer to the C-interface (\ref phcsBflRegCtl_t) in   
                                              order to let this member function find its "object". 
                                              Should be left dangling when calling the C++ wrapper.*/
} phcsBflRegCtl_SetRegParam_t;

/*! \ingroup regctl
 *  \brief Parameter structure for GetRegister functionality.
 */
typedef struct 
{
    uint8_t     address;             /*!< \brief [in] References a register location within the RF chip.
                                              This member is not modified by the function. */
    uint8_t     reg_data;            /*!< \brief [in,out] Data to read from the specified register. */
    void        *self;                /*!< \brief [in] Pointer to the C-interface (\ref phcsBflRegCtl_t) in   
                                              order to let this member function find its "object". 
                                              Should be left dangling when calling the C++ wrapper.*/
} phcsBflRegCtl_GetRegParam_t;


/*! \ingroup regctl
 *  \brief Parameter structure for ModifyRegister functionality. The function performs a 
 *  Read-Modify-Write operation.
 */
typedef struct 
{
    uint8_t     address;             /*!< \brief [in] References a register location within the RF chip.
                                             This member is not modified by the function. */
    uint8_t     mask;                /*!< \brief [in] A mask, specifying which register bits to modify.
                                             While bits set to one in the mask are touched in the register,
                                             those set to zero remain unchanged. */
    uint8_t     set;                 /*!< \brief [in] Type of operation  - Set = 1, Clear = 0. The bits set to one
                                             in the mask are modified in the register, according to the
                                             value of this member. */
    void       *self;                /*!< \brief [in] Pointer to the C-interface (\ref phcsBflRegCtl_t) in   
                                             order to let this member function find its "object". 
                                             Should be left dangling when calling the C++ wrapper.*/
} phcsBflRegCtl_ModRegParam_t;

/*! \ingroup regctl
 *  \brief Parameter structure for SetRegisterMultiple functionality.
 *  The function writes multiple bytes specified into the chosen register.
 */
typedef struct 
{
    uint8_t     address;             /*!< \brief [in] References a register location within the RF chip to be written
                                             or read from. This member is not modified by the function. */
    uint16_t    length;              /*!< \brief [in] Length of the buffer handed over to the function for write operations. 
                                             For read operations this parameter carries the number of bytes to read. */
    uint8_t    *buffer;              /*!< \brief [in,out] Buffer which carries the data for chosen transaction. 
                                             For read operations the length variable must not exceed this length of 
                                             the buffer in order to avoid buffer overruns. */
    void       *self;                /*!< \brief [in] Pointer to the C-interface (\ref phcsBflRegCtl_t) in   
                                             order to let this member function find its "object". 
                                             Should be left dangling when calling the C++ wrapper.*/
} phcsBflRegCtl_SetMultiRegParam_t;

/*! \ingroup regctl
 *  \brief Parameter structure for GetRegisterMultiple functionality.
 *  The function writes multiple bytes specified into the chosen register.
 */
typedef struct 
{
    uint8_t     address;             /*!< \brief [in] References a register location within the RF chip to be written
                                             or read from. This member is not modified by the function. */
    uint16_t    length;              /*!< \brief [in] Length of the buffer handed over to the function for write operations. 
                                             For read operations this parameter carries the number of bytes to read. */
    uint8_t    *buffer;              /*!< \brief [in,out] Buffer which carries the data for chosen transaction. 
                                             For read operations the length variable must not exceed this length of 
                                             the buffer in order to avoid buffer overruns. */
    void       *self;                /*!< \brief [in] Pointer to the C-interface (\ref phcsBflRegCtl_t) in   
                                             order to let this member function find its "object". 
                                             Should be left dangling when calling the C++ wrapper.*/
} phcsBflRegCtl_GetMultiRegParam_t;


/*  C-interface member function pointer types: */
/*!
 * \if sec_PHFL_BFL_CPP 
 * \fn phcsBfl_Status_t phcs_BFL::phcsBflRegCtl_Wrapper< phcsBflRCcp >::SetRegister(phcsBflRegCtl_SetRegParam_t *setreg_param)
 * \else
 * \fn phcsBfl_Status_t phcsBflRegCtl_t::SetRegister
 * \endif
 *
 * \ingroup regctl 
 *  \par Parameters:
 *  \li \ref phcsBflRegCtl_SetRegParam_t [in]: Pointer to the register I/O parameter structure.
 *
 *  \retval  PH_ERR_BFL_SUCCESS                Operation successful.
 *  \retval  Other                             Depending on implementation and underlaying component.
 *
 *  This function handles the register write access to the Rc52x as defined in the hardware specification.
 *  Interface handling (acknowledge, ...) is handled internally.
 *
 */
typedef phStatus_t (*pphcsBflRegCtl_SetReg_t)     (phcsBflRegCtl_SetRegParam_t*);

/*!
 * \if sec_PHFL_BFL_CPP 
 * \fn phcsBfl_Status_t phcs_BFL::phcsBflRegCtl_Wrapper< phcsBflRCcp >::GetRegister(phcsBflRegCtl_GetRegParam_t *getreg_param)
 * \else
 * \fn phcsBfl_Status_t phcsBflRegCtl_t::GetRegister()
 * \endif
 *
 * \ingroup regctl 
 *  \par Parameters:
 *  \li \ref phcsBflRegCtl_GetRegParam_t [in,out]: Pointer to the register I/O parameter structure.
 *
 *  \retval  PH_ERR_BFL_SUCCESS                Operation successful.
 *  \retval  Other                             Depending on implementation and underlaying component.
 *
 *  This function handles the read access to the Rc52x as defined in the hardware specification.
 *  Bus handling (acknowledge, ...) is handled internally.
 */
typedef phStatus_t (*pphcsBflRegCtl_GetReg_t)     (phcsBflRegCtl_GetRegParam_t*);

/*!
 * \if sec_PHFL_BFL_CPP 
 * \fn phcsBfl_Status_t phcs_BFL::phcsBflRegCtl_Wrapper< phcsBflRCcp >::ModifyRegister(phcsBflRegCtl_ModRegParam_t *modify_param)
 * \else
 * \fn phcsBfl_Status_t phcsBflRegCtl_t::ModifyRegister
 * \endif
 *
 * \ingroup regctl 
 *  \par Parameters:
 *  \li \ref phcsBflRegCtl_ModRegParam_t [in]: Pointer to the register I/O parameter structure.
 *
 *  \retval  PH_ERR_BFL_SUCCESS                Operation successful.
 *  \retval  Other                             Depending on implementation and underlaying component.
 *
 * This function handles register bit modifications of the Rc52x. Therefore a GetRegister command is 
 * performed before a SetRegistercommand. In between the modification is done. 
 * The bits which are 1 in the mask of \ref phcsBflRegCtl_ModRegParam_t are set or cleared according 
 * to set of \ref phcsBflRegCtl_ModRegParam_t. If this parameter is set to 0, the bits are cleared, 
 * otherwise they are set.
 * \note Some register content may change in between read and write. For these registers use an
 *       apropriate way of bitwise modification.
 */
typedef phStatus_t (*pphcsBflRegCtl_ModReg_t)     (phcsBflRegCtl_ModRegParam_t*);

/*!
 * \if sec_PHFL_BFL_CPP 
 * \fn phcsBfl_Status_t phcs_BFL::phcsBflRegCtl_Wrapper< phcsBflRCcp >::SetRegisterMultiple(phcsBflRegCtl_SetMultiRegParam_t *setmultireg_param)
 * \else
 * \fn phcsBfl_Status_t phcsBflRegCtl_t::SetRegisterMultiple
 * \endif
 *
 * \ingroup regctl 
 *  \par Parameters:
 *  \li \ref phcsBflRegCtl_SetMultiRegParam_t [in]: Pointer to the register I/O parameter structure.
 *
 *  \retval  PH_ERR_BFL_SUCCESS                Operation successful.
 *  \retval  Other                             Depending on implementation and underlaying component.
 *
 *  This function handles the write access for multiple data bytes to one register of the Rc52x. Depending 
 *  on the hardware support the function \ref SetRegister is either called as often as 
 *  required to write all data bytes or the dedicated harware support is used.
 */
typedef phStatus_t (*pphcsBflRegCtl_SetMultiReg_t)(phcsBflRegCtl_SetMultiRegParam_t*);

/*!
 * \if sec_PHFL_BFL_CPP 
 * \fn phcsBfl_Status_t phcs_BFL::phcsBflRegCtl_Wrapper< phcsBflRCcp >::GetRegisterMultiple(phcsBflRegCtl_GetMultiRegParam_t *getmultireg_param)
 * \else
 * \fn phcsBfl_Status_t phcsBflRegCtl_t::GetRegisterMultiple
 * \endif
 *
 * \ingroup regctl 
 *  \par Parameters:
 *  \li \ref phcsBflRegCtl_SetMultiRegParam_t [in,out]: Pointer to the register I/O parameter structure.
 *
 *  \retval  PH_ERR_BFL_SUCCESS                Operation successful.
 *  \retval  Other                             Depending on implementation and underlaying component.
 *
 *  This function handles multiple read accesses for data bytes from one register adress of the Rc52x. 
 *  Depending on the hardware support the function \ref GetRegister is either called as 
 *  often as required to read all data bytes or the dedicated harware support is used.
 */
typedef phStatus_t (*pphcsBflRegCtl_GetMultiReg_t)(phcsBflRegCtl_GetMultiRegParam_t*);

    
/*! \ifnot sec_PHFL_BFL_CPP
 * 
 *  \ingroup regctl
 *  \brief C-interface structure of register access functionality.
 *
 *  The Register Control interface is the basic related interface. It allows access of RC registers
 *  and their bits. This component is the most versatile, but least specific one. Usually this component
 *  is the lowest one.
 * 
 * \endif
 */
typedef struct 
{
    /* Methods: */
    pphcsBflRegCtl_SetReg_t      SetRegister;           /* SetRegister member function. */
    pphcsBflRegCtl_GetReg_t      GetRegister;           /* GetRegister member function. */
    pphcsBflRegCtl_ModReg_t      ModifyRegister;        /* ModifyRegister member function. */
    pphcsBflRegCtl_SetMultiReg_t SetRegisterMultiple;   /* SetRegisterMultiple member function. */
    pphcsBflRegCtl_GetMultiReg_t GetRegisterMultiple;   /* GetRegisterMultiple member function. */

    void *mp_Members;   /* Internal variables of the C-interface. Usually a structure is behind
                           this pointer. The type of the structure depends on the implementation
                           requirements. */
    #ifdef PHFL_BFL_CPP
        void       *mp_CallingObject;   /* Used by the "Glue-Class" to reference the wrapping
                                           C++ object, calling into the C-interface. */
    #endif

    /* Lower edge: */
//    phcsBflBal_t   *mp_Lower;
} phcsBflRegCtl_t;





/* //////////////////////////////////////////////////////////////////////////////////////////////
// Reg-Ctl Initialise for Serial Interface on Rc52x:
*/
/*! \cond cond_addCStruct */
/*! \ingroup rs232
 *  \brief Parameter structure for RS232 interface. Currently not used.
 */
typedef struct 
{
    /* For the current implementation we do not need any parameter. */
     uint8_t    dummy;
} phcsBflRegCtl_SerHw1Params_t;
/*! \endcond */

/*!
* \ingroup rs232
* \param[in] *cif       Pointer to the instance of the C object interface structure
* \param[in] *p_params  Pointer to the internal operation variables structure instance.
* \param[in] *p_lower   Pointer to the underlying layer.
* 
* \brief This function shall be called first to initialise the register control component.
*
* An own function pointer is typedef'ed for this function to enable the call within
* a generic C++ Reg-Ctl wrapper. For the C++ wrapper implementation see INCRCREGCTL.H .
*/
void      phcsBflRegCtl_SerHw1Init(phcsBflRegCtl_t  *cif,
                                 void           *p_params);






/* //////////////////////////////////////////////////////////////////////////////////////////////
// Reg-Ctl Initialise for I2C Interface on Rc52x:
*/
/*! \ingroup i2c
 *  \brief Parameter structure for I2C configuration. 
 */
typedef struct 
{
    uint8_t  slave_address;  /*!< \brief Address of the slave to be addressed (depending 
                                        on external circuit).*/
    uint8_t *speed;          /*!< \brief Speed clock registers from controller (0, 1, 2).
                                        if pointer is set to NULL, default data rate is set 
                                        to 533 kBit. */
} phcsBflRegCtl_I2CHw1Params_t;


/* //////////////////////////////////////////////////////////////////////////////////////////////
// Reg-Ctl Initialise for Rc52x:
*/
/*!
* \ingroup i2c
* \param[in] *cif       Pointer to the instance of the C object interface structure
* \param[in] *p_params  Pointer to the internal operation variables structure instance.
* \param[in] *p_lower   Pointer to the underlying layer.
* 
* This function shall be called first to initialise the register control component for the I2C interface.
* The parameters contain the the slave address and the speed for communication 
* (see \link phcsBflRegCtl_I2CHw1Params_t phcsBflRegCtl_I2CHw1Params_t \endlink). 
* These parameters are set during initialization.
* An own function pointer is typedef'ed for this function to enable the call within a generic C++ 
* Reg-Ctl wrapper. For the C++ wrapper see the file INCRCREGCTL.H.
*/
void      phcsBflRegCtl_I2CHw1Init(phcsBflRegCtl_t  *cif,
                                 void           *p_params);





/* //////////////////////////////////////////////////////////////////////////////////////////////
// Reg-Ctl Initialise for SPI Interface on Rc52x:
*/
/*! \name Hardware definitions */
/*@{*/
/*! \ingroup spi */
/* Following defines have to be adjusted according to the hardware setup.
 * This is just an example. From here...
 */
#define PHCS_BFLREGCTL_SPIHW1_NSS			PB0
#define PHCS_BFLREGCTL_SPIHW1_SCK			PB1
#define PHCS_BFLREGCTL_SPIHW1_MOSI		    PB2
#define PHCS_BFLREGCTL_SPIHW1_MISO		    PB3
#define PHCS_BFLREGCTL_SPIHW1_NRESET		PB5
/* IRQ currently not used 
#define PHCS_BFLREGCTL_SPIHW1_IRQ			PD3 */

/* Defines the port */
#define PHCS_BFLREGCTL_SPIHW1_DATA_PORT	  PORTB
/* Defines whether the pin is output or input */
#define PHCS_BFLREGCTL_SPIHW1_DATA_DIR_PORT DDRB
/*@}*/

/* ... until there. */

/*! \ingroup spi
 *  \brief Parameter structure for SPI interface. Currently used to change the communication data rate.
 */
typedef struct 
{
    /*! \brief We use a uint32_t value for the initialisation process
        to choose different data rates for communication.
     */
     uint32_t   baudrate;
} phcsBflRegCtl_SpiHw1Params_t;


/* //////////////////////////////////////////////////////////////////////////////////////////////
// Reg-Ctl Initialise for Rc52x:
*/
/*!
* \ingroup spi
* \param[in] *cif       Pointer to the instance of the C object interface structure
* \param[in] *p_params  Pointer to the internal operation variables structure instance.
* \param[in] *p_lower   Pointer to the underlying layer.
* 
* This function shall be called first to initialise the register control component.
* An own function pointer is typedef'ed for this function to enable the call within
* a generic C++ Reg-Ctl wrapper. See INCRCREGCTL.H .
*
*/
void     phcsBflRegCtl_SpiHw1Init(phcsBflRegCtl_t  *cif,
                                 void           *p_params);




#endif  /* PHCSBFLREGCTL_H */
