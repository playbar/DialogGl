#include "EgFrameBuffer.h"
#include "cclog.h"
#include "limits.h"
#include "assert.h"

#define LOG_TAG "EgFrameBuffer"


	EgFrameBuffer::EgFrameBuffer()
	{
		mOldFrameId = 0xffffffff;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFrameId);
	}

	EgFrameBuffer::~EgFrameBuffer()
	{
		clearAllScreenBuffer();
	}

	void EgFrameBuffer::initInstance(int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			return;
		}
		if (!mapBuffer())
			return;
		init(width, height);
	}

	void EgFrameBuffer::bindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
		glViewport(0, 0, mWidth, mHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void EgFrameBuffer::showCurrentScreenBuffer(int x, int y, int w, int h)
	{
		if (mOldFrameId != 0)
		{
			CCLOG("m_screenbuff is not 0 ");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, mOldFrameId);
		assert(false);
		//GLView::getInstance()->resetGLViewport();
		show();
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void EgFrameBuffer::clearAllScreenBuffer()
	{
		if (mFrameBufferID > 0)
		{
			glDeleteFramebuffers(1, &mFrameBufferID);
		}
		if (mRenderBufferID > 0)
		{
			glDeleteRenderbuffers(0, &mRenderBufferID);
		}

		if (mTextrueId)
		{
			glDeleteTextures(1, &mTextrueId);
		}
		if (_vertexBuffer != UINT_MAX)
		{
			glDeleteBuffers(1, &_vertexBuffer);
		}
		_vertexBuffer = UINT_MAX;

		if (_uvBuffer != UINT_MAX)
		{
			glDeleteBuffers(1, &_uvBuffer);
		}
		_uvBuffer = UINT_MAX;

		if (_colorBuffer != UINT_MAX)
		{
			glDeleteBuffers(1, &_colorBuffer);
		}
		_uvBuffer = UINT_MAX;

		if (_indexBuffer != UINT_MAX)
		{
			glDeleteBuffers(1, &_indexBuffer);
		}
		_uvBuffer = UINT_MAX;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void EgFrameBuffer::show()
	{
		GLShader * shader = GLShader::getShader(TEXTURE_SHADER);
		shader->useProgram();
		glBindTexture(GL_TEXTURE_2D, mTextrueId);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glUniformMatrix4fv(shader->gvViewTransMattixHandle, 1, false, MatrixManager::getScreenBufferViewMatrix());
		CHECK_GL_ERROR("EGTScreenBuffer::show: glUniformMatrix4fv shader->gvViewTransMattixHandle");

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glVertexAttribPointer(shader->gvPositionHandle, 3, GL_FLOAT, false, 0, 0);
		CHECK_GL_ERROR("EGTScreenBuffer::show: glVertexAttribPointer shader->gvPositionHandle");

		glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
		glVertexAttribPointer(shader->gvTextureCoordHandle, 2, GL_FLOAT, false, 0, 0);
		CHECK_GL_ERROR("EGTScreenBuffer::show: glVertexAttribPointer shader->gvTextureCoordHandle");

		glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
		glVertexAttribPointer(shader->gvColorHandle, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
		CHECK_GL_ERROR("EGTScreenBuffer::show: glVertexAttribPointer shader->gvColorHandle");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		CHECK_GL_ERROR("EGTScreenBuffer::show: glDrawElements _indexBuffer");

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	bool EgFrameBuffer::init(int width, int height)
	{
		glGenTextures(1, &this->mTextrueId);
		glBindTexture(GL_TEXTURE_2D, this->mTextrueId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);  
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR("EGTFrameBufferObj::init 0");

		glGenRenderbuffers(1, &this->mRenderBufferID);
		CHECK_GL_ERROR("EGTFrameBufferObj::init 2");

		resize(width, height);
		CHECK_GL_ERROR("EGTFrameBufferObj::init 3");
		//Create frame buffer 
		//	glEnable( GL_FRAMEBUFFER );

		glGenFramebuffers(1, &this->mFrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, this->mFrameBufferID);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mTextrueId, 0);
		//bind depth render buffer 

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->mRenderBufferID);
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LOGE("glCheckFramebufferStatusEXT error status  = %d", status);
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CHECK_GL_ERROR("EGTFrameBufferObj::init ERROR");
		return true;
	}

	bool EgFrameBuffer::mapBuffer()
	{
		GLuint _buffers[4];

		glGenBuffers(4, _buffers);

		CHECK_GL_ERROR("GLFilterManager::mapBuffer glGenBuffers");

		for (int i = 0; i<4; i++)
		{
			if (!_buffers[i] && _buffers[i] == UINT_MAX)
			{
				LOGE("EGTScreenBuffer::mapBuffer glGenBuffers error ");
				return false;
			}
		}
		_vertexBuffer = _buffers[0];
		_uvBuffer = _buffers[1];
		_colorBuffer = _buffers[2];
		_indexBuffer = _buffers[3];

#define DX 0
		/*GLfloat vertexArray[] = {
		0.0f+DX, GLView::getInstance()->getDesignHeight(), 0.0f,
		GLView::getInstance()->getDesignWidth()+DX, GLView::getInstance()->getDesignHeight(), 0.0f ,
		0.0f+DX, 0.0f, 0.0f,
		GLView::getInstance()->getDesignWidth()+DX, 0.0f, 0.0f,
		};*/
		GLfloat vertexArray[] = 
		{
			//0.0f + DX, (GLfloat)GLView::getInstance()->getViewHeight(), 0.0f,
			//(GLfloat)GLView::getInstance()->getViewWidth() + DX, (GLfloat)GLView::getInstance()->getViewHeight(), 0.0f,
			//0.0f + DX, 0.0f, 0.0f,
			//(GLfloat)GLView::getInstance()->getViewWidth() + DX, 0.0f, 0.0f,
			0.0f + DX, (GLfloat)GLView::getInstance()->getViewHeight(), 0.0f,
			(GLfloat)GLView::getInstance()->getViewWidth() + DX, (GLfloat)GLView::getInstance()->getViewHeight(), 0.0f,
			0.0f + DX, 0.0f, 0.0f,
			(GLfloat)GLView::getInstance()->getViewWidth() + DX, 0.0f, 0.0f,

		};
		memcpy(_vertexArray, vertexArray, sizeof(GLfloat) * 4 * 3);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexArray[0]) * 12, _vertexArray, GL_STATIC_DRAW);

		//
		GLfloat uvArray[] = 
		{
			0.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f
		};
		memcpy(_uvArray, uvArray, sizeof(GLfloat) * 4 * 2);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_uvArray[0]) * 8, _uvArray, GL_STATIC_DRAW);

		//
		/*		GLfloat colorArray[] = {
		1.0f, 0xFFFFFF,
		1.0f, 0xFFFFFF,
		1.0f, 0xFFFFFF,
		1.0f, 0xFFFFFF};*/
		GLbyte colorArray[] =
		{
			(GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF,
			(GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF,
			(GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF,
			(GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF, (GLbyte)0XFF
		};
		glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorArray[0]) * 4 * 4, colorArray, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// bind and upload the index
		GLushort indexis[] = { 0, 1, 2, 1, 3, 2 };
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexis[0]) * 6, indexis, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		CHECK_GL_ERROR("EGTScreenBuffer::mapBuffer glBindBuffer end");
		return true;
	}

	void EgFrameBuffer::resize(GLuint width, GLuint height)
	{
		if (this->mWidth == width && this->mHeight == height)
		{
			return;
		}

		this->mWidth = width;
		this->mHeight = height;
		//set texture buffer size
		glBindTexture(GL_TEXTURE_2D, this->mTextrueId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR("EGTFrameBufferObj::resize 0");
		//set depth buffer

		glBindRenderbuffer(GL_RENDERBUFFER, this->mRenderBufferID);
		CHECK_GL_ERROR("EGTFrameBufferObj::resize 1 this->renderBuffer_name ");
		LOGI("EGTFrameBufferObj::resize  ,this->renderBuffer_name = %d (%d,%d)", this->mRenderBufferID, width, this->mHeight);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, this->mWidth, this->mHeight);
		CHECK_GL_ERROR("EGTFrameBufferObj::resize 2");
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

	}
