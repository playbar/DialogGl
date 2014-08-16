#include "BwShapeRecord.h"
#include <Windows.h>

BwEdge::BwEdge()
{
	memset( &mControlPt, 0, sizeof( CCPoint ));
	memset( &mEndPt, 0, sizeof( CCPoint ));
	mbLine = 0;
}

BwEdge::BwEdge( int cx, int cy, int ax, int ay)
{
	mControlPt.x = cx;
	mControlPt.y = cy;
	mEndPt.x = ax;
	mEndPt.y = ay;
}

BwEdge::~BwEdge()
{

}

void BwEdge::IsLine(int bline )
{
	mbLine = bline;
	return;
}

int BwEdge::IsLine()
{
	return mbLine;
	//return mControlPt == mEndPt;
}

void BwEdge::SetControlPt( CCPoint &pt )
{
	mControlPt = pt;
}

void BwEdge::SetEndPt( CCPoint &pt )
{
	mEndPt = pt;
}

//////////////////////////////////////////////////////////////////////////

BwPath::BwPath()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
	mFillLeft = 0;
	mFillRight = 0;
	mLineStyle = 0;
	mbNewShape = false;
}

BwPath::BwPath( int ax, int ay, int fillLeft, int fillRight, int line, bool bNewShape)
{
	mStartPt.x = ax;
	mStartPt.y = ay;
	mFillLeft = fillLeft;
	mFillRight = fillRight;
	mLineStyle = line;
	mbNewShape = bNewShape;
	mVecEdges.resize( 0 );
	return;
}

BwPath::~BwPath()
{
	ReleaseEdge();
}

void BwPath::AddEdge(BwEdge &pedge )
{
	mVecEdges.push_back( pedge );
	return;
}

void BwPath::StartPt( CCPoint &pt )
{
	//mStartPt = pt;
	mStartPt.x = pt.x;
	mStartPt.y = pt.y;
	return;
}

void BwPath::EmptyStartPt()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
}

bool BwPath::IsStartPtEmpty()
{
	bool b = (mStartPt.x == 0 && mStartPt.y == 0);
	return b;
}

void BwPath::ClearEdges()
{
	mStartPt.x = 0;
	mStartPt.y = 0;
	mVecEdges.clear();
	mVecEdges.resize( 0 );
	return;
}

bool BwPath::EdgeEmpty()
{
	return mVecEdges.empty();
}

int BwPath::PtInPath(CCPoint &pt )
{
	if( mStartPt == pt )
		return en_PtPos_Begin;
	else if( mVecEdges.back().mEndPt == pt )
		return en_PtPos_End;
	else 
	{
		vector< BwEdge >::iterator it = mVecEdges.begin();
		vector< BwEdge >::iterator end = mVecEdges.end();
		for( ; it != end; it++ )
		{
			BwEdge &edge = *it;
			if( edge.mEndPt == pt )
				return en_PtPos_Mid;
		}
	}
	return en_PtPos_No;
}

CCPoint &BwPath::StartPt()
{
	return mStartPt;
}

void BwPath::FillLeft( int fillleft )
{
	mFillLeft = fillleft;
}

int BwPath::FillLeft( )
{
	return mFillLeft;
}

void BwPath::FillRight( int fillRight )
{
	mFillRight = fillRight;
}
int BwPath::FillRight()
{
	return mFillRight;
}

void BwPath::LineStyle(int ls )
{
	mLineStyle = ls;
}

int BwPath::LineStyle()
{
	return mLineStyle;
}

void BwPath::ReleaseEdge()
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

void BwShapeRecord::AddFillSyle( BwFillStyle *fs )
{
	mVecFillStyles.push_back( fs );
}

void BwShapeRecord::AddLineStyle( BwLineStyle &ls )
{
	mVecLineStyles.push_back( ls );
}

void BwShapeRecord::AddPath( BwPath &path )
{
	mVecPaths.push_back( path );
}

void BwShapeRecord::ReleaseFillStyle()
{
	vector<BwFillStyle *>::iterator it;
	for( it = mVecFillStyles.begin(); it != mVecFillStyles.end(); it++ )
	{
		delete *it;
	}
	mVecFillStyles.clear();
	mVecFillStyles.resize( 0 );
}

void BwShapeRecord::ReleaseLineStyle()
{
	//vector< BwLineStyle *>::iterator it;
	//for( it = mVecLineStyles.begin(); it != mVecLineStyles.end(); it++ )
	//{
	//	delete *it;
	//}
	mVecLineStyles.clear();
	mVecLineStyles.resize( 0 );
	return;
}

void BwShapeRecord::ReleasePaths()
{
	//vector<BwPath*>::iterator it;
	//for( it = mVecPaths.begin(); it != mVecPaths.end(); it++ )
	//{
	//	delete *it;
	//}
	mVecPaths.clear();
	mVecPaths.resize( 0 );
	return;
}

void BwShapeRecord::ReleaseAll()
{
	OutputDebugStringA("----->Release All ShapeRecord");
	ReleaseFillStyle();
	ReleaseLineStyle();
	ReleasePaths();
}

BwPath &BwShapeRecord::GetPath(int index )
{
	return mVecPaths[index];
}
