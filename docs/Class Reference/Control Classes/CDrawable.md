# Control Class Reference: CDrawable
## Abstractive Rendering

### Definition
```cpp
class CDrawable : public CBaseUI , public CLoggable {
	protected:
		int x,y;
		int centreX, centreY;
		unsigned int w,h;
		unsigned int minW,minH;
		float scale;
		float rotation;
		int index;

		CContext* context;
		CDrawable* parentDrawable;
		CContainer* parentContainer;

		OTheme internalTheme;
		CTheme theme;
		uint8_t themeFlags;

		friend class CContainer;
	public:
		void(*drawPtr)(CDrawable*);

		struct{
			uint8_t size;
			uint8_t pos;
		}flags;

		CDrawable(void);

		bool linkTo(CContainer&);
		bool unlinkTo(CContainer&);

		void setPos(int x, int y);
		void setCentre(int x, int y);
		void setSize(unsigned int w,unsigned int h);
		void setMinSize(unsigned int w, unsigned int h);
		void setScale(float);
		void setRotation(float);
		void setTheme(OTheme&);
		void setPrimaryCol(unsigned char r, unsigned char g, unsigned char b); void setPrimaryCol(OCol&);
		void setSecondaryCol(unsigned char r, unsigned char g, unsigned char b); void setSecondaryCol(OCol&);
		void setTertiaryCol(unsigned char r, unsigned char g, unsigned char b); void setTertiaryCol(OCol&);
		void setAccentCol(unsigned char r, unsigned char g, unsigned char b); void setAccentCol(OCol&);
		void resetTheme(void);

		OVec getPos(bool globalToWindow=false);
		OVec getCentre();
		OVec4 getSize(bool useScale=false);
		OVec4 getMinSize(bool useScale=false);
		float getScale(bool includeParents=false);
		float getRotation(bool includeParents=false);
		OVec4 getGeometry(bool globalToWindow=false);
		int getIndex(void);

		OTheme getTheme(void);

		virtual void log(bool verbose=false) override;
};
```
### Use
CDrawable extends from [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md), meaning it can be logged both verbosely and minimally.
