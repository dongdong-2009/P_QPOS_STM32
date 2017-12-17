#ifndef __RC522REG_H__
#define __RC522REG_H__



/*! \name Register definitions of Page 0
 *  \ingroup reg
 *  Following all register defintions of the RC522 Page 0.
 */
/*@{*/
#define     JREG_RFU00           0x00      /*   Currently not used.                                     */

#define     JREG_COMMAND         0x01      /*!< Contains Command bits, PowerDown bit and bit to
                                                switch receiver off.                                    */
#define     JREG_COMMIEN         0x02      /*!< Contains Communication interrupt enable bits and
                                                bit for Interrupt inversion.                            */
#define     JREG_DIVIEN          0x03      /*!< Contains RfOn, RfOff, CRC and Mode Interrupt enable

                                                and bit to switch Interrupt pin to PushPull mode.       */
#define     JREG_COMMIRQ         0x04      /*!< Contains Communication interrupt request bits.          */
#define     JREG_DIVIRQ          0x05      /*!< Contains RfOn, RfOff, CRC and Mode Interrupt request.  */
#define     JREG_ERROR           0x06      /*!< Contains Protocol, Parity, CRC, Collision, Buffer
                                                overflow, Temperature and RF error flags.               */

#define     JREG_STATUS1         0x07      /*!< Contains status information about Lo- and HiAlert,
                                                RF-field on, Timer, Interrupt request and CRC status.   */
#define     JREG_STATUS2         0x08      /*!< Contains information about internal states (Modemstate),
                                                Mifare states and possibility to switch Temperature
                                                sensor off.                                             */

#define     JREG_FIFODATA        0x09      /*!< Gives access to FIFO. Writing to register increments the
                                                FIFO level (register 0x0A), reading decrements it.      */
#define     JREG_FIFOLEVEL       0x0A      /*!< Contains the actual level of the FIFO.                  */
#define     JREG_WATERLEVEL      0x0B      /*!< Contains the Waterlevel value for the FIFO              */
#define     JREG_CONTROL         0x0C      /*!< Contains information about last received bits and to

                                                Start and stop the Timer unit.                          */
#define     JREG_BITFRAMING      0x0D      /*!< Contains information of last bits to send, to align
                                                received bits in FIFO and activate sending in Transceive*/
#define     JREG_COLL            0x0E      /*!< Contains all necessary bits for Collission handling     */
#define     JREG_RFU0F           0x0F      /*   Currently not used.                                     */
/*@}*/


/*! \name Register definitions of Page 1
 *  \ingroup reg
 *  Following all register defintions of the RC522 Page 1.
 */
/*@{*/
#define     JREG_RFU10           0x10      /*   Currently not used.                                     */

#define     JREG_MODE            0x11      /*!< Contains bits for auto wait on Rf, to detect SYNC byte in
                                                NFC mode and MSB first for CRC calculation              */
#define     JREG_TXMODE          0x12      /*!< Contains Transmit Framing, Speed, CRC enable, bit for
                                                inverse mode and TXMix bit.                             */
#define     JREG_RXMODE          0x13      /*!< Contains Transmit Framing, Speed, CRC enable, bit for

                                                multiple receive and to filter errors.                  */
#define     JREG_TXCONTROL       0x14      /*!< Contains bits to activate and configure Tx1 and Tx2 and
                                                bit to activate 100> modulation.                        */
#define     JREG_TXASK           0x15      /* */
#define     JREG_TXSEL           0x16      /*!< Contains SigoutSel, DriverSel and LoadModSel bits.      */

#define     JREG_RXSEL           0x17      /*!< Contains UartSel and RxWait bits.                       */
#define     JREG_RXTRESHOLD      0x18      /*!< Contains MinLevel and CollLevel for detection.          */
#define     JREG_DEMOD           0x19      /*!< Contains bits for time constants, hysteresis and
                                                IQ demodulator settings.                                */
#define     JREG_RFU1A           0x1A      /* */

#define     JREG_RFU1B           0x1B      /* */
#define     JREG_RFU1C           0x1C      /* */
#define     JREG_RFU1D           0x1D      /* */
#define     JREG_RFU1E           0x1E      /*   Currently not used.                                     */
#define     JREG_SERIALSPEED     0x1F      /*!< Contains speed settings for serila interface.           */
/*@}*/

