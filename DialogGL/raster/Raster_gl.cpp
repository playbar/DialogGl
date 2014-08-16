#include "Raster_gl.h"
#include <math.h>
#include <list>

vector<GLdouble*> gVecPoint;

class oglScopeMatrix 
{
public:
	oglScopeMatrix(const kmMat3& m)
	{
		//glPushMatrix();
		//float mat[16];
		//memset(&mat[0], 0, sizeof(mat));
		//mat[0] = m.a / 65536.0f;
		//mat[1] = m.b / 65536.0f;
		//mat[4] = m.c / 65536.0f;
		//mat[5] = m.d / 65536.0f;
		//mat[10] = 1;
		//mat[12] = m.tx;
		//mat[13] = m.ty;
		//mat[15] = 1;
		//glMultMatrixf(mat);
	}
	~oglScopeMatrix()
	{
		glPopMatrix();
	}
};

CCPoint PtMiddle( CCPoint pt1, CCPoint pt2 )
{
	CCPoint pt;
	pt.x = ( pt1.x + pt2.x ) / 2;
	pt.y = ( pt1.y + pt2.y ) / 2;
	return pt;
}

void TraceCurve( CCPoint &startP, CCPoint&controlP, CCPoint &endP, 
				vector<oglVertex> &coords )
{
	CCPoint mid = PtMiddle( startP, endP );
	CCPoint q = PtMiddle( mid, controlP );
	if( sqrt( (float)((q.x - mid.x) * (q.x - mid.x) + (q.y - mid.y) * (q.y - mid.y )) ) < 0.1 )
	{
		coords.push_back( oglVertex( endP ) );
	}
	else
	{
		TraceCurve( startP, PtMiddle( startP, controlP), q, coords );
		TraceCurve( q, PtMiddle( controlP, endP), endP, coords );
	}
	return;
}

vector<oglVertex> BwInterpolate( vector<BwEdge> &edgs, float &anchor_x, float &anchor_y )
{
	CCPoint anchor( anchor_x, anchor_y );
	vector<oglVertex> shape_points;
	shape_points.push_back( oglVertex( anchor ) );
	for( vector<BwEdge>::iterator it = edgs.begin(), end = edgs.end();
		it != end; ++it )
	{
		BwEdge &the_edge = *it;
		CCPoint target( the_edge.mEndPt.x, the_edge.mEndPt.y );
		if( the_edge.IsLine( ) )
		{
			shape_points.push_back( oglVertex( target ) );
		}
		else
		{
			CCPoint control( the_edge.mControlPt.x, the_edge.mControlPt.y );
			TraceCurve( anchor, control, target, shape_points );
		}
		anchor = target;
	}
	return shape_points;
}

PathPtrVec paths_by_fill_style( PathVec &path_vec, unsigned int style )
{
	PathPtrVec paths;
	for( PathVec::iterator it = path_vec.begin(), end = path_vec.end();
		it != end; ++it )
	{
		BwPath &cur_path = *it;
		if( cur_path.mFillLeft == style )
		{
			paths.push_back( &cur_path );
		}
		if( cur_path.mFillRight == style )
		{
			paths.push_back( &cur_path );
		}
	}
	return paths;
}

PathPtrVec paths_by_line_style( PathVec &path_vec, unsigned int style )
{
	PathPtrVec paths;
	for( PathVec::iterator it = path_vec.begin(), end = path_vec.end();
		it != end; ++it )
	{
		BwPath &cur_path = *it;
		if( cur_path.mLineStyle == style )
		{
			paths.push_back( &cur_path );
		}
	}
	return paths;
}

BwPath *find_connecting_path( BwPath &to_connect, list<BwPath*> path_refs )
{
	float target_x = to_connect.mVecEdges.back().mEndPt.x;
	float target_y = to_connect.mVecEdges.back().mEndPt.y;

	if( target_x == to_connect.mStartPt.x && target_y == to_connect.mStartPt.y )
	{
		return NULL;
	}
	for( list<BwPath*>::iterator it = path_refs.begin(), end = path_refs.end();
		it != end; ++it )
	{
		BwPath *cur_path = *it;
		if( cur_path == &to_connect )
		{
			continue;
		}
		if( cur_path->mStartPt.x == target_x && cur_path->mStartPt.y )
		{
			if( cur_path->mFillLeft != to_connect.mFillLeft )
			{
				continue;
			}
			return cur_path;
		}
	}
	return NULL;
}

