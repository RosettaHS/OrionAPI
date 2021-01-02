/**********************************************************************************/
/*                                                                                */
/*                              Copyright (c) 2021                                */
/*                           Rosetta H&S International                            */
/*                                                                                */
/*  Permission is hereby granted, free of charge, to any person obtaining a copy  */
/*  of this software and associated documentation files (the "Software"), to deal */
/*  in the Software without restriction, including without limitation the right   */
/*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   */
/*       copies of the Software, and to permit persons to whom the Software is    */
/*          furnished to do so, subject to the following conditions:              */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/*                copies or substantial portions of the Software.                 */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/*                                                                                */
/**********************************************************************************/

#ifndef __ORION_OKIT_APPLICATION_H__
#define __ORION_OKIT_APPLICATION_H__

/* Wrapper if() statement for exiting upon fatal errors. Include <stdlib.h>! */
#define OERR_EXIT(errcode) if(Orion::Application::fatalErrors){exit(errcode);}
/* Wrapper if() statements for printing debug information to the terminal. Include <stdio.h>! */
#define OVERB_OUT if(Orion::Application::verbose){printf(
#define OVERB_END );}

/* Quick re-route to Orion::Application::scale. Use this to get scale of the application. */
#define OAPP_SCALE Orion::Application::scale
/* Quick re-route to Orion::Application::fatalErrors. Use this to check if OKit errors will result in process termination. */
#define OAPP_FATALERRORS Orion::Application::fatalErrors
/* Quick re-route to Orion::Application::verbose. Use this to check if OKit is set to Verbose mode. */
#define OAPP_VERBOSE Orion::Application::verbose

namespace Orion{
	namespace Application{
		/* The global UI scale of the OApp. */
		extern float	scale;
		/* Forces OApp to terminate upon encountering any error. */
		extern bool		fatalErrors;
		/* Causes OKit to print out data to the terminal for most interactions. */
		extern bool		verbose;
		
		/* Add more later, focus on getting the Context to work first! */
	}
}

#endif /* !__ORION_OKIT_APPLICATION_H__ */