/*! \name Register definitions of Page 2

 *  \ingroup reg
 *  Following all register defintions of the RC522 Page 2.
 */
/*@{*/
#define     JREG_RFU20           0x20      /*   Currently not used.                                     */
#define     JREG_CRCRESULT1      0x21	   /*!< Contains MSByte of CRC Result.                          */

#define     JREG_CRCRESULT2      0x22	   /*!< Contains LSByte of CRC Result.                          */
#define     JREG_RFU23           0x23      /*   Currently not used.                                     */
#define     JREG_MODWIDTH        0x24      /*!< Contains modulation width setting.                      */
#define     JREG_RFU25           0x25      /*   Currently not used.                                     */
#define     JREG_RFCFG           0x26      /*!< Contains sensitivity of Rf Level detector, the receiver
                                                gain factor and the RfLevelAmp.                         */

#define     JREG_GSN             0x27      /*!< Contains the conductance and the modulation settings for
                                                the N-MOS transistor. */
#define     JREG_CWGSP           0x28      /*!< Contains the conductance for the P-Mos transistor.      */
#define     JREG_MODGSP          0x29      /*!< Contains the modulation index for the PMos transistor.  */
#define     JREG_TMODE           0x2A      /*!< Contains all settings for the timer and the highest 4

                                                bits of the prescaler.                                  */
#define     JREG_TPRESCALER      0x2B      /*!< Contais the lowest byte of the prescaler.               */
#define     JREG_TRELOADHI       0x2C      /*!< Contains the high byte of the reload value.             */
#define     JREG_TRELOADLO       0x2D      /*!< Contains the low byte of the reload value.              */
#define     JREG_TCOUNTERVALHI   0x2E      /*!< Contains the high byte of the counter value.            */

#define     JREG_TCOUNTERVALLO   0x2F      /*!< Contains the low byte of the counter value.             */
/*@}*/

/*! \name Register definitions of Page 3
 *  \ingroup reg
 *  Following all register defintions of the RC522 Page 3.
 */

/*@{*/
#define     JREG_RFU30           0x30      /*   Currently not used.                                     */
#define     JREG_TESTSEL1        0x31      /*   Test register                                           */
#define     JREG_TESTSEL2        0x32      /*   Test register                                           */
#define     JREG_TESTPINEN       0x33      /*   Test register                                           */
#define     JREG_TESTPINVALUE    0x34      /*   Test register                                           */
#define     JREG_TESTBUS         0x35      /*   Test register                                           */
#define     JREG_AUTOTEST        0x36      /*   Test register                                           */
#define     JREG_VERSION         0x37      /*!< Contains the product number and the version.            */

#define     JREG_ANALOGTEST      0x38      /*   Test register                                           */
#define     JREG_TESTDAC1        0x39      /*   Test register                                           */
#define     JREG_TESTDAC2        0x3A      /*   Test register                                           */
#define     JREG_TESTADC         0x3B      /*   Test register                                           */
#define     JREG_ANALOGUETEST1   0x3C      /*   Test register                                           */
#define     JREG_RFT3D           0x3D      /*   Test register                                           */
#define     JREG_RFT3E           0x3E      /*   Test register                                           */
#define     JREG_RFT3F           0x3F      /*   Test register                                           */
/*@}*/



/* /////////////////////////////////////////////////////////////////////////////
 * Possible commands
 * ////////////////////////////////////////////////////////////////////////// */
/*! \name RC522 Command definitions
 *  \ingroup reg

 *  Following all commands of the RC522.
 */
/*@{*/
#define     JCMD_IDLE          0x00 /*!< No action: cancel current command
                                     or home state. \n */
#define     JCMD_CALCCRC       0x03 /*!< Activate the CRC-Coprocessor \n<em><strong>

                                     Remark: </strong>The result of the CRC calculation can
                                     be read from the register CRCResultXXX </em>*/
#define     JCMD_TRANSMIT      0x04 /*!< Transmit data from FIFO to the card \n<em>
                                     <strong>Remark: </strong>If data is already in

                                     the FIFO when the command is activated, this data is
                                     transmitted immediately. It is possible to
                                     write data to the FIFO while the Transmit
                                     command is active. Thus it is possible to
                                     transmit an unlimited number of bytes in one
                                     stream by writting them to the FIFO in time.</em>*/

