#include "ProgramData.h"



ProgramData::ProgramData()
{
	mFraBuffer = NULL;
	memset(mUinform, 0, sizeof(GLuint) * enUni_Count);
}

