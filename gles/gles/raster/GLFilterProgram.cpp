#include "GLFilterProgram.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include <stdlib.h>
#include "cclog.h"

// extern
#include "kazmath/GL/matrix.h"
#include "kazmath/kazmath.h"

//GLint gUniforms[kEGUni_MAX];


GLFilterProgram::GLFilterProgram()
: m_uProgram(0)
, m_uVertShader(0)
, m_uFragShader(0)
{
    
}

GLFilterProgram::~GLFilterProgram()
{
    if (m_uProgram) 
    {
        glDeleteProgram(m_uProgram);
    }
}

bool GLFilterProgram::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
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

bool GLFilterProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
		CCLOG("egret: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLOG("egret: vertexShaderL" );
        }
        else
        {
            CCLOG("egret: fragmentShaderL");
        }
        free(src);

        abort();
    }
    return (status == GL_TRUE);
}

void GLFilterProgram::BindAttributeLocation(const char* attributeName, GLuint index)
{
    glBindAttribLocation(m_uProgram, index, attributeName);
}

void GLFilterProgram::updateUniforms()
{
	//gUniforms[kEGUniFillColor] = glGetUniformLocation( m_uProgram, kEGUniformFillColor_s );
	//gUniforms[kEGUniMVPMatrix] = glGetUniformLocation(m_uProgram, kEGUniformMVPMatrix_s);
	//gUniforms[kEGUniTexMatrix] = glGetUniformLocation( m_uProgram, kEGUniformTexMatrix_s );
    //gUniforms[kEGUniTexture0] = glGetUniformLocation(m_uProgram, kEGUniformTextrue0_s);
	//gUniforms[kEGUniDrawType] = glGetUniformLocation( m_uProgram, kEGUniformDrawType_s );
	

    //this->use();
    
    // Since sample most probably won't change, set it to 0 now.
	//glUniform1i( (GLint)gUniforms[kEGUniTexture0],  0 );
	//glUniform1i( (GLint)gUniforms[kEGUniDrawType], 1 );
}

bool GLFilterProgram::link()
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
    return (status == GL_TRUE);
}

void GLFilterProgram::use()
{
    glUseProgram(m_uProgram);
}

// Uniform cache

bool GLFilterProgram::updateUniformLocation(GLint location, GLvoid* data, unsigned int bytes)
{
    if (location < 0)
    {
        return false;
    }
    
    bool updated = true;

    return updated;
}

GLint GLFilterProgram::getUniformLocationForName(const char* name)
{
    return glGetUniformLocation(m_uProgram, name);
}

void GLFilterProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
    bool updated =  updateUniformLocation(location, &i1, sizeof(i1)*1);
    
    if( updated )
    {
        glUniform1i( (GLint)location, i1);
    }
}

void GLFilterProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
    GLint ints[2] = {i1,i2};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform2i( (GLint)location, i1, i2);
    }
}

void GLFilterProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
    GLint ints[3] = {i1,i2,i3};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform3i( (GLint)location, i1, i2, i3);
    }
}

void GLFilterProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    GLint ints[4] = {i1,i2,i3,i4};
    bool updated =  updateUniformLocation(location, ints, sizeof(ints));
    
    if( updated )
    {
        glUniform4i( (GLint)location, i1, i2, i3, i4);
    }
}

void GLFilterProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*2*numberOfArrays);
    
    if( updated )
    {
        glUniform2iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLFilterProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*3*numberOfArrays);
    
    if( updated )
    {
        glUniform3iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLFilterProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, ints, sizeof(int)*4*numberOfArrays);
    
    if( updated )
    {
        glUniform4iv( (GLint)location, (GLsizei)numberOfArrays, ints );
    }
}

void GLFilterProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
    bool updated =  updateUniformLocation(location, &f1, sizeof(f1)*1);

    if( updated )
    {
        glUniform1f( (GLint)location, f1);
    }
}

void GLFilterProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
    GLfloat floats[2] = {f1,f2};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform2f( (GLint)location, f1, f2);
    }
}

void GLFilterProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    GLfloat floats[3] = {f1,f2,f3};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform3f( (GLint)location, f1, f2, f3);
    }
}

void GLFilterProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLfloat floats[4] = {f1,f2,f3,f4};
    bool updated =  updateUniformLocation(location, floats, sizeof(floats));

    if( updated )
    {
        glUniform4f( (GLint)location, f1, f2, f3,f4);
    }
}

void GLFilterProgram::setUniformLocationWith2fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*2*numberOfArrays);

    if( updated )
    {
        glUniform2fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLFilterProgram::setUniformLocationWith3fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*3*numberOfArrays);

    if( updated )
    {
        glUniform3fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}

void GLFilterProgram::setUniformLocationWith4fv(GLint location, GLfloat* floats, unsigned int numberOfArrays)
{
    bool updated =  updateUniformLocation(location, floats, sizeof(float)*4*numberOfArrays);

    if( updated )
    {
        glUniform4fv( (GLint)location, (GLsizei)numberOfArrays, floats );
    }
}


void GLFilterProgram::setUniformLocationWithMatrix4fv(GLint location, GLfloat* matrixArray, unsigned int numberOfMatrices)
{
    bool updated =  updateUniformLocation(location, matrixArray, sizeof(float)*16*numberOfMatrices);

    if( updated )
    {
        glUniformMatrix4fv( (GLint)location, (GLsizei)numberOfMatrices, GL_FALSE, matrixArray);
    }
}

void GLFilterProgram::setMatrixValue( kmMat4 *matTrans )
{
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	kmMat4Multiply( &matrixMVP, &matrixMVP, matTrans);
    //setUniformLocationWithMatrix4fv(gUniforms[kEGUniMVPMatrix], matrixMVP.mat, 1);
}