#define     JCMD_NOCMDCHANGE   0x07 /*!< This command does not change the actual commant of
                                     the RC522 and can only be written. \n<em><strong>
                                     Remark: </strong>This command is used for WakeUp procedure
                                     of RC522 to not change the current state. </em>*/

#define     JCMD_RECEIVE       0x08 /*!< Activate Receiver Circuitry. Before the
                                     receiver actually starts, the state machine
                                     waits until the time configured in the
                                     register RxWait has passed. \n<em><strong>
                                     Remark: </strong>It is possible to read any received

                                     data from the FIFO while the Receive command
                                     is active. Thus it is possible to receive an
                                     unlimited number of bytes by reading them
                                     from the FIFO in time. </em>*/
#define     JCMD_TRANSCEIVE    0x0C /*!< This Command has two modes:\n

                                     Transmits data from FIFO to the card and after
                                     that automatically activates
                                     the receiver. Before the receiver actually
                                     starts,the state machine waits until the
                                     time configured in the register RxWait has
                                     passed. \n <em><strong>

                                     Remark: </strong>This command is the combination of
                                     Transmit and Receive.</em> */
#define     JCMD_AUTHENT       0x0E /*!< Perform the card authentication using the
                                     Crypto1 algorithm.

                                     \n <em><strong>Remark: </strong></em>*/
#define     JCMD_SOFTRESET     0x0F /*!< Runs the Reset- and Initialisation Phase
                                     \n <em><strong>Remark:</strong> This command can

                                     be activated by software, but only by a Power-On
                                     or Hard Reset </em>*/
/*@}*/


/* /////////////////////////////////////////////////////////////////////////////
 * Bit Definitions

 * ////////////////////////////////////////////////////////////////////////// */
/*! \name RC522 Bit definitions of Page 0
 *  \ingroup reg
 *  Below there are useful bit definition of the RC522 register set of Page 0.
 */
/*@{*/
/* Command Register							(01) */

#define     JBIT_RCVOFF             0x20   /*!< Switches the receiver on/off. */
#define     JBIT_POWERDOWN          0x10   /*!< Switches RC522 to Power Down mode. */

/* CommIEn Register							(02) */
#define     JBIT_IRQINV             0x80   /*!< Inverts the output of IRQ Pin. */


/* DivIEn Register							(03) */
#define     JBIT_IRQPUSHPULL        0x80   /*!< Sets the IRQ pin to Push Pull mode. */

/* CommIEn and CommIrq Register         (02, 04) */
#define     JBIT_TXI                0x40   /*!< Bit position for Transmit Interrupt Enable/Request. */
#define     JBIT_RXI                0x20   /*!< Bit position for Receive Interrupt Enable/Request. */

#define     JBIT_IDLEI              0x10   /*!< Bit position for Idle Interrupt Enable/Request. */
#define     JBIT_HIALERTI           0x08   /*!< Bit position for HiAlert Interrupt Enable/Request. */
#define     JBIT_LOALERTI           0x04   /*!< Bit position for LoAlert Interrupt Enable/Request. */
#define     JBIT_ERRI               0x02   /*!< Bit position for Error Interrupt Enable/Request. */
#define     JBIT_TIMERI             0x01   /*!< Bit position for Timer Interrupt Enable/Request. */


/* DivIEn and DivIrq Register           (03, 05) */
#define     JBIT_SIGINACTI          0x10   /*!< Bit position for SiginAct Interrupt Enable/Request. */
#define     JBIT_CRCI               0x04   /*!< Bit position for CRC Interrupt Enable/Request. */

/* CommIrq and DivIrq Register          (04, 05) */
#define     JBIT_SET                0x80   /*!< Bit position to set/clear dedicated IRQ bits. */


/* Error Register 							(06) */
#define     JBIT_WRERR              0x80   /*!< Bit position for Write Access Error. */
#define     JBIT_TEMPERR            0x40   /*!< Bit position for Temerature Error. */
#define     JBIT_BUFFEROVFL         0x10   /*!< Bit position for Buffer Overflow Error. */
#define     JBIT_COLLERR            0x08   /*!< Bit position for Collision Error. */

