
static const char vert_canvas[] =
"attribute vec3 a_position;"
"attribute vec2 a_textureCoordinate;"
"uniform vec2 u_resolution;"
"uniform float u_flipY;"
"uniform float u_time;"
"uniform mat4 u_transformMatrix;"
"varying vec2 v_texCoord;"
"void main() {"
//"	vec2 position = ((u_transformMatrix * vec4(a_position, 1.0)).xy / u_resolution) * 2.0 - 1.0;"
//"	position *= vec2(1.0, u_flipY);"
//"	gl_Position = vec4(vec3(position, 1.0), 1.0);"
"	gl_Position = u_transformMatrix * vec4(a_position, 1.0);"
"	v_texCoord = a_textureCoordinate;"
"}";


static const char frag_alpha[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"uniform vec4 u_color;"
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = u_color * texture2D(u_image, v_texCoord).a;"
"}";

static const char frag_blurh[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"uniform vec2 u_textureSize;"
"varying vec2 v_texCoord;"
"void main() {"
"	const int sampleRadius = 10;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2(1.0, 1.0) / u_textureSize;"
"	vec4 color = vec4(0, 0, 0, 0);"
"	for (int i = -sampleRadius; i <= sampleRadius; i++) {"
"		color += texture2D(u_image, v_texCoord + vec2(float(i) * one.x, 0));"
"	}"
"	color /= float(samples);"
"	gl_FragColor = color;"
"}";

static const char frag_blurv[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"uniform vec2 u_textureSize;"
"varying vec2 v_texCoord;"
"void main() {"
"	const int sampleRadius = 10;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2(1.0, 1.0) / u_textureSize;"
"	vec4 color = vec4(0, 0, 0, 0);"
"	for (int i = -sampleRadius; i <= sampleRadius; i++) {"
"		color += texture2D(u_image, v_texCoord + vec2(0, float(i) * one.y));"
"	}"
"	color /= float(samples);"
"	gl_FragColor = color;"
"}";

static const char frag_color[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"uniform mat4 u_colorMatrix;"
"uniform vec4 u_vector;"
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = u_colorMatrix * texture2D(u_image, v_texCoord) + u_vector;"
"}";

static const char frag_identity[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = texture2D(u_image, v_texCoord);"
//"	gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0 );"
"}";

static const char frag_multiply[] =
#ifndef WIN32
"precision mediump float;"
#endif
"uniform sampler2D u_image;"
"uniform vec4 u_color;"
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = texture2D(u_image, v_texCoord) * vec4(1.9, 1.0, 1.0, 1.0);"
"	gl_FragColor = texture2D(u_image, v_texCoord) * u_color;"
"}";