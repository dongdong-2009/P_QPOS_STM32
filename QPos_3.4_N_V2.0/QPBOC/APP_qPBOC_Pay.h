#ifndef __APP_QPBOC_PAY_H__
#define __APP_QPBOC_PAY_H__


#ifndef NULL
#define NULL 0
#endif

#define  PREPROCESSING_END  			0
#define  PREPROCESSING_DONE 			1
#define  REQUEST_MESSAGE 				2
#define  SELECT_ANOTHER_AID			3



#define	FAIL		1
#define	SUCCESS	0


unsigned short App_qPBOC_Purchase(unsigned char OnlineFlag);

#endif
