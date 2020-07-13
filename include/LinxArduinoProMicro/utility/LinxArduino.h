/****************************************************************************************
**  LINX header for Arduino family of devices
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
**  Written By Sam Kristoff
**
** BSD2 License.
****************************************************************************************/	

#ifndef LINX_ARDUINO_H
#define LINX_ARDUINO_H


/****************************************************************************************
**  Defines
****************************************************************************************/
#include "LinxDevice.h"
#include "LinxWiringDevice.h"
	
class LinxArduino : public LinxWiringDevice
{
	public:	
		/****************************************************************************************
		**  Variables
		****************************************************************************************/		
		
		/****************************************************************************************
		**  Constructors /  Destructor
		****************************************************************************************/
		LinxArduino();
		
		/****************************************************************************************
		**  Functions
		****************************************************************************************/
		
	private:
		/****************************************************************************************
		**  Variables
		****************************************************************************************/		
				
		
		/****************************************************************************************
		**  Functions
		****************************************************************************************/
	
};

#endif //LINX_ARDUINO_H