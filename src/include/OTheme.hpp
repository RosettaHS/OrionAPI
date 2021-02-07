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

#ifndef __ORION_OKIT_OTHEME_H__
#define __ORION_OKIT_OTHEME_H__

#include "CLoggable.hpp"
#include "OCol.hpp"

#ifndef ORION_NOHELPERS

/* The Global Theme for this OApp. See OTheme. */
#define OTHEME Orion::OTHEME_GLOBAL
/* The primary colour of this OApp's Global Theme. See OTheme. */
#define OTHEME_PRIMARY Orion::OTHEME_GLOBAL.primary
/* The secondary colour of this OApp's Global Theme. See OTheme. */
#define OTHEME_SECONDARY Orion::OTHEME_GLOBAL.secondary
/* The tertiary colour of this OApp's Global Theme. See OTheme. */
#define OTHEME_TERTIARY Orion::OTHEME_GLOBAL.tertiary
/* The accen colour of this OApp's Global Theme. See OTheme. */
#define OTHEME_ACCENT Orion::OTHEME_GLOBAL.accent

#endif /* !ORION_NOHELPERS */

namespace Orion{
	/* Container for multiple OCols. Used for theming UI elements. */
	struct OTheme : public CLoggable{
		/* The primary colour of the theme. Used for backgrounds. */
		OCol primary;
		/* The secondary colour of the theme. Used for top-level UI elements. */
		OCol secondary;
		/* The tertiary colour of the theme. Used for borders between elements. */
		OCol tertiary;
		/* The accent colour of the theme.  Used for accents around elements or in symbols. */
		OCol accent;

		void setPrimary(unsigned char r, unsigned char g, unsigned char b);
		void setSecondary(unsigned char r, unsigned char g, unsigned char b);
		void setTertiary(unsigned char r, unsigned char g, unsigned char b);
		void setAccent(unsigned char r, unsigned char g, unsigned char b);
		/* Logs the information of this Theme to the terminal. Pass true for more verbose information. */
		virtual void log(bool verbose=false) override;
	};

	/* The Global Theme for this OApp. */
	extern OTheme OTHEME_GLOBAL;

	/* Overrides that prevent Orion from automatically re-theming applications when the system theme changes. Can be overridden by system toggles. */
	extern bool OTHEME_PRIMARY_OVERRIDE;
	extern bool OTHEME_SECONDARY_OVERRIDE;
	extern bool OTHEME_TERTIARY_OVERRIDE;
	extern bool OTHEME_ACCENT_OVERRIDE;

#ifdef ORION_INTERNAL

	#define _CTHEME_OVERRIDE_PRIMARY	0x1
	#define _CTHEME_OVERRIDE_SECONDARY	0x2
	#define _CTHEME_OVERRIDE_TERTIARY	0x4
	#define _CTHEME_OVERRIDE_ACCENT		0x8

#endif /* !ORION_INTERNAL */

	/* Internal themer for UI elements. Contains pointers to the real values to use and can be overridden. */
	struct CTheme{
		/* Pointer to the primary colour to use. Can either be from OTHEME_GLOBAL or an internal colour. */
		OCol* primary;
		/* Pointer to the secondary colour to use. Can either be from OTHEME_GLOBAL or an internal colour. */
		OCol* secondary;
		/* Pointer to the tertiary colour to use. Can either be from OTHEME_GLOBAL or an internal colour. */
		OCol* tertiary;
		/* Pointer to the accent colour to use. Can either be from OTHEME_GLOBAL or an internal colour. */
		OCol* accent;
	};

}

#endif /* !__ORION_OKIT_OTHEME_H__ */