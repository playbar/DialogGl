#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MainLayer : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	 

	void DrawTexture();
	void DrawPolygen();
	void DrawCCCipingNode();
	void DrawGradient();

protected:
	void InitActionTest();
	CCSprite*    m_grossini;
	CCSprite*    m_tamara;
	CCSprite*    m_kathia;

public:
	virtual void draw();
	void drawGraphics();
	virtual void onEnter();
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainLayer);
};

#endif // __HELLOWORLD_SCENE_H__