list<PathPtrVec > get_contours( PathPtrVec &paths )
{
	list<BwPath *> path_refs;
	list<PathPtrVec > contours;

	for( PathPtrVec::iterator it = paths.begin(), end = paths.end();
		it != end; ++it )
	{
		BwPath *cur_path = *it;
		path_refs.push_back( cur_path );
	}
	list< BwPath *>::iterator ittmp;
	for( list< BwPath *>::iterator it = path_refs.begin(), end = path_refs.end();
		it != end; ++it )
	{
		ittmp = it;
		BwPath *cur_path = *it;
		if( cur_path->mVecEdges.empty() )
		{
			continue;
		}
		PathPtrVec contour;
		contour.push_back( cur_path );
		BwPath *connector = find_connecting_path( *cur_path, path_refs );
		while( connector )
		{
			contour.push_back( connector );
			BwPath *tmp = connector;
			ittmp++;
			connector = find_connecting_path( *connector, list<BwPath*>(ittmp, end ) );
			path_refs.remove( tmp );
		}
		contours.push_back( contour );
	}
	return contours;
}

//////////////////////////////////////////////////////////////////////////
//
//
//Tesselator::Tesselator() : _tessobj(gluNewTess())
//{
//	gluTessCallback(_tessobj, GLU_TESS_ERROR, 
//		 (void (__stdcall *)())(Tesselator::error));
//	gluTessCallback(_tessobj, GLU_TESS_COMBINE_DATA,
//		 (void (__stdcall *)())(Tesselator::combine));
//
//	gluTessCallback(_tessobj, GLU_TESS_BEGIN,
//		 (void (__stdcall *)())(glBegin));
//	gluTessCallback(_tessobj, GLU_TESS_END,
//		 (void (__stdcall *)())(glEnd));
//
//	gluTessCallback(_tessobj, GLU_TESS_VERTEX,
//		 (void (__stdcall *)())(glVertex3dv)); 
//
//#if 0        
//	// for testing, draw only the outside of shapes.          
//	gluTessProperty(_tessobj, GLU_TESS_BOUNDARY_ONLY, GL_TRUE);
//#endif
//
//	// all coordinates lie in the x-y plane
//	// this speeds up tesselation 
//	gluTessNormal(_tessobj, 0.0, 0.0, 1.0);
//}
//
//Tesselator::~Tesselator()
//{
//	gluDeleteTess(_tessobj);  
//}
//
//void Tesselator::beginPolygon()
//{
//	gluTessBeginPolygon(_tessobj, this);
//}
//
//void Tesselator::beginContour()
//{
//	gluTessBeginContour(_tessobj);
//}
//
//void Tesselator::feed(std::vector<oglVertex>& vertices)
//{
//	for (std::vector<oglVertex>::const_iterator it = vertices.begin(), end = vertices.end();
//		it != end; ++it) 
//	{
//			GLdouble* vertex = const_cast<GLdouble*>(&(*it)._x);
//			gluTessVertex(_tessobj, vertex, vertex);
//	}
//}
//
//void Tesselator::endContour()
//{
//	gluTessEndContour(_tessobj);  
//}
//
//void Tesselator::tesselate()
//{
//	gluTessEndPolygon(_tessobj);
//
//	for (std::vector<GLdouble*>::iterator it = gVecPoint.begin(),
//		end = gVecPoint.end(); it != end; ++it) 
//	{
//			delete [] *it;
//	}
//
//	gVecPoint.clear();
//}
//
//// static
//void Tesselator::error(GLenum error)
//{  
//	//log_error(_("GLU: %s"), gluErrorString(error));
//}
//
//// static
//void Tesselator::combine(GLdouble coords [3], void *vertex_data[4],
//						 GLdouble weight[4], void **outData)
//{
//	GLdouble* v = new GLdouble[3];
//	v[0] = coords[0];
//	v[1] = coords[1];
//	v[2] = coords[2];
//	*outData = v;
//	gVecPoint.push_back(v);
//}

