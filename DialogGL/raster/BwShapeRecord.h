#ifndef __BWSHAPERECORD_H__
#define __BWSHAPERECORD_H__

#include "BwFillStyle.h"
#include <vector>
using namespace std;
#include "cocos2d.h"
USING_NS_CC;

class Edge
{
public:
	Edge();
	Edge( int cx, int cy, int ax, int ay);
	~Edge();
	void SetControlPt( CCPoint &pt );
	void SetEndPt( CCPoint &pt );
	bool straight();
	int IsLine( );
	void IsLine( int bline);
public:
	CCPoint cp;	//control
	CCPoint ap;
	int    mbLine;
};

enum PtPos
{
	en_PtPos_No = 0,
	en_PtPos_Begin,
	en_PtPos_Mid,
	en_PtPos_End
};

class Path
{
public:
	Path();
	Path( int ax, int ay, int fillLeft, int fillRight, int line, bool bNewShape);
	~Path();

public:
	void AddEdge(Edge &pedge );
	void StartPt( CCPoint &pt );
	CCPoint &StartPt();
	void EmptyStartPt();
	bool IsStartPtEmpty();
	void ClearEdges();
	bool EdgeEmpty();
	int PtInPath(CCPoint &pt );


	void FillLeft( int fillleft );
	int FillLeft( );

	void FillRight( int fillRight );
	int FillRight();

	void LineStyle(int ls );
	int LineStyle();

	void ReleaseEdge();

public:
	CCPoint mStartPt;	// start point
	vector< Edge > mVecEdges;
public:
	int mFill0;
	int mFill1;
	int mLine;
	bool mbNewShape;
};

class BwShapeRecord
{
public:
	BwShapeRecord();
	~BwShapeRecord();

public:
	void AddFillSyle( FillStyle *fs );
	void AddLineStyle( LineStyle &ls );
	void AddPath( Path &path );
	void SetBounds( );
	void ReleaseFillStyle();
	void ReleaseLineStyle();
	void ReleasePaths();
	void ReleaseAll();
	Path &GetPath(int index );

public:
	vector<FillStyle*> mFillStyles;
	vector<LineStyle> mLineStyles;
	vector<Path> mPaths;

	CCRect mBounds;
	int mcharid;
public:
	Path mClipPath;
};


#endif

