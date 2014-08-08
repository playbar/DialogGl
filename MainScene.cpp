#include "MainScene.h"
#include "VisibleRect.h"

USING_NS_CC;


CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	DrawCCCipingNode();

	//DrawTexture();
	

    //// add a label shows "Hello World"
    //// create and initialize a label
  

    //
    return true;
}

void MainScene::DrawGradient()
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	//CCLayerColor *layer = CCLayerColor::create( ccc4( 0xFF, 0x00, 0x00, 0x80), 200, 200 );
	//layer->ignoreAnchorPointForPosition( false );
	//layer->setPosition( ccp( s.width / 2, s.height / 2 ));
	//this->addChild( layer, 1 );

	CCLayerGradient *layer = CCLayerGradient::create( ccc4( 0xff, 0x00, 0x00, 0xff ), ccc4( 255, 255, 0, 255 ), ccp( 200, 200));
	layer->ignoreAnchorPointForPosition( false );
	layer->setPosition( ccp( s.width / 2, s.height/ 2 ) );
	this->addChild( layer, 1 );
	

	return;
}

void MainScene::DrawCCCipingNode()
{
	enum
	{
		clippingNodeTag,
		spriteTag,
	};
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	CCSprite * background = CCSprite::create( "background.png" );
	background->setPosition( ccp( size.width / 2, size.height / 2 ));
	this->addChild( background, 0 );

	CCClippingNode * clippingNode = CCClippingNode::create();
	clippingNode->setContentSize( CCSizeMake( 200, 200 ) );
	clippingNode->setAnchorPoint( ccp( 0.5, 0.5 ) );
	clippingNode->setPosition( ccp( size.width /2, size.height / 2 ) );
	this->addChild( clippingNode, 1, clippingNodeTag );

	CCSprite * sprite = CCSprite::create( "HelloWorld.png" );
	sprite->setPosition( ccp( clippingNode->getContentSize().width / 2, clippingNode->getContentSize().height / 2 ));
	clippingNode->addChild( sprite, 1, spriteTag );

	CCDrawNode * stencil = CCDrawNode::create();
	CCPoint rectangle[8];
	rectangle[0].x = 0;
	rectangle[0].y = 0;

	rectangle[1].x = 0;
	rectangle[1].y = 50;

	rectangle[2].x = 100;
	rectangle[2].y = 70;

	rectangle[3].x = 50;
	rectangle[3].y = 50;

	rectangle[4].x = 130;
	rectangle[4].y = 60;

	rectangle[5].x = 120;
	rectangle[5].y = 40;

	rectangle[6].x = 140;
	rectangle[6].y = 20;

	rectangle[7].x = 100;
	rectangle[7].y = 0;

	ccColor4F white = { 1, 1, 1, 1 };
	stencil->drawPolygon( rectangle, 8, white, 1, white );
	clippingNode->setStencil( stencil );
	clippingNode->setInverted( false );
	return; 
}

void MainScene::DrawTexture()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(MainScene::menuCloseCallback));

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
		origin.y + pCloseItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 20 );
	
	// position the label on the center of the screen
	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
	                        origin.y + visibleSize.height - pLabel->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	//pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pSprite->setPosition( ccp( 0, 0 ));
	pSprite->setAnchorPoint( ccp( 0, 0 ));

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);

}


void MainScene::DrawPolygen()
{
	static ccColor4F green = { 0, 0, 1, 1 };
	const int nCount = 8;
	const float coef = 2.0f * (float) M_PI / nCount;
	static CCPoint circle[ nCount ];
	//float fRadius  = 100;
	//for ( unsigned int i = 0; i < nCount; i++ )
	//{
	//	float rads = i * coef;
	//	circle[i].x = fRadius * cosf( rads );
	//	circle[i].y = fRadius * sinf( rads );
	//}
	circle[0].x = 0;
	circle[0].y = 0;

	circle[1].x = 0;
	circle[1].y = 50;

	circle[2].x = 100;
	circle[2].y = 70;

	circle[3].x = 50;
	circle[3].y = 50;

	circle[4].x = 130;
	circle[4].y = 60;

	circle[5].x = 120;
	circle[5].y = 40;

	circle[6].x = 140;
	circle[6].y = 20;

	circle[7].x = 100;
	circle[7].y = 0;

	CCDrawNode *mStencil = CCDrawNode::create();
	mStencil->drawPolygon( circle, nCount, green, 0, green );
	//mStencil->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCScaleBy::create(0.05f, 0.95f),
	//	CCScaleTo::create(0.125f, 1))));
	mStencil->setScale( 2 );
	mStencil->setPosition(ccp( 200, 200 ) );
	addChild( mStencil );
}