//////////////////////////////////////////////////////////////////////////


RasterGL::RasterGL()
{
	mpSt9Patch = NULL;
	mAntiShift = 0;

}

RasterGL::~RasterGL()
{

}

void RasterGL::Init(int width, int height )
{

}

void RasterGL::Uninit()
{

}

void RasterGL::GetCacheData( unsigned char *pdata, int width, int height )
{

}

void RasterGL::GlesDrawShape( BwShapeRecord & rec, kmMat3 &matrix, ccColor4F &cx )
{
	//BwglDrawLine(0, 0, 100, 100, 2, 0xFF00FF00 );
	//return;
	ShowAllPt( rec );
	//BwglClear();
	//GLDrawLine(20, 20, 100, 100, 2, 0xFF00FF00 );
	//return;
	vector<BwPath>& paths = rec.mVecPaths;
	bool haveShape =false;
	bool haveOutline = false;
	AnalyzePath( paths, haveShape, haveOutline );
	if( !haveOutline && !haveShape )
		return;

	vector< vector<BwPath>::iterator > subShapes = FindSubShape( paths );
	vector< BwFillStyle *> &fillStyle = rec.mVecFillStyles;
	vector< BwLineStyle > &lineStyle = rec.mVecLineStyles;

	for( int i = 0; i < subShapes.size() - 1; ++i )
	{
		vector<BwPath> subShapePaths;
		if( subShapes[i] != subShapes[i + 1 ])
		{
			subShapePaths = vector<BwPath>( subShapes[i], subShapes[i + 1 ]);
		}
		else
		{
			subShapePaths.push_back( *subShapes[i]);
		}
		DrawSubShape( subShapePaths, matrix,cx, fillStyle, lineStyle );
	}
	//GLFlush();
	return;

//////////////////////////////////////////////////////////////////////////

	vector<BwPath>::iterator it;
	for( it = rec.mVecPaths.begin(); it != rec.mVecPaths.end(); it++ )
	{
		GlesDrawSubShape( *it, matrix, cx, rec.mVecFillStyles, rec.mVecLineStyles );
	}
	return;
}

void RasterGL::ShowAllPt( BwShapeRecord & rec )
{
	char pTmp[256];
	memset(pTmp, 0, 256 );
	int ipathnum = 0;
	int iedgenum = 0;
	PathVec::iterator iter = rec.mVecPaths.begin();
	PathVec::iterator final = rec.mVecPaths.end();
	for( ; iter != final; iter++ )
	{
		iedgenum = 0;
		ipathnum++;
		BwPath &path = *iter;
		memset(pTmp, 0, 256 );
		sprintf(pTmp, "Path num-->%d, StartPt->x:%d, y:%d", ipathnum, path.mStartPt.x, path.mStartPt.y );
		OutputDebugStringA( pTmp );
		vector< BwEdge >::iterator it;
		for( it = path.mVecEdges.begin(); it != path.mVecEdges.end(); it++ )
		{
			iedgenum++;
			BwEdge &edge = *it;
			memset(pTmp, 0, 256 );
			sprintf(pTmp, "Path edge---->%d:%d, ControlPt->x:%d, y:%d, EndPt->x:%d, y:%d", 
				ipathnum, iedgenum, edge.mControlPt.x, edge.mControlPt.y, edge.mEndPt.x, edge.mEndPt.y );
			OutputDebugStringA( pTmp );
		}
	}
	return;
}