#define     JBIT_CRCERR             0x04   /*!< Bit position for CRC Error. */
#define     JBIT_PARITYERR          0x02   /*!< Bit position for Parity Error. */
#define     JBIT_PROTERR            0x01   /*!< Bit position for Protocol Error. */

/* Status 1 Register 						(07) */
#define     JBIT_CRCOK              0x40   /*!< Bit position for status CRC OK. */

#define     JBIT_CRCREADY           0x20   /*!< Bit position for status CRC Ready. */
#define     JBIT_IRQ                0x10   /*!< Bit position for status IRQ is active. */
#define     JBIT_TRUNNUNG           0x08   /*!< Bit position for status Timer is running. */
#define     JBIT_HIALERT            0x02   /*!< Bit position for status HiAlert. */
#define     JBIT_LOALERT            0x01   /*!< Bit position for status LoAlert. */


/* Status 2 Register				    		(08) */
#define     JBIT_TEMPSENSOFF        0x80   /*!< Bit position to switch Temperture sensors on/off. */
#define     JBIT_I2CFORCEHS         0x40   /*!< Bit position to forece High speed mode for I2C Interface. */
#define     JBIT_CRYPTO1ON          0x08   /*!< Bit position for reader status Crypto is on. */


/* FIFOLevel Register				    		(0A) */
#define     JBIT_FLUSHBUFFER        0x80   /*!< Clears FIFO buffer if set to 1 */

/* Control Register					    		(0C) */
#define     JBIT_TSTOPNOW           0x80   /*!< Stops timer if set to 1. */
#define     JBIT_TSTARTNOW          0x40   /*!< Starts timer if set to 1. */


/* BitFraming Register					    (0D) */
#define     JBIT_STARTSEND          0x80   /*!< Starts transmission in transceive command if set to 1. */

/* BitFraming Register					    (0E) */
#define     JBIT_VALUESAFTERCOLL    0x80   /*!< Activates mode to keep data after collision. */
/*@}*/


/*! \name RC522 Bit definitions of Page 1
 *  \ingroup reg
 *  Below there are useful bit definition of the RC522 register set of Page 1.
 */
/*@{*/
/* Mode Register							(11) */

#define     JBIT_TXWAITRF           0x20   /*!< Tx waits until Rf is enabled until transmit is startet, else
                                                transmit is started immideately. */
#define     JBIT_POLSIGIN           0x08   /*!< Inverts polarity of SiginActIrq, if bit is set to 1 IRQ occures
                                                when Sigin line is 0. */

/* TxMode Register							(12) */

#define     JBIT_INVMOD             0x08   /*!< Activates inverted transmission mode. */

/* RxMode Register							(13) */
#define     JBIT_RXNOERR            0x08   /*!< If 1, receiver does not receive less than 4 bits. */

/* Definitions for Tx and Rx		    (12, 13) */
#define     JBIT_106KBPS            0x00   /*!< Activates speed of 106kbps. */

#define     JBIT_212KBPS            0x10   /*!< Activates speed of 212kbps. */
#define     JBIT_424KBPS            0x20   /*!< Activates speed of 424kbps. */

#define     JBIT_CRCEN              0x80   /*!< Activates transmit or receive CRC. */

/* TxControl Register						(14) */

#define     JBIT_INVTX2ON           0x80   /*!< Inverts the Tx2 output if drivers are switched on. */
#define     JBIT_INVTX1ON           0x40   /*!< Inverts the Tx1 output if drivers are switched on. */
#define     JBIT_INVTX2OFF          0x20   /*!< Inverts the Tx2 output if drivers are switched off. */
#define     JBIT_INVTX1OFF          0x10   /*!< Inverts the Tx1 output if drivers are switched off. */
#define     JBIT_TX2CW              0x08   /*!< Does not modulate the Tx2 output, only constant wave. */

#define     JBIT_TX2RFEN            0x02   /*!< Switches the driver for Tx2 pin on. */
#define     JBIT_TX1RFEN            0x01   /*!< Switches the driver for Tx1 pin on. */

/* Demod Register 							(19) */
#define     JBIT_FIXIQ              0x20   /*!< If set to 1 and the lower bit of AddIQ is set to 0, the receiving is fixed to I channel.
                                                If set to 1 and the lower bit of AddIQ is set to 1, the receiving is fixed to Q channel. */

/*@}*/


