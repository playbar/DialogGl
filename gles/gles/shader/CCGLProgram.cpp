/****************************************************************************
Copyright 2012 cocos2d-x.org
Copyright 2011 Jeff Lamarche
Copyright 2012 Goffredo Marocchi
Copyright 2012 Ricardo Quesada

http://www.cocos2d-x.org
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

//#include "CCDirector.h"
#include "CCGLProgram.h"
#include "ccGLStateCache.h"
#include "cclog.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include "CCString.h"
#include "CCFileUtils.h"


// extern
#include "kazmath/GL/matrix.h"
#include "kazmath/kazmath.h"
#include "ccMacros.h"


GLint gUniforms[kCCUniform_MAX];


CCGLProgram::CCGLProgram()
: m_uProgram(0)
, m_uVertShader(0)
, m_uFragShader(0)
, m_bUsesTime(false)
{
    memset(gUniforms, 0, sizeof(gUniforms));
}

CCGLProgram::~CCGLProgram()
{
    if (m_uProgram) 
    {
        ccGLDeleteProgram(m_uProgram);
    }
}

bool CCGLProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    m_uProgram = glCreateProgram();

    m_uVertShader = m_uFragShader = 0;

    if (vShaderByteArray)
    {
        if (!compileShader(&m_uVertShader, GL_VERTEX_SHADER, vShaderByteArray))
        {
            ;
        }
    }

    // Create and compile fragment shader
    if (fShaderByteArray)
    {
        if (!compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray))
        {
            ;
        }
    }

    if (m_uVertShader)
    {
        glAttachShader(m_uProgram, m_uVertShader);
    }

    if (m_uFragShader)
    {
        glAttachShader(m_uProgram, m_uFragShader);
    }
    

    return true;
}


const char* CCGLProgram::description()
{
    return CCString::createWithFormat("<CCGLProgram = %08X | Program = %i, VertexShader = %i, FragmentShader = %i>", this, m_uProgram, m_uVertShader, m_uFragShader)->getCString();
}

bool CCGLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
		CCLOG("cocos2d: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLOG("cocos2d: %s", vertexShaderLog());
        }
        else
        {
            CCLOG("cocos2d: %s", fragmentShaderLog());
        }
        free(src);

        abort();
    }
    return (status == GL_TRUE);
}

void CCGLProgram::addAttribute(const char* attributeName, GLuint index)
{
    glBindAttribLocation(m_uProgram, index, attributeName);
}

void CCGLProgram::updateUniforms()
{
    gUniforms[kCCUniformPMatrix] = glGetUniformLocation(m_uProgram, kCCUniformPMatrix_s);
	gUniforms[kCCUniformMVMatrix] = glGetUniformLocation(m_uProgram, kCCUniformMVMatrix_s);
	gUniforms[kCCUniformMVPMatrix] = glGetUniformLocation(m_uProgram, kCCUniformMVPMatrix_s);
    gUniforms[kCCUniformTexture0] = glGetUniformLocation(m_uProgram, kCCUniformTextrue0_s);
	gUniforms[kCCuniformDrawType] = glGetUniformLocation( m_uProgram, kCCUniformDrawType_s );

    this->use();
    
    // Since sample most probably won't change, set it to 0 now.
	glUniform1i( (GLint)gUniforms[kCCUniformTexture0],  0 );
	glUniform1i( (GLint)gUniforms[kCCuniformDrawType], 0 );
}

bool CCGLProgram::link()
{
    GLint status = GL_TRUE;
    
    glLinkProgram(m_uProgram);

    if (m_uVertShader)
    {
        glDeleteShader(m_uVertShader);
    }
    
    if (m_uFragShader)
    {
        glDeleteShader(m_uFragShader);
    }
    
    m_uVertShader = m_uFragShader = 0;
	
#if DEBUG
    glGetProgramiv(m_uProgram, GL_LINK_STATUS, &status);
	
    if (status == GL_FALSE)
    {
        CCLOG("cocos2d: ERROR: Failed to link program: %i", m_uProgram);
        ccGLDeleteProgram(m_uProgram);
        m_uProgram = 0;
    }
#endif
	
    return (status == GL_TRUE);
}

void CCGLProgram::use()
{
    ccGLUseProgram(m_uProgram);
}

const char* CCGLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc)
{
    GLint logLength = 0, charsWritten = 0;

    infoFunc(object, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength < 1)
        return 0;

    char *logBytes = (char*)malloc(logLength);
    logFunc(object, logLength, &charsWritten, logBytes);

    CCString* log = CCString::create(logBytes);

    free(logBytes);
    return log->getCString();
}

const char* CCGLProgram::vertexShaderLog()
{
    return this->logForOpenGLObject(m_uVertShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* CCGLProgram::fragmentShaderLog()
{
    return this->logForOpenGLObject(m_uFragShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

const char* CCGLProgram::programLog()
{
    return this->logForOpenGLObject(m_uProgram, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

// Uniform cache

bool CCGLProgram::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes)
{
    if (location < 0)
    {
        return false;
    }
    
    bool updated = true;

    return updated;
}

GLint CCGLProgram::getUniformLocationForName(const char* name)
{
    
    return glGetUniformLocation(m_uProgram, name);
}

void CCGLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    bool updated =  updateUniformLocation(location, &i1, sizeof(i1)*1);
    
    if( updated )
    {
        glUniform1i( (GLint)location, i1);
    }
}

void CCGLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
    GLint ints[2] = {i1,i2};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform2i( (GLint)location, i1, i2);
    }
}

void CCGLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
    GLint ints[3] = {i1,i2,i3};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform3i( (GLint)location, i1, i2, i3);
    }
}

void CCGLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    GLint ints[4] = {i1,i2,i3,i4};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform4i( (GLint)location, i1, i2, i3, i4);
    }
}

void CCGLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*2*numberOfArrays);
    
    if( updated )
    {
        glUniform2iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void CCGLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*3*numberOfArrays);
    
    if( updated )
    {
        glUniform3iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void CCGLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*4*numberOfArrays);
    
    if( updated )
    {
        glUniform4iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void CCGLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
    bool updated =  updateUniformLocation(location, &f1, sizeof(f1)*1);

    if( updated )
    {
        glUniform1f( (GLint)location, f1);
    }
}

void CCGLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
    GLfloat floats[2] = {f1,f2};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform2f( (GLint)location, f1, f2);
    }
}

void CCGLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    GLfloat floats[3] = {f1,f2,f3};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform3f( (GLint)location, f1, f2, f3);
    }
}

void CCGLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLfloat floats[4] = {f1,f2,f3,f4};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform4f( (GLint)location, f1, f2, f3,f4);
    }
}

void CCGLProgram::setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*2*numberOfArrays);

    if( updated )
    {
        glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void CCGLProgram::setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*3*numberOfArrays);

    if( updated )
    {
        glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void CCGLProgram::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*4*numberOfArrays);

    if( updated )
    {
        glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}


void CCGLProgram::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    bool updated =  updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

    if( updated )
    {
        glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void CCGLProgram::setMatrixValue()
{
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
    setUniformLocationWithMatrix4fv(gUniforms[kCCUniformPMatrix], matrixP.mat, 1);
    setUniformLocationWithMatrix4fv(gUniforms[kCCUniformMVMatrix], matrixMV.mat, 1);
    setUniformLocationWithMatrix4fv(gUniforms[kCCUniformMVPMatrix], matrixMVP.mat, 1);
	
}

void CCGLProgram::reset()
{
    m_uVertShader = m_uFragShader = 0;
    memset(gUniforms, 0, sizeof(gUniforms));
    m_uProgram = 0;
 
}