void RasterGL::GlesDrawSubShape( BwPath &pPath, kmMat3 &matrix, ccColor4F &cx, vector<BwFillStyle*> &fillStyle,
				  vector<BwLineStyle> &lineStyle )
{
	char pTmp[256];
	memset(pTmp, 0, 256 );
	int ilen = pPath.mVecEdges.size() * 3 + 3;
	GLdouble *pData = new GLdouble[ilen];
	memset( pData, 0, sizeof( GLdouble) * ilen );
	pData[0] = pPath.mStartPt.x; //  mAntiShift;
	pData[1] = pPath.mStartPt.y;// >> mAntiShift;
	sprintf(pTmp, "s.x:%f, s.y:%f", pData[0], pData[1] );
	OutputDebugStringA( pTmp );
	vector< BwEdge >::iterator it;
	int pos = 3;
	for( it = pPath.mVecEdges.begin(); it != pPath.mVecEdges.end(); it++ )
	{
		pData[pos] =(*it).mEndPt.x;// >> mAntiShift;
		pos++;
		pData[pos] = (*it).mEndPt.y;// >> mAntiShift;
		sprintf(pTmp, "pt:%d,x:%f, y:%f", pos / 3, pData[pos - 1], pData[pos] );
		OutputDebugStringA( pTmp );
		pos++;
		pos++;
	}
	
	unsigned long color = 0xFF0000FF;
	GLenum mode = GL_LINE_STRIP;
	if( pPath.LineStyle() > 0 )
	{
		lineStyle[pPath.LineStyle()].BeginLineStyle();
		//color = lineStyle[pPath->LineStyle()]->LineColor().nColor;
	}
	if( pPath.FillRight( ) > 0 )
	{
		mode = GL_TRIANGLE_STRIP;
		fillStyle[pPath.FillRight()]->BeginFillStyle();
	}
	//GLDrawLines( mode, pData, ilen / 3, 1 );
	delete []pData;
	return;
}

void RasterGL::AnalyzePath( vector<BwPath> &paths, bool &haveShape, bool &haveOutline )
{
	haveShape = false;
	haveOutline = false;
	int cou = paths.size();
	for( int i = 0; i < cou; i++ )
	{
		const BwPath&thePath = paths[i];
		if( (thePath.mFillLeft > 0) || (thePath.mFillRight > 0 ))
		{
			haveShape = true;
			if( haveOutline )
				return;
		}
		if( thePath.mLineStyle > 0 )
		{
			haveOutline = true;
			if( haveShape )
				return;
		}
	}
}

vector< vector<BwPath>::iterator > RasterGL::FindSubShape( vector<BwPath> &paths )
{
	vector< vector<BwPath>::iterator > subShapes;
	vector<BwPath>::iterator it = paths.begin(),
							 end = paths.end();
	subShapes.push_back( it );
	++it;
	for( ; it != end; ++it )
	{
		BwPath &curPath = *it;
		if( curPath.mbNewShape )
		{
			subShapes.push_back( it );
		}
	}
	if( subShapes.back() != end )
	{
		subShapes.push_back( end );
	}
	return subShapes;
}



