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

#define ORION_INTERNAL

#include <stdlib.h>
#include <xcb/xcb.h>
#include "../include/Application.hpp"
#include "../include/OSL/OString.hpp"
#include "../include/OKit/CContext.hpp"

namespace Orion{
/*** Context Handling***/

/** Constructors/Destructors **/
	CContext::~CContext(void){ destroy(); }
	CContext::CContext(void) :
		XTYPE{CCT_ERROR}, XWIN{0}, XPARENT{0}, XGC{0}, XCOL{0},
		XMASK{0}, XTITLE{0}, XFLAGS{0,0,0}
		{}

/** Creation **/
	bool CContext::create(CContext* root, int16_t _x, int16_t _y, uint16_t _w, uint16_t _h, const char* t, OCol* col, uint32_t mask, CCType type){
		if(XWIN){ return false; }
		XONLY{
		/* Context initialisation */
			XTYPE   = type;
			XWIN    = xcb_generate_id(XCON);
			XGC     = xcb_generate_id(XCON);
			XPARENT = ( (root) ? root->XWIN : XROOT );
			XCOL    = ( (col) ? col->XCOL : 0);
			XMASK   = mask;
		/* Creating X Window */
			uint32_t tmpVal[2]={ XCOL,XMASK };
			int16_t  x,y;
			uint16_t w,h;
			/* Scale the position differently depending if it's using the WM as a root Context or not. */
			if(root) { x=(_x*OAPP_SCALE); y=(_y*OAPP_SCALE); }
			else     { x=_x; y=_y; }
			w=(_w*OAPP_SCALE); h=(_h*OAPP_SCALE);
			// xcb_void_cookie_t result=
			xcb_create_window(
				XCON,XCB_COPY_FROM_PARENT,XWIN,XPARENT,
				x,y,w,h,0,( (XTYPE!=CCT_INPUTONLY) ? XCB_WINDOW_CLASS_INPUT_OUTPUT : XCB_WINDOW_CLASS_INPUT_ONLY ),
				XSCR->root_visual,XCB_CW_BACK_PIXEL|XCB_CW_EVENT_MASK,tmpVal
			);
			xcb_create_gc(
				XCON,XGC,XWIN,XCB_GC_FOREGROUND,&XCOL
			);
			// if( xcb_request_check(XCON,result) ){ destroy(); return false; }
		/* Type Configuration */
			const char               *cvalA="_NET_WM_WINDOW_TYPE",  *cvalB; /* This is the ONLY time you'll ever see me using the "back *pointer" notation... */
			xcb_intern_atom_cookie_t  cookieA, cookieB;
			xcb_intern_atom_reply_t  *replyA, *replyB;
			xcb_atom_enum_t           modeType;
			bool                      NOTMOTIF=false;
			switch(XTYPE){
				case CCT_ERROR:     { break; } /* The fuck are you doing if this is set? */
				case CCT_INPUTONLY: { break; } /* Handled externally... */
				case CCT_TOPLEVEL:{
					cvalA="_MOTIF_WM_HINTS";
					cookieA = xcb_intern_atom(XCON,0,OStringLength(cvalA),cvalA);
					replyA  = xcb_intern_atom_reply(XCON,cookieA,0);
					uint32_t buff[5]={2,0,0,0,0};
					xcb_change_property(XCON,XCB_PROP_MODE_REPLACE,XWIN,replyA->atom,replyA->atom,32,5,buff);
					free(replyA);
					NOTMOTIF=false; /* This specific one does its own internal operations. */
					break;
				}
				case CCT_DESKTOP:{
					cvalB    = "_NET_WM_WINDOW_TYPE_DESKTOP";
					NOTMOTIF = true;
					modeType = XCB_ATOM_ATOM;
					break;
				}
				case CCT_PANEL:{
					cvalB    = "_NET_WM_WINDOW_TYPE_DOCK";
					NOTMOTIF = true;
					modeType = XCB_ATOM_ATOM;
					break;
				}
				case CCT_TOOLTIP:{
					cvalB    = "_NET_WM_WINDOW_TYPE_TOOLTIP";
					NOTMOTIF = true;
					modeType = XCB_ATOM_ATOM;
					break;
				}
				case CCT_MENU:{
					cvalB    = "_NET_WM_WINDOW_TYPE_MENU";
					NOTMOTIF = true;
					modeType = XCB_ATOM_ATOM;
					break;
				}
				case CCT_ELEMENT: { break; } /* Do nothing. Elements aren't meant to talk to the WM... */
			}

			if(NOTMOTIF){ /* Saves on excess code generated by the compiler. */
				cookieA = xcb_intern_atom(XCON,0,OStringLength(cvalA),cvalA);
				cookieB = xcb_intern_atom(XCON,0,OStringLength(cvalB),cvalB);
				replyA  = xcb_intern_atom_reply(XCON, cookieA, 0);
				replyB  = xcb_intern_atom_reply(XCON, cookieB, 0);

				xcb_change_property(XCON, XCB_PROP_MODE_REPLACE, XWIN, replyA->atom, modeType, 32, 1, &replyB->atom);

				free(replyA);
				free(replyB);
			}
		/* If provided, set the title. */
			setTitle(t);
		/* Finishing up */
			return true;
		}
		return false;
	}

/** Destruction **/
	bool CContext::destroy(void){
		XONLY{
			if(XWIN){
				unmap();
				xcb_destroy_window(XCON,XWIN);
				if(XTITLE){ free(XTITLE); }
				XTYPE=CCT_ERROR;
				XWIN=0;
				XPARENT=0;
				XCOL=0;
				XMASK=0;
				XTITLE=0;
				XFLAGS.XMAPPED=0;
				XFLAGS.XLINKED=0;
				XFLAGS.XABSTRACTED=0;
				return true;
			}
		}
		return false;
	}

/** Modification **/
	bool CContext::map(bool link){
		XONLY{
			if(XWIN && !XFLAGS.XMAPPED){
				// xcb_void_cookie_t result=
				xcb_map_window(XCON,XWIN);
				// if( xcb_request_check(XCON,result) ){ return false; }
				if(link){ XFLAGS.XLINKED=CXHA_LINK(this); }
				XFLAGS.XMAPPED=1;
				return true;
			}
		}
		return false;
	}

