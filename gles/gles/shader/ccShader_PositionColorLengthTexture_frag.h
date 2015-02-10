"																															\n\
#ifdef GL_ES																												\n\
// #extension GL_OES_standard_derivatives : enable																			\n\
																															\n\
varying mediump vec4 v_color;																								\n\
varying mediump vec2 v_texcoord;																							\n\
#else																														\n\
varying vec4 v_color;																										\n\
varying vec2 v_texcoord;																									\n\
#endif																														\n\
																															\n\
void main()																													\n\
{																															\n\
// #if defined GL_OES_standard_derivatives																						\n\
	// gl_FragColor = v_color*smoothstep(0.0, length(fwidth(v_texcoord)), 1.0 - length(v_texcoord));							\n\
// #else																														\n\
	gl_FragColor = v_color*step(0.0, 1.0 - length(v_texcoord));																\n\
// #endif																														\n\
}																															\n\
";
