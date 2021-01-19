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

#include "OCol.hpp"

/* Quick re-route for Orion::OTHEME_GLOBAL. */
#define OTHEME Orion::OTHEME_GLOBAL
/* Quick re-route for Orion::OTHEME_GLOBAL.primary. */
#define OTHEME_PRIMARY Orion::OTHEME_GLOBAL.primary
/* Quick re-route for Orion::OTHEME_GLOBAL.secondary. */
#define OTHEME_SECONDARY Orion::OTHEME_GLOBAL.secondary
/* Quick re-route for Orion::OTHEME_GLOBAL.tertiary. */
#define OTHEME_TERTIARY Orion::OTHEME_GLOBAL.primary
/* Quick re-route for Orion::OTHEME_GLOBAL.accent. */
#define OTHEME_ACCENT Orion::OTHEME_GLOBAL.accent

namespace Orion{
	/* Container for multiple OCols. Used for theming UI elements. */
	struct OTheme{
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
		/* Prints all OCol data out to the terminal. */
		void log(void);
	};

	void OLog(OTheme&);
	void OLog(OTheme*);
	void OVLog(OTheme&);
	void OVLog(OTheme&);

	/* The global theme for the OApp. */
	extern OTheme OTHEME_GLOBAL;

	/* Overrides that prevent Orion from automatically re-theming applications when the system theme changes. Can be overridden by system toggles. */
	extern bool OTHEME_PRIMARY_OVERRIDE;
	extern bool OTHEME_SECONDARY_OVERRIDE;
	extern bool OTHEME_TERTIARY_OVERRIDE;
	extern bool OTHEME_ACCENT_OVERRIDE;

#define _CTHEME_OVERRIDE_PRIMARY	0x1
#define _CTHEME_OVERRIDE_SECONDARY	0x2
#define _CTHEME_OVERRIDE_TERTIARY	0x4
#define _CTHEME_OVERRIDE_ACCENT		0x8

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