	bool CContext::unmap(void){
		XONLY{
			if(XWIN && XFLAGS.XMAPPED){
				// xcb_void_cookie_t result=
				xcb_unmap_window(XCON,XWIN);
				// if( xcb_request_check(XCON,result) ){ return false; }
				if(XFLAGS.XLINKED){ XFLAGS.XLINKED=!CXHA_UNLINK(this); }
				XFLAGS.XMAPPED=0;
				return true;
			}
		}
		return false;
	}

	void CContext::clear(int16_t startX, int16_t startY, uint16_t endX, uint16_t endY){
		XONLY{
			if(XWIN && XFLAGS.XMAPPED){
				xcb_clear_area(
					XCON,1,XWIN,
					(startX*OAPP_SCALE),(startY*OAPP_SCALE),
					(endX*OAPP_SCALE),(endY*OAPP_SCALE)
				);
			}
		}
	}

	void CContext::drawArea(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col){
		XONLY{
			if(XWIN && XFLAGS.XMAPPED){
				x*=OAPP_SCALE,y*=OAPP_SCALE;
				w*=OAPP_SCALE,h*=OAPP_SCALE;
				xcb_rectangle_t r={x,y,w,h};

				xcb_change_gc(XCON,XGC,XCB_GC_FOREGROUND,&col->XCOL);
				xcb_poly_fill_rectangle(XCON,XWIN,XGC,1,&r);
			}
		}
	}
	

	bool CContext::setTitle(const char* title){
		XONLY{
			if(title){
				size_t l=OStringLength(title);
				// xcb_void_cookie_t result=
				xcb_change_property(XCON,XCB_PROP_MODE_REPLACE,XWIN,XCB_ATOM_WM_NAME,XCB_ATOM_STRING,8,l,title);
				// if( xcb_request_check(XCON,result) ){ return false; }
				XTITLE=(char*)malloc(l+1);
				for(size_t i=0;i<l;i++){ XTITLE[i]=title[i]; }
				XTITLE[l]=0;
				return true;
			}else{ if(XTITLE){ free(XTITLE); } XTITLE=0; }
		}
		return false;
	}

	bool CContext::setCol(OCol* col){
		XONLY{
			if(col->XCOL!=XCOL && XWIN){
				// xcb_void_cookie_t result=
				xcb_change_window_attributes(XCON,XWIN,XCB_CW_BACK_PIXEL,&(col->XCOL));
				// if( xcb_request_check(XCON,result) ){ return false; }
				XCOL=col->XCOL;
				return true;
			}
		}
		return false;
	}

	bool CContext::setPos(int16_t x, int16_t y){
		XONLY{
			if(XWIN){
				uint32_t buff[2];
			/* Scale the position differently depending if it's using the WM as a root Context or not. */
				if(XPARENT==XROOT){ buff[0]=x; buff[1]=y; }
				else{ buff[0]=(x*OAPP_SCALE); buff[1]=(y*OAPP_SCALE); }
				xcb_configure_window(XCON,XWIN,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y,buff);
				return true;
			}
		}
		return false;
	}

	bool CContext::setSize(uint16_t w, uint16_t h){
		XONLY{
			if(XWIN){
				uint32_t buff[2]={ (uint32_t)(w*OAPP_SCALE),(uint32_t)(h*OAPP_SCALE) };
				xcb_configure_window(XCON,XWIN,XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,buff);
				return true;
			}
		}
		return false;
	}

