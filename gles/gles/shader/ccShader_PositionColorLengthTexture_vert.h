"																	\n\
#ifdef GL_ES														\n\
attribute mediump vec4 a_position;									\n\
attribute mediump vec2 a_texcoord;									\n\
attribute mediump vec4 a_color;										\n\
																	\n\
varying mediump vec4 v_color;										\n\
varying mediump vec2 v_texcoord;									\n\
																	\n\
#else																\n\
attribute vec4 a_position;											\n\
attribute vec2 a_texcoord;											\n\
attribute vec4 a_color;												\n\
																	\n\
varying vec4 v_color;												\n\
varying vec2 v_texcoord;											\n\
#endif																\n\
																	\n\
void main()															\n\
{																	\n\
	v_color = vec4(a_color.rgb * a_color.a, a_color.a);				\n\
	v_texcoord = a_texcoord;										\n\
																	\n\
	gl_Position = CC_MVPMatrix * a_position;						\n\
}																	\n\
";
