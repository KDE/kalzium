#ifndef KALZIUMGLHELPERCLASSES_H
#define KALZIUMGLHELPERCLASSES_H
/***************************************************************************
    copyright            : (C) 2006 by Benoit Jacob
 ***************************************************************************/

/***************************************************************************
 *         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.   *
 *         *
 ***************************************************************************/
#include <math.h>
#include <QGLWidget>
#include <QPainter>
#include <QImage>
#include <QChar>
#include <QHash>
#include <kdebug.h>

#include <openbabel/mol.h>
#include <openbabel/obiter.h>

/** USE_FPS_COUNTER: if defined, the GL Widget will show a frames-per-second
 * counter. Use only for testing: this makes the GL Widget constantly
 * redraw, which under normal circumstances is a waste of CPU time.
 */
#define USE_FPS_COUNTER 

/** USE_DISPLAY_LISTS: if defined, the whole scene will be stored in
 * an OpenGL display list. The vertex arrays will then be converted into
 * (nested) display lists, in order to avoid replication of geometric data.
 * This option improves performance, especially when rendering complex models,
 * but increases memory usage.
 */
//#define USE_DISPLAY_LISTS

namespace KalziumGLHelpers
{

/**
 * This struct represents a style in which to render a molecule
 *
 * @author Benoit Jacob
 */
struct MolStyle
{
	enum BondStyle
	{
		BONDS_DISABLED,
		BONDS_GRAY,
		BONDS_USE_ATOMS_COLORS
	} m_bondStyle;

	enum AtomStyle
	{
		ATOMS_DISABLED,
		ATOMS_USE_FIXED_RADIUS,
		ATOMS_USE_VAN_DER_WAALS_RADIUS,
	} m_atomStyle;

	double m_singleBondRadius;
	double m_multipleBondRadius;
	double m_multipleBondShift;
	double m_atomRadiusFactor;

	void setup( BondStyle bondStyle, AtomStyle atomStyle,
		double singleBondRadius,
		double multipleBondRadius,
		double multipleBondShift,
		double atomRadiusFactor );

	double getAtomRadius( int atomicNumber );
	inline double getAtomRadius( const OpenBabel::OBAtom *atom )
	{ return getAtomRadius( atom->GetAtomicNum() ); }
};

/**
* This class represents a color in OpenGL float red-green-blue-alpha format.
*
* @author Benoit Jacob
*/
struct Color
{
	GLfloat m_red, m_green, m_blue, m_alpha;

	Color() {}

	/**
	 * This constructor sets the four components of the color
	 * individually. Each one ranges from 0.0 (lowest intensity) to
	 * 1.0 (highest intensity). For the alpha component, 0.0 means fully
	 * transparent and 1.0 (the default) means fully opaque.
	 */
	Color( GLfloat red, GLfloat green, GLfloat blue,
		GLfloat alpha = 1.0 );

	/**
	 * This constructor uses OpenBabel to retrieve the color in which
	 * the atom should be rendered.
	 */
	Color( const OpenBabel::OBAtom *atom );

	Color& operator=( const Color& other );

	/**
	* Sets this color to be the one used by OpenGL for rendering
	* when lighting is disabled.
	*/
	inline void apply()
	{
		glColor4fv( reinterpret_cast<GLfloat *>( this ) );
	}

	/**
	* Applies nice OpenGL materials using this color as the
	* diffuse color while using different shades for the ambient and
	* specular colors. This is only useful if lighting is enabled.
	*/
	void applyAsMaterials();
};

/**
* Given a vector U, constructs two unit vectors v and w
* such that (U, v, w) is an orthogonal basis.
* U is not supposed to be normalized.
*
* Returns false if something went wrong.
*/
bool createOrthoBasisGivenFirstVector( const OpenBabel::vector3 &U, OpenBabel::vector3 & v, OpenBabel::vector3 & w );

/**
* This is an abstract base class for an OpenGL vertex array.
*
* @author Benoit Jacob
*/
class VertexArray
{
	protected:

		struct Vector
		{
			GLfloat x, y, z;
		};