	bool CContext::setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h){
		XONLY{
			if(XWIN){
				uint32_t buff[4];
			/* Scale the position differently depending if it's using the WM as a root Context or not. */
				if(XPARENT==XROOT){ buff[0]=x; buff[1]=y; }
				else{ buff[0]=(x*OAPP_SCALE); buff[1]=(y*OAPP_SCALE); }
				buff[2]=(w*OAPP_SCALE); buff[3]=(h*OAPP_SCALE);
				xcb_configure_window(XCON,XWIN,
					XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y|XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,
					buff
				);
				return true;
			}
		}
		return false;
	}

/** Getters/misc ops **/

	OVec4 CContext::getGeometry(void){
		OVec4 ret;
		XONLY{
			if(XWIN){
				/*
				 * This seems really buggy... it doesn't return the position values if the Context's parent-
				 * is the WM...
				 */
				xcb_get_geometry_reply_t* reply=xcb_get_geometry_reply(XCON,xcb_get_geometry(XCON,XWIN),0);
				ret.x=reply->x;
				ret.y=reply->y;
				ret.w=reply->width;
				ret.h=reply->height;
				free(reply);
			}
		}
		return ret;
	}

	void CContext::log(bool verbose, bool newLine){
		if(verbose){
			OLog("CONTEXT : %p | XTYPE : %d | XWIN : %lu | XPARENT : %lu | XCOL : %lu | XMASK : %lu | XTITLE : %s | XFLAGS.XMAPPED : %s | XFLAGS.XLINKED : %s",
				(const void*)this, XTYPE,XWIN,XPARENT,XCOL,XMASK,XTITLE,( (XFLAGS.XMAPPED) ? "true" : "false" ),( (XFLAGS.XLINKED) ? "true" : "false" )
			);
			if(newLine){ OLog("\n"); }
		}else{
			OLog("%lu",XWIN);
			if(newLine){ OLog("\n"); }
		}
	}

/*** Generic Context functions / Context Handling ***/

#define __CXHA_DEFAULT_CAP  30
#define __CXHA_DEFAULT_STEP 10

	struct CXHANDLE{
		uint32_t  XWIN;
		CContext* context;
	};

	static CXHANDLE* CXHA=0;
	static size_t    CXHA_COUNT=0;
	static size_t    CXHA_CAP=__CXHA_DEFAULT_CAP;

	/** Memory management **/

	static bool CXHA_EXPAND(void){
		if(CXHA){
			CXHA_CAP=CXHA_COUNT+__CXHA_DEFAULT_STEP;
			CXHA=(CXHANDLE*)realloc(CXHA,sizeof(CXHANDLE)*CXHA_CAP);
			for(size_t i=CXHA_COUNT;i<CXHA_CAP;i++){ CXHA[i]={0,0}; }
			return true;
		}
		return false;
	}

	bool CXHA_INIT(void){
		if(!CXHA){
			CXHA=(CXHANDLE*)malloc(sizeof(CXHANDLE)*__CXHA_DEFAULT_CAP);
			CXHA_COUNT=0;
			CXHA_CAP=__CXHA_DEFAULT_CAP;
			for(size_t i=0;i<CXHA_CAP;i++){ CXHA[i]={0,0}; }
			return true;
		}
		return false;
	}

	bool CXHA_FREE(void){
		if(CXHA){
			free(CXHA);
			CXHA=0;
			CXHA_COUNT=0;
			CXHA_CAP=__CXHA_DEFAULT_CAP;
		}
		return false;
	}

	/** Linking/Unlinking **/

	bool CXHA_LINK(CContext* context){
		if(CXHA){
			if(CXHA_COUNT+1>=CXHA_CAP){ if(!CXHA_EXPAND()){ return false; } }
		/* Scan for an empty slot */
			for(size_t i=0;i<CXHA_CAP;i++){
				if(CXHA[i].XWIN){ continue; }
				else{
					CXHA[i]={ context->XWIN,context };
					CXHA_COUNT++;
					return true;
				}
			}
		}
		return false;
	}

	bool CXHA_UNLINK(CContext* context){
		if(CXHA){
			for(size_t i=0;i<CXHA_COUNT;i++){
				if(CXHA[i].context==context){
					CXHA[i]={0,0};
					CXHA_COUNT--;
					for(size_t j=i+1;j<CXHA_CAP;j++){
						CXHA[j-1]=CXHA[j];
						CXHA[j]={0,0};
					}
					return true;
				}
			}
		}
		return false;
	}

	/** Return a Context stored in the CXHA from an XID. **/

	CContext* CXHA_FROMXID(uint32_t XWIN){
		if(CXHA){
			for(size_t i=0;i<CXHA_COUNT;i++){
				if(CXHA[i].XWIN==XWIN){ return CXHA[i].context; }
			}
		}
		return 0;
	}

}
