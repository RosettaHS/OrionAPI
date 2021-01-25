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

#ifndef __ORION_OKIT_H__
#define __ORION_OKIT_H__

#define OKIT_VERSION 	0
#define OKIT_REVISION	0

#include "errdef.h"
#include "signals.h"
#include "application.hpp"
#include "xservice.hpp"
#include "OLog.hpp"
#include "CLoggable.hpp"
#include "OVec.hpp"
#include "OCol.hpp"
#include "OTheme.hpp"
#include "OString.hpp"
#include "CBaseUI.hpp"
#include "OSignal.hpp"
#include "CSignalListener.hpp"
#include "CEventArray.hpp"
#include "CSignalDispatcher.hpp"
#include "CXEvent.hpp"
#include "CContext.hpp"
#include "CDrawable.hpp"
#include "CNodeArray.hpp"
#include "CContainer.hpp"

#include "OWindow.hpp"

#include "OService.hpp"

/*
*	Since "using namespace Orion;" is so common in OApps, and since everything is prefixed anyway,
*	it might make more sense to force "using namespace Orion;" on and allow it to be disabled,
*	rather than force everyone to write "using namespace Orion;" in each of their files,
*	as is dictated in each tutorial and example for OKit. 
*/

#ifndef ORION_UNUSE_NAMESPACE
using namespace Orion;
#endif /* !ORION_UNUSE_NAMESPACE */

#endif /* !__ORION_OKIT_H__ */