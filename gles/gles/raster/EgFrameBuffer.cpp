#include "EgFrameBuffer.h"
#include "cclog.h"
#include "limits.h"
#include "assert.h"
#include "memory.h"
#include "EgretFilter.h"
#include "gl/matrix.h"

#define LOG_TAG "EgFrameBuffer"


EgFrameBuffer::EgFrameBuffer()
{
	mOldFrameId = 0xffffffff;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFrameId);
}

EgFrameBuffer::~EgFrameBuffer()
{
	clear();
}

void EgFrameBuffer::init(int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		return;
	}
	mWidth = width;
	mHeight = height;
	glGenTextures(1, &mTextrueId);
	glBindTexture(GL_TEXTURE_2D, mTextrueId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	glGenRenderbuffers(1, &mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, mWidth, mHeight);
	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextrueId, 0);
	//bind depth render buffer 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}
	if (!mapBuffer())
		return;
	glBindFramebuffer(GL_FRAMEBUFFER,  mOldFrameId );
}

void EgFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
	glViewport(0, 0, mWidth, mWidth);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EgFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mOldFrameId);

}

void EgFrameBuffer::clear()
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

	_uvBuffer = UINT_MAX;

	if (_indexBuffer != UINT_MAX)
	{
		glDeleteBuffers(1, &_indexBuffer);
	}
	_uvBuffer = UINT_MAX;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void EgFrameBuffer::show(ProgramData *proData, int x, int y, int w, int h)
{
	if (mOldFrameId != 0)
	{
		CCLOG("m_screenbuff is not 0 ");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, mOldFrameId);
	proData->program.use();

	glViewport(x, y, w, h);
	kmMat4 orthoMatrix;
	kmMat4Identity(&orthoMatrix);
	kmMat4OrthographicProjection(&orthoMatrix, 0, mWidth, 0, mHeight, -1024, 1024);
	glUniformMatrix4fv(proData->mUinform[enUni_transformMatrix], 1, GL_FALSE, orthoMatrix.mat);

	//kmMat4 matrixP;
	//kmMat4 matrixMV;
	//kmMat4 matrixMVP;
	//kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	//kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	//kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	////kmMat4Identity(&matrixMVP);
	//glUniformMatrix4fv(proData->mUinform[enUni_transformMatrix], 1, GL_FALSE, matrixMVP.mat);	

	glBindTexture(GL_TEXTURE_2D, mTextrueId);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	//glUniformMatrix4fv(shader->gvViewTransMattixHandle, 1, false, MatrixManager::getScreenBufferViewMatrix());

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(enAtt_position, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer);
	glVertexAttribPointer( enAtt_textureCoordinate, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool EgFrameBuffer::mapBuffer()
{
	GLuint _buffers[3];
	glGenBuffers(3, _buffers);
	for (int i = 0; i < 3; i++)
	{
		if (!_buffers[i] && _buffers[i] == UINT_MAX)
		{
			return false;
		}
	}
	_vertexBuffer = _buffers[0];
	_uvBuffer = _buffers[1];
	_indexBuffer = _buffers[2];

	GLfloat vertexArray[] =
	{
		0.0f, mWidth, 0.0f,
		mWidth, mHeight, 0.0f,
		0.0f, 0.0f, 0.0f,
		mWidth, 0.0f, 0.0f,

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

	// bind and upload the index
	GLushort indexis[] = { 0, 1, 2, 1, 3, 2 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexis[0]) * 6, indexis, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void EgFrameBuffer::resize(GLuint width, GLuint height)
{
	if (this->mWidth == width && this->mHeight == height)
	{
		return;
	}

	mWidth = width;
	mHeight = height;
	//set texture buffer size
	glBindTexture(GL_TEXTURE_2D, this->mTextrueId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->mWidth, this->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//set depth buffer

	glBindRenderbuffer(GL_RENDERBUFFER, this->mRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, this->mWidth, this->mHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return;

}