		Vector *m_vertexBuffer;
		Vector *m_normalBuffer;
		unsigned short *m_indexBuffer;
		GLenum m_mode;
		int m_vertexCount;
		int m_indexCount;
		GLuint m_displayList;
		bool m_hasIndexBuffer;
		bool m_hasSeparateNormalBuffer;
		bool m_isValid;
		
		virtual int computeVertexCount() = 0;
		virtual int computeIndexCount() { return 0; }
		virtual void buildBuffers() = 0;
		bool allocateBuffers();
		void freeBuffers();
		void compileDisplayList();

		void initialize();

	public:
		VertexArray( GLenum mode,
			bool hasIndexBuffer,
			bool hasSeparateNormalBuffer );
		virtual ~VertexArray();

		void do_draw();
		inline void draw()
		{
#ifdef USE_DISPLAY_LISTS
			if( m_isValid ) glCallList( m_displayList );
#else
			if( m_isValid ) do_draw();
#endif
		}
};

/**
* This class represents and draws a sphere
*
* @author Benoit Jacob
*/
class Sphere : public VertexArray
{
	private:
		inline unsigned short indexOfVertex(
			int strip, int column, int row);
		void computeVertex( int strip, int column, int row );

	protected:
		int m_detail;
		virtual int computeVertexCount();
		virtual int computeIndexCount();
		virtual void buildBuffers();

	public:
		Sphere();
		virtual ~Sphere() {}
		virtual void setup( int detail );
		virtual void draw( const OpenBabel::vector3 &center, double radius );
};

/**
* This class represents and draws a cylinder
*
* @author Benoit Jacob
*/
class Cylinder : public VertexArray
{
	protected:
		int m_faces;

		virtual int computeVertexCount();
		virtual void buildBuffers();

	public:
		Cylinder();
		virtual ~Cylinder() {}
		virtual void setup( int detail );
		virtual void draw( const OpenBabel::vector3 &end1, const OpenBabel::vector3 &end2,
			double radius, int order = 1, double shift = 0.0 );
};

/** This is a helper class for TextRenderer, and should probably never be
* used directly. See TextRenderer.
*
* The CharRenderer class represents a character stored as OpenGL rendering
* data : a texture object and a display list mapping it on a quad and then
* translating to the right of it.
*
* See the m_charTable member of TextRenderer for an example of use of
* this class.
*/
class CharRenderer
{
	protected:
		/**
		 * The OpenGL texture object
		 */
		GLuint m_texture;

		/**
		 * The OpenGL display list
		 */
		GLuint m_displayList;

		/**
		 * Width and height in pixels of the rendered character
		 */
		int m_width, m_height;

