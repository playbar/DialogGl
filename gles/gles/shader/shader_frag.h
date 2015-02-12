"																\n\
#ifdef GL_ES													\n\
// #extension GL_OES_standard_derivatives : enable				\n\																													\n\
varying mediump vec4 v_color;									\n\
varying mediump vec2 v_texcoord;								\n\
#else															\n\
varying vec4 v_color;											\n\
varying vec2 v_texcoord;										\n\
#endif															\n\
																\n\
void main()														\n\
{		\n\
	if( u_drawType == 0 ) \n\
	{ \n\
		gl_FragColor = v_color*step(0.0, 1.0 - length(v_texcoord));	\n\
	}\n\
	else if( u_drawType == 1 ) \n\
	{ \n\
		gl_FragColor = CC_FillColor;\n\
	} \n\
	else if( u_drawType == 4 ) \n\
	{ \n\
		gl_FragColor = texture2D( CC_Texture0, v_texcoord ); \n\
		//gl_FragColor = vec4( 1, 0, 0, 1 ); \n\
	} \n\
	else if( u_drawType == 5 ) \n\
	{ \n\
		gl_FragColor = texture2D( CC_Texture0, v_texcoord ); \n\
		//gl_FragColor = vec4( 1, 0, 0, 1 ); \n\
	} \n\
}																\n\
";
