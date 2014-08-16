#ifndef __BWSHAPERECORD_H__
#define __BWSHAPERECORD_H__

#include "BwFillStyle.h"
#include <vector>
using namespace std;
#include "cocos2d.h"
USING_NS_CC;

class BwEdge
{
public:
	BwEdge();
	BwEdge( int cx, int cy, int ax, int ay);
	~BwEdge();
	void SetControlPt( CCPoint &pt );
	void SetEndPt( CCPoint &pt );
	int IsLine( );
	void IsLine( int bline);
public:
	CCPoint mControlPt;	//control
	CCPoint mEndPt;
	int    mbLine;
};

enum PtPos
{
	en_PtPos_No = 0,
	en_PtPos_Begin,
	en_PtPos_Mid,
	en_PtPos_End
};

class BwPath
{
public:
	BwPath();
	BwPath( int ax, int ay, int fillLeft, int fillRight, int line, bool bNewShape);
	~BwPath();

public:
	void AddEdge(BwEdge &pedge );
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
	vector< BwEdge > mVecEdges;
public:
	int mFillLeft;
	int mFillRight;
	int mLineStyle;
	bool mbNewShape;
};

class BwShapeRecord
{
public:
	BwShapeRecord();
	~BwShapeRecord();

public:
	void AddFillSyle( BwFillStyle *fs );
	void AddLineStyle( BwLineStyle &ls );
	void AddPath( BwPath &path );
	void SetBounds( );
	void ReleaseFillStyle();
	void ReleaseLineStyle();
	void ReleasePaths();
	void ReleaseAll();
	BwPath &GetPath(int index );

public:
	vector<BwFillStyle*> mVecFillStyles;
	vector<BwLineStyle> mVecLineStyles;
	vector<BwPath> mVecPaths;
public:
	BwPath mClipPath;
};


#endif