void MainScene::InitActionTest()
{
	// Or you can create an sprite using a filename. only PNG is supported now. Probably TIFF too
	m_grossini = CCSprite::create( "grossini.png" );
	m_grossini->retain();

	m_tamara = CCSprite::create( "grossinis_sister1.png" ); 
	m_tamara->retain();

	m_kathia = CCSprite::create( "grossinis_sister2.png" );
	m_kathia->retain();

	addChild(m_grossini, 1);
	addChild(m_tamara, 2);
	addChild(m_kathia, 3);

	m_grossini->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height/3));
	m_tamara->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height*2/3));
	m_kathia->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height/2)); 

	return;
}

void MainScene::onEnter()
{
	CCLayer::onEnter();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	m_tamara->setScale( 2.5f );
	
}


void MainScene::draw()
{
	CCLayer::draw();
}

void MainScene::drawGraphics()
{
	//CCLayer::draw();

	CHECK_GL_ERROR_DEBUG();

	// draw a simple line
	// The default state is:
	// Line Width: 1
	// color: 255,255,255,255 (white, non-transparent)
	// Anti-Aliased
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth( 1 );
	//ccDrawColor4B( 255, 255, 255, 255 );
	ccDrawLine( VisibleRect::leftBottom(), VisibleRect::rightTop() );

	CHECK_GL_ERROR_DEBUG();

	// line: color, width, aliased
	// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	//	glDisable(GL_LINE_SMOOTH);
	glLineWidth( 5.0f );
	ccDrawColor4B(255,0,0,255);
	ccDrawLine( VisibleRect::leftTop(), VisibleRect::rightBottom() );

	CHECK_GL_ERROR_DEBUG();


	// TIP:
	// If you are going to use always the same color or width, you don't
	// need to call it before every draw
	//
	// Remember: OpenGL is a state-machine.

	// draw big point in the center
	ccPointSize(64);
	ccDrawColor4B(0,0,255,128);
	ccDrawPoint( VisibleRect::center() );

	CHECK_GL_ERROR_DEBUG();

	// draw 4 small points
	CCPoint points[] = { ccp(60,60), ccp(70,70), ccp(60,70), ccp(70,60) };
	ccPointSize(4);
	ccDrawColor4B(0,255,255,255);
	ccDrawPoints( points, 4);

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 10 segments
	glLineWidth(16);
	ccDrawColor4B(0, 255, 0, 255);
	ccDrawCircle( VisibleRect::center(), 100, 0, 100, false );

	CHECK_GL_ERROR_DEBUG();

	// draw a green circle with 50 segments with line to center
	glLineWidth(2);
	ccDrawColor4B(0, 255, 255, 255);
	ccDrawCircle( VisibleRect::center(), 50, CC_DEGREES_TO_RADIANS(90), 50,  false );

	CHECK_GL_ERROR_DEBUG();
	

	// open yellow poly
	ccDrawColor4B(255, 255, 0, 255);
	glLineWidth(10);
	CCPoint vertices[] = { ccp(10,0), ccp(50,50), ccp(100,50), ccp(100,100), ccp(50,100), ccp( 75, 75 ), ccp( 10, 75 ) };
	ccDrawPoly( vertices, 5, false );
	

	CHECK_GL_ERROR_DEBUG();

	// filled poly
	glLineWidth(1);
	CCPoint filledVertices[] = { ccp(0,120), ccp(50,120), ccp(50,170), ccp(25,200), ccp(0,170) };
	ccDrawSolidPoly(filledVertices, 5, ccc4f(0.5f, 0.5f, 1, 1 ) );
	
	// closed purble poly
	ccDrawColor4B(255, 0, 255, 255);
	glLineWidth(2);
	CCPoint vertices2[] = { ccp(30,130), ccp(30,230), ccp(50,200) };
	ccDrawPoly( vertices2, 3, true);

	CHECK_GL_ERROR_DEBUG();

	// draw quad bezier path
	ccDrawQuadBezier(VisibleRect::leftTop(), VisibleRect::center(), VisibleRect::rightTop(), 50);
	

	CHECK_GL_ERROR_DEBUG();

	// draw cubic bezier path
	ccDrawCubicBezier(VisibleRect::center(), ccp(VisibleRect::center().x+30,VisibleRect::center().y+50), ccp(VisibleRect::center().x+60,VisibleRect::center().y-50),VisibleRect::right(),100);

	CHECK_GL_ERROR_DEBUG();

	//draw a solid polygon
	CCPoint vertices3[] = {ccp(60,160), ccp(70,190), ccp(100,190), ccp(90,160)};
	ccDrawSolidPoly( vertices3, 4, ccc4f(1,1,0,1) );

	//return;

	// restore original values
	glLineWidth(1);
	ccDrawColor4B(255,255,255,255);
	ccPointSize(1);

	CHECK_GL_ERROR_DEBUG();
}

void MainScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
