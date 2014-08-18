#include "BwShapeRecord.h"
#include <Windows.h>

Edge::Edge()
{
	memset( &cp, 0, sizeof( CCPoint ));
	memset( &ap, 0, sizeof( CCPoint ));
	mbLine = 0;
}

Edge::Edge( int cx, int cy, int ax, int ay)
{
	cp.x = cx;
	cp.y = cy;
	ap.x = ax;
	ap.y = ay;
}

Edge::~Edge()
{

}

void Edge::IsLine(int bline )
{
	mbLine = bline;
	return;
}

int Edge::IsLine()
{
	return mbLine;
	//return mControlPt == mEndPt;
}

void Edge::SetControlPt( CCPoint &pt )
{
	cp = pt;
}

void Edge::SetEndPt( CCPoint &pt )
{
	ap = pt;
}

bool Edge::straight()
{
	return cp == ap;
}

//////////////////////////////////////////////////////////////////////////

Path::Path()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
	mFill0 = 0;
	mFill1 = 0;
	mLine = 0;
	mbNewShape = false;
}

Path::Path( int ax, int ay, int fillLeft, int fillRight, int line, bool bNewShape)
{
	mStartPt.x = ax;
	mStartPt.y = ay;
	mFill0 = fillLeft;
	mFill1 = fillRight;
	mLine = line;
	mbNewShape = bNewShape;
	mVecEdges.resize( 0 );
	return;
}

Path::~Path()
{
	ReleaseEdge();
}

void Path::AddEdge(Edge &pedge )
{
	mVecEdges.push_back( pedge );
	return;
}

void Path::StartPt( CCPoint &pt )
{
	//mStartPt = pt;
	mStartPt.x = pt.x;
	mStartPt.y = pt.y;
	return;
}

void Path::EmptyStartPt()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
}

bool Path::IsStartPtEmpty()
{
	bool b = (mStartPt.x == 0 && mStartPt.y == 0);
	return b;
}

void Path::ClearEdges()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
	mVecEdges.clear();
	mVecEdges.resize( 0 );
	return;
}

bool Path::EdgeEmpty()
{
	return mVecEdges.empty();
}

int Path::PtInPath(CCPoint &pt )
{
	if( mStartPt == pt )
		return en_PtPos_Begin;
	else if( mVecEdges.back().ap == pt )
		return en_PtPos_End;
	else 
	{
		vector< Edge >::iterator it = mVecEdges.begin();
		vector< Edge >::iterator end = mVecEdges.end();
		for( ; it != end; it++ )
		{
			Edge &edge = *it;
			if( edge.ap == pt )
				return en_PtPos_Mid;
		}
	}
	return en_PtPos_No;
}

CCPoint &Path::StartPt()
{
	return mStartPt;
}

void Path::FillLeft( int fillleft )
{
	mFill0 = fillleft;
}

int Path::FillLeft( )
{
	return mFill0;
}

void Path::FillRight( int fillRight )
{
	mFill1 = fillRight;
}
int Path::FillRight()
{
	return mFill1;
}

void Path::LineStyle(int ls )
{
	mLine = ls;
}

int Path::LineStyle()
{
	return mLine;
}

void Path::ReleaseEdge()
{
	//vector<BwEdge*>::iterator it;
	//for( it = mVecEdges.begin(); it != mVecEdges.end(); it++ )
	//{
	//	delete *it;
	//}
	mVecEdges.clear();
	return;
}

BwShapeRecord::BwShapeRecord()
{
	//mClipPath;
}

BwShapeRecord::~BwShapeRecord()
{
	ReleaseAll();
}

void BwShapeRecord::AddFillSyle( FillStyle *fs )
{
	mFillStyles.push_back( fs );
}

void BwShapeRecord::AddLineStyle( LineStyle &ls )
{
	mLineStyles.push_back( ls );
}

void BwShapeRecord::AddPath( Path &path )
{
	mPaths.push_back( path );
}

void BwShapeRecord::ReleaseFillStyle()
{
	vector<FillStyle *>::iterator it;
	for( it = mFillStyles.begin(); it != mFillStyles.end(); it++ )
	{
		delete *it;
	}
	mFillStyles.clear();
	mFillStyles.resize( 0 );
}

void BwShapeRecord::ReleaseLineStyle()
{
	//vector< BwLineStyle *>::iterator it;
	//for( it = mVecLineStyles.begin(); it != mVecLineStyles.end(); it++ )
	//{
	//	delete *it;
	//}
	mLineStyles.clear();
	mLineStyles.resize( 0 );
	return;
}

void BwShapeRecord::ReleasePaths()
{
	//vector<BwPath*>::iterator it;
	//for( it = mVecPaths.begin(); it != mVecPaths.end(); it++ )
	//{
	//	delete *it;
	//}
	mPaths.clear();
	mPaths.resize( 0 );
	return;
}

void BwShapeRecord::ReleaseAll()
{
	OutputDebugStringA("----->Release All ShapeRecord");
	ReleaseFillStyle();
	ReleaseLineStyle();
	ReleasePaths();
}

Path &BwShapeRecord::GetPath(int index )
{
	return mPaths[index];
}