void RasterGL::DrawSubShape( vector<BwPath>&paths, kmMat3 &mat, ccColor4F &cx, vector<BwFillStyle*>&fillStyle,
				  vector<BwLineStyle>&lineStyle )
{
	PathVec normalized = NormalizePaths( paths );
	PathPointMap pathpoints = GetPathPoints( normalized );

	for( int i = 0; i < fillStyle.size(); ++i )
	{
		PathPtrVec paths = paths_by_fill_style( normalized, i + 1 );
		if( paths.size() == 0 )
		{
			continue;
		}
		BwFillStyle *fs = fillStyle[ i + 1 ];
		list<PathPtrVec > contours = get_contours( paths );
		for( list< PathPtrVec>::iterator iter = contours.begin(), final = contours.end();
			iter != final; ++iter )
		{
			PathPtrVec &refs = *iter;
			//_tesselator.beginContour();

			for( PathPtrVec::iterator it = refs.begin(), end = refs.end();
				it != end; ++it )
			{
				BwPath &cur_path = *(*it);
				//_tesselator.feed( pathpoints[&cur_path] );
				//vector<oglVertex> &pData = pathpoints[&cur_path];
				//fs->BeginFillStyle();
				//GLDrawLines( GL_TRIANGLE_FAN, &(pData[0].mX), pData.size(), 1 );
				//GLDrawTexture( GL_TRIANGLES, &(pData[0].mX), pData.size() );
				//GLDrawTexture1( GL_TRIANGLES, &(pData[0].mX), pData.size(), fs->TextureID());
			}
			//_tesselator.endContour();
		}
		fs->BeginFillStyle();

		//_tesselator.tesselate();
	}

	draw_outline( normalized, pathpoints, mat, cx, lineStyle );
	//////////////////////////////////////////////////////////////////////////
	
	//for( int i = 0; i < lineStyle.size(); ++i )
	//{
	//	PathPtrVec paths = paths_by_line_style( normalized, i + 1 );
	//	if( paths.size() == 0 )
	//	{
	//		continue;
	//	}
	//	BwLineStyle &ls = lineStyle[i + 1];
	//	list<PathPtrVec > contours = get_contours( paths );
	//	for( list< PathPtrVec>::iterator iter = contours.begin(), final = contours.end();
	//		iter != final; ++iter )
	//	{
	//		PathPtrVec &refs = *iter;
	//		for( PathPtrVec::iterator it = refs.begin(), end = refs.end();
	//			it != end; ++it )
	//		{
	//			BwPath &cur_path = *(*it);
	//			vector<oglVertex> &pData = pathpoints[&cur_path];
	//			//for( int i = 0; i < pData.size(); i++ )
	//			//{
	//			//	char chTmp[256];
	//			//	memset( chTmp, 0, 256 );
	//			//	sprintf( chTmp, "--------->num:%d, x=%f, y=%f", i, pData[i].mX, pData[i].mY );
	//			//	OutputDebugStringA( chTmp );
	//			//}
	//			
	//			//GLApplyStyle( 0xFF00FF00, 2 );
	//			ls.BeginLineStyle();
	//			//BwglDrawLines( GL_LINE_STRIP, &(pData[0].mX), pData.size(), 1 );
	//		}
	//	}
	//	//ls.EndLineStyle();
	//}
	return;
}

void RasterGL::draw_outline( vector<BwPath>&path_vec, PathPointMap& pathpoints, kmMat3 &mat,
							ccColor4F &cx, vector<BwLineStyle>&lineStyle)
{
	for( PathVec::iterator it = path_vec.begin(), end = path_vec.end();
		it != end; ++it )
	{
		BwPath &cur_path = *it;
		if( !cur_path.mLineStyle )
		{
			continue;
		}
		bool draw_points = apply_line_style(lineStyle[cur_path.mLineStyle ], mat, cx );
		vector<oglVertex> &shape_points = (*pathpoints.find(&cur_path)).second;

		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_DOUBLE, 0, &shape_points.front() );
		glDrawArrays( GL_LINE_STRIP, 0, shape_points.size() );
		glEnableClientState( GL_VERTEX_ARRAY );

		if( !draw_points )
			continue;

		glEnable( GL_POINT_SMOOTH );
		glBegin( GL_POINTS );
		{
			glVertex2d( cur_path.mStartPt.x, cur_path.mStartPt.y );
			for( vector<BwEdge>::iterator it = cur_path.mVecEdges.begin(), end = cur_path.mVecEdges.end();
				it != end; it++ )
			{
				BwEdge &cur_edge = *it;
				glVertex2d( cur_edge.mEndPt.x, cur_edge.mEndPt.y );
			}
		}
		glEnd();
	}
}

bool RasterGL::apply_line_style( BwLineStyle &style, kmMat3 &mat, ccColor4F &cx )
{
	glDisable( GL_TEXTURE_2D );
	bool rv = true;
	float width = style.LineWidth();
	if( !width )
	{
		glLineWidth( 1.0f );
		rv = false;
	}
	else if( (! style.mbNoVScaleFlag ) &&( ! style.mbNoVScaleFlag ) )
	{
		glLineWidth( width );
		glPointSize( width );
	}
	else 
	{
		glLineWidth( width );
		glPointSize( width );
	}
	glColor4ub( style.mColor.b, style.mColor.g, style.mColor.r, style.mColor.a );
	//glColor3f( 0.5, 1.0f, 0.0f ) ; 
	return rv;
}

