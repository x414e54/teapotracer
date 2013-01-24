#include "GLRenderer.h"

//-----------------------------------------------------------------------------
// Init opengl
//-----------------------------------------------------------------------------
bool GLRenderer::Init(AntSettings settings)
{
		if(SDL_Init(SDL_INIT_VIDEO) < 0 )
		{
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
 
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,            32);
 
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,        8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,        8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
 
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_Surface *bg;
		SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_OPENGL);
		SDL_WM_SetCaption("NAme","O");

		glClearColor(1, 1, 1, 1);

		return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::BeginScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::EndScene()
{
	SDL_GL_SwapBuffers();//SwapBuffers(m_deviceContext);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetView(POVector3* pos, POVector3* target)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,
	          0.0, 0.0, 0.0,
	          0.0, 1.0, 0.0);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetProjection()
{
	   glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	   gluPerspective(3.14f * 0.5f, (float)800 / (float)600, 1.0f, 1000.0f);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawMesh(UINT meshId, POVector3* pos, POVector3* o)
{
	glBegin(GL_QUADS);
	        glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
	        glColor3f(1, 1, 0); glVertex3f(100, 0, 0);
	        glColor3f(1, 0, 1); glVertex3f(100, 100, 0);
	        glColor3f(1, 1, 1); glVertex3f(0, 100, 0);
	    glEnd();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawText(UINT fontId, const std::wstring* string, RECT* dst, AntFontColorARGB* fontColor)
{
	//if (string.empty())
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* o)
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddFont(const std::wstring& typeFace, UINT width, UINT height)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddTexture(const std::wstring& fileName)
{
	AntTexture* texture = new AntTexture;
	texture->h=0;
	texture->w=0;
	texture->fileName = fileName;

	char buffer[fileName.length()];
	int ret = wcstombs( buffer, fileName.c_str(), sizeof(buffer) );
	// checking ret here.
	GLuint* tex = new GLuint();
	texture->texture = (void*)tex;
	SDL_Surface *surface;
	surface = IMG_Load(buffer);
	if (!surface) { fprintf(stderr,"TextureLoadFailed(FileNotFound)"); return; }
	glGenTextures( 1, (GLuint*)texture->texture );
	glBindTexture( GL_TEXTURE_2D, *(GLuint*)texture->texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
	texture->h=surface->h;
	texture->w=surface->w;
	SDL_FreeSurface( surface );
	if (texture!=0) {
		_textures.push_back(texture);
	} else {
		fprintf(stderr,"TextureLoadFailed(TryingToContinue)");
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddMesh(const std::wstring& fileName)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool GLRenderer::Cleanup()
{

	return false;
}