/*! \name RC522 Bit definitions of Page 2
 *  \ingroup reg
 *  Below there are useful bit definition of the RC522 register set.
 */

/*@{*/
/* TMode Register 							(2A) */
#define     JBIT_TAUTO              0x80   /*!< Sets the Timer start/stop conditions to Auto mode. */
#define     JBIT_TAUTORESTART       0x10   /*!< Restarts the timer automatically after finished
                                                counting down to 0. */
/*@}*/



/* /////////////////////////////////////////////////////////////////////////////
 * Bitmask Definitions
 * ////////////////////////////////////////////////////////////////////////// */
/*! \name RC522 Bitmask definitions
 *  \ingroup reg

 *  Below there are some useful mask defintions for the RC522. All specified
 *  bits are set to 1.
 */
/*@{*/

/* Command register                 (0x01)*/
#define     JMASK_COMMAND           0x0F   /*!< Bitmask for Command bits in Register JREG_COMMAND. */


/* Waterlevel register              (0x0B)*/
#define     JMASK_WATERLEVEL        0x3F   /*!< Bitmask for Waterlevel bits in register JREG_WATERLEVEL. */

/* Control register                 (0x0C)*/
#define     JMASK_RXBITS            0x07   /*!< Bitmask for RxLast bits in register JREG_CONTROL. */


/* Mode register                    (0x11)*/
#define     JMASK_CRCPRESET         0x03   /*!< Bitmask for CRCPreset bits in register JREG_MODE. */

/* TxMode register                  (0x12, 0x13)*/
#define     JMASK_SPEED             0x70   /*!< Bitmask for Tx/RxSpeed bits in register JREG_TXMODE and JREG_RXMODE. */

/* TxSel register                   (0x16)*/

#define     JMASK_DRIVERSEL         0x30   /*!< Bitmask for DriverSel bits in register JREG_TXSEL. */
#define     JMASK_SIGOUTSEL         0x0F   /*!< Bitmask for SigoutSel bits in register JREG_TXSEL. */

/* RxSel register                   (0x17)*/
#define     JMASK_UARTSEL           0xC0   /*!< Bitmask for UartSel bits in register JREG_RXSEL. */
#define     JMASK_RXWAIT            0x3F   /*!< Bitmask for RxWait bits in register JREG_RXSEL. */


/* RxThreshold register             (0x18)*/
#define     JMASK_MINLEVEL          0xF0   /*!< Bitmask for MinLevel bits in register JREG_RXTHRESHOLD. */
#define     JMASK_COLLEVEL          0x07   /*!< Bitmask for CollLevel bits in register JREG_RXTHRESHOLD. */

/* Demod register                   (0x19)*/
#define     JMASK_ADDIQ             0xC0   /*!< Bitmask for ADDIQ bits in register JREG_DEMOD. */

#define     JMASK_TAURCV            0x0C   /*!< Bitmask for TauRcv bits in register JREG_DEMOD. */
#define     JMASK_TAUSYNC           0x03   /*!< Bitmask for TauSync bits in register JREG_DEMOD. */

/* RFCfg register                   (0x26)*/
#define     JMASK_RXGAIN            0x70   /*!< Bitmask for RxGain bits in register JREG_RFCFG. */


/* GsN register                     (0x27)*/
#define     JMASK_CWGSN             0xF0   /*!< Bitmask for CWGsN bits in register JREG_GSN. */
#define     JMASK_MODGSN            0x0F   /*!< Bitmask for ModGsN bits in register JREG_GSN. */

/* CWGsP register                   (0x28)*/
#define     JMASK_CWGSP             0x3F   /*!< Bitmask for CWGsP bits in register JREG_CWGSP. */


/* ModGsP register                  (0x29)*/
#define     JMASK_MODGSP            0x3F   /*!< Bitmask for ModGsP bits in register JREG_MODGSP. */

/* TMode register                   (0x2A)*/
#define     JMASK_TGATED            0x60   /*!< Bitmask for TGated bits in register JREG_TMODE. */
#define     JMASK_TPRESCALER_HI     0x0F   /*!< Bitmask for TPrescalerHi bits in register JREG_TMODE. */


/*@}*/

#endif /* __RC522REG_H__ */

/* ///////////////////////////////////////////////////////////////////////////
 * End of File
 * //////////////////////////////////////////////////////////////////////// */