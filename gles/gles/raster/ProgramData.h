#ifndef __PROGRAMDATA_H__
#define __PROGRAMDATA_H__

#include "GLFilterProgram.h"
#include "cctypes.h"

enum PROGRAMRTYPE
{
	enFilter_BLURH = 0,
	enFilter_BLURV,
	enFilter_COLOR,
	enFilter_ALPHA,
	enFilter_MULTIPLY,
	enFilter_IDENTITY,
	enFilter_COUNT
};

enum
{
	enUni_resolution = 0,
	enUni_flipY,
	enUni_time,
	enUni_transformMatrix,
	enUni_image,
	enUni_color,
	enUni_textureSize,
	enUni_colorMatrix,
	enUni_vector,
	enUni_Count,
};

#define	enUni_resolution_s		"u_resolution"
#define enUni_flipY_s			"u_flipY" 
#define enUni_time_s			"u_time"
#define enUni_transformMatrix_s	"u_transformMatrix"
#define enUni_image_s			"u_image"
#define enUni_color_s			"u_color"
#define enUni_textureSize_s		"u_textureSize"
#define enUni_colorMatrix_s		"u_colorMatrix"
#define enUni_vector_s			"u_vector"

enum
{
	enAtt_position,
	enAtt_textureCoordinate,
	enAtt_Count,
};

#define enAtt_position_s			"a_position"
#define enAtt_textureCoordinate_s	"a_textureCoordinate"


class ProgramData
{
public:
	ProgramData();
	GLFilterProgram program;
	GLuint mUinform[enUni_Count];
	const char *mFraBuffer;
};


#endif