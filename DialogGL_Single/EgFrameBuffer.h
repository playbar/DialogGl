#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "gl/glew.h"


	class EgFrameBuffer
	{
	public:
		EgFrameBuffer();
		virtual ~EgFrameBuffer();
		void initInstance(int width , int height );
		void bindFrameBuffer();
		void showCurrentScreenBuffer( int x , int y , int w , int h );
		void clearAllScreenBuffer();
		void show();

	protected:

		bool init(int width, int height);
		bool mapBuffer();
		void resize(GLuint width, GLuint height);

	private:
		GLint mOldFrameId;

	private:
		GLuint _vertexBuffer;
		GLfloat _vertexArray[4 * 3];
		GLuint _uvBuffer;
		GLfloat _uvArray[4 * 2];
		GLuint _colorBuffer;
		GLuint _indexBuffer;

		//////////////////////////////////////////////////////////////////////////
		
		GLuint mFrameBufferID;
		GLuint mRenderBufferID;
		GLuint mTextrueId;
		GLuint mWidth;
		GLuint mHeight;
	};

#endif//__EgFrameBuffer_H__