PathPointMap RasterGL::GetPathPoints( PathVec &path_vec )
{
	PathPointMap pathpoints;
	for( PathVec::iterator it = path_vec.begin(), end = path_vec.end();
		it != end; ++it )
	{
		BwPath &cur_path = *it;
		if( cur_path.mVecEdges.size() == 0 )
		{
			continue;
		}
		pathpoints[ &cur_path ] = BwInterpolate( cur_path.mVecEdges, 
			cur_path.mStartPt.x, cur_path.mStartPt.y );
	}
	return pathpoints;
}

vector<BwPath> RasterGL::NormalizePaths( vector<BwPath>&paths )
{
	vector<BwPath> normalized;
	vector<BwPath>::iterator it = paths.begin(),
							end = paths.end();
	for( ; it != end; ++it )
	{
		BwPath &curPath = *it;
		if( curPath.mVecEdges.empty() )
		{
			continue;
		}
		else if( curPath.mFillLeft && curPath.mFillRight )
		{
			normalized.push_back( curPath );
			normalized.back().mFillLeft = 0;
			BwPath newPath = ReversePath( curPath );
			newPath.mFillLeft = 0;
			normalized.push_back( newPath );
		}
		else if( curPath.mFillLeft )
		{
			BwPath newPath = ReversePath( curPath );
			newPath.mFillLeft = 0;
			normalized.push_back( newPath );
		}
		else if( curPath.mFillRight )
		{
			normalized.push_back( curPath );
		}
		else
		{
			normalized.push_back( curPath );
		}
	}
	return normalized;
}

BwPath RasterGL::ReversePath( BwPath &curPath )
{
	BwEdge &cur_end = curPath.mVecEdges.back();
	float prev_cx = cur_end.mControlPt.x;
	float prev_cy = cur_end.mControlPt.y;
	BwPath newPath( cur_end.mEndPt.x, cur_end.mEndPt.y, curPath.mFillRight, curPath.mFillLeft,
		curPath.mLineStyle, curPath.mbNewShape );
	float prev_ax = cur_end.mEndPt.x;
	float prev_ay = cur_end.mEndPt.y;

	for( vector<BwEdge>::reverse_iterator it = curPath.mVecEdges.rbegin() + 1,
		end = curPath.mVecEdges.rend(); it != end; ++it )
	{
		BwEdge &curEdge = *it;
		if( prev_ax == prev_cx && prev_ay == prev_cy )
		{
			prev_cx = curEdge.mEndPt.x;
			prev_cy = curEdge.mEndPt.y;
		}
		BwEdge newEdge( prev_cx, prev_cy, curEdge.mEndPt.x, curEdge.mEndPt.y );
		newPath.mVecEdges.push_back( newEdge );

		prev_cx = curEdge.mControlPt.x;
		prev_cy = curEdge.mControlPt.y;
		prev_ax = curEdge.mEndPt.x;
		prev_ay = curEdge.mEndPt.y;
	}
	BwEdge newLastEdge( prev_cx, prev_cy, curPath.mStartPt.x, curPath.mStartPt.y );
	newPath.mVecEdges.push_back( newLastEdge );
	return newPath;
}

void RasterGL::MaskBegin()
{

}

void RasterGL::MaskAddPath( BwPath *path )
{

}
void RasterGL::MaskEnd()	//apply mask
{

}

void RasterGL::EnableBlend( ENBlendMode mode )
{

}
void RasterGL::DisableBlend()
{

}

void RasterGL::EnalbeBitmapFilter( ENBitmapFilter filter )
{

}
void RasterGL::DisableBitmapFilter()
{

}

void RasterGL::Begin9Patch(ST9Patch * patch )
{

}
void RasterGL::End9Patch()
{

}

void RasterGL::DrawOffScreen( bool bOff )
{

}

void RasterGL::QualityAntiShift( int antiShift )
{
	mAntiShift = antiShift;
}

int RasterGL::QualityAntiShift()
{
	return mAntiShift;
}