	public:
		CharRenderer();
		~CharRenderer();
		bool initialize( QChar c, const QFont &font,
			GLenum textureTarget );
		inline void draw()
		{
			glCallList( m_displayList );
		}
};


/** This class renders text inside a QGLWidget. It replaces the functionality
* of QGLWidget::renderText(). The advantages over renderText() include:
*  - supports any font, any character encoding supported by Qt
*    (renderText is 8-bit-only and can only use "OpenGL-compatible" fonts)
*  - does not use any library outside Qt (renderText uses FreeType on X11)
*  - renders characters as textured quads instead of calling glDrawPixels,
*    which does not make much of a difference on MesaGL, but can be a lot
*    faster and safer with other (buggy) OpenGL implementations. It will also
*    allow to add more graphical effects in the future, like rotation,
*    if we ever need that.
*  - the characters are stored as 8bpp Alpha, which takes 4 times less
*    memory than the 32bpp RGBA used by renderText.
*  - the characters are rendered on-the-fly on the first time they appear
*    in a QString being printed. This is achieved using a QHash to test whether
*    a character has already been rendered.
*
* Recommended usage:
* The TextRender class is meant to be used from inside a child class of
* QGLWidget, say MyGLWidget.
*
* In the declaration of MyGLWidget, please declare a TextRenderer member:
* 
* @code
class MyGLWidget : public QGLWidget
{
	...
	TextRenderer m_textRenderer;
	...
};
* @endcode
* 
* Now, in the constructor of MyGLWidget, please call setup() along these lines:
*
* @code
	QFont f;
	f.setStyleHint( QFont::SansSerif, QFont::PreferAntialias );
	m_textRenderer.setup( this, f );
* @endcode 
*
* The setup() method should be called only once, which means you have to choose
* a font once and for all, in the lifetime of your TextRenderer. Any QFont can
* be used, the above is just an example. Now, to actually render text, in
* the MyGLWidget::paintGL() method, you can call

* @code
	m_textRenderer.print( x, y, string );
* @endcode

* where x,y are ints and string is any QString. If you want to choose a color,
* please call glColor3f or glColor4f before calling print(). Of course you can
* also call qglColor or Color::apply. You can achieve semitransparent text at
* no additional cost by choosing a semitransparent color.
*
* If you wish to do several calls to print(), it will improve performance
* to enclose them between a call to begin() and a call to end(), like that:
* 
* @code
	m_textRenderer.begin();
	m_textRenderer.print( x1, y1, string1 );
	m_textRenderer.print( x2, y2, string2 );
	m_textRenderer.print( x3, y2, string3 );
	m_textRenderer.end();
* @endcode
* 
* Please make sure, though, that no relevant OpenGL state change occurs between
* begin() and end(), except the state changes performed by the TextRenderer
* itself. In other words, please avoid calling glSomething() between begin() and
* end(), except if you are sure that this call won't perform a relevant state
* change.
*
* The print() method when called alone, or the begin()-print()-end() group,
* do restore the OpenGL state as they found it, including the matrices.
*
* If you experience rendering problems, you can try the following:
* - disable some OpenGL state bits. For instance, TextRenderer automatically
*   disables fog and lighting during rendering, because it doesn't work
*   correctly with them enabled. There probably are other OpenGL state bits
*   that have to be disabled, so if your program enables some of them, you
*   might have to disable them before rendering text.
* - if you experience poor font quality, please consider using an antialiased
*   font.
*
* @author Benoit Jacob
*/
class TextRenderer
{
	protected:
		/**
		 * The font used for rendering the chars. This is set
		 * once and for all by setup(). Note that it is stored
		 * by value, so the caller doesn't have to keep it alive.
		 */
		QFont m_font;
		
		/**
		 * This hash gives the correspondence table between QChars
		 * (the keys) and the corresponding CharRenderers (the values).
		 * Every time a QChar is being met, either it is found in this
		 * table, in which case it can be directly rendered, or it is
		 * not found, in which case a new CharRenderer is created for
		 * it and added to this table.
		 */
		QHash<QChar, CharRenderer*> m_charTable;

		/**
		 * The QGLWidget in which to render. This is set
		 * once and for all by setup().
		 */
		const QGLWidget *m_glwidget;

		/**
		 * This equals true if begin() has been called, but end() hasn't
		 * since.
		 */
		GLboolean m_isBetweenBeginAndEnd;

		///{ Members used to remember the OpenGL state in order to be able to restore it after rendering. See do_end().
		GLboolean m_wasEnabled_LIGHTING;
		GLboolean m_wasEnabled_textureTarget;
		GLboolean m_wasEnabled_FOG;
		GLboolean m_wasEnabled_BLEND;
		GLboolean m_wasEnabled_DEPTH_TEST;
		///}

		GLenum m_textureTarget;

		/**
		 * Stores the relevant part of the OpenGL state, and prepares
		 * for rendering
		 */
		void do_begin();

		/**
		 * Restores the OpenGL state
		 */
		void do_end();

	public:
		TextRenderer();
		~TextRenderer();
		
		/**
		 * This should be called only once, before any printing occurs.
		 * @param glwidget The QGLWidget in which to render.
		 * See m_glwidget member.
		 * @param font The QFont to use. See m_font member.
		 */
		void setup( const QGLWidget *glwidget, const QFont &font );

		/**
		 * Prints text at the position (x,y) in window coordinates
		 * (0,0) is the bottom left corner
		 * @param x the x-coordinate
		 * @param y the y-coordinate
		 * @param string the QString to print
		 */
		void print( int x, int y, const QString &string);

		/**
		 * Call this before doing multiple calls to print(). This is
		 * not necessary, but will improve performance. Don't forget,
		 * then, to call end() after.
		 */
		void begin();

		/**
		 * Call this after having called begin() and print().
		 */
		void end();
};

} // namespace KalziumGL

#endif // KALZIUMGLHELPERCLASSES_H

