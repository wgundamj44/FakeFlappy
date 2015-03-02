/*
 * LoadingScene.h
 *
 *  Created on: Jan 20, 2015
 *      Author: gao.da
 */

#ifndef LOADINGSCENE_H_
#define LOADINGSCENE_H_
#include "cocos2d.h"

class LoadingScene: public cocos2d::Scene {
public:
	virtual bool init();
	CREATE_FUNC(LoadingScene);

	void onEnter() override;
private:
	void loadingCallBack(cocos2d::Texture2D *texture);
};


#endif /* LOADINGSCENE_H_ */
