#pragma once
/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
___                 __    __ _ _    _
/___\__ _ _ __ ___  / / /\ \ (_) | _(_)
//  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
|___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_



#include "BaseApplication.h"
#include <OgreAxisAlignedBox.h>


//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);

	char* Stage;
	char* GameStage[4];

	Ogre::AxisAlignedBox SphereBox;
	Ogre::AxisAlignedBox LacketBox;
	Ogre::AxisAlignedBox Lacket2Box;


protected:
	virtual void createScene(void);
	void Change_GameStage(char* a);

	// 오우거 움직이기 관련 함수
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void SetDirection(float newdirx, float newdiry);

	virtual void createFrameListener(void);

	int Right_Score;
	int Left_Score;

	bool PanelSleep;


	//리플레이
	bool Replay; 

	Ogre::Real dirx;
	Ogre::Real diry;

	float BALL_SPEED;


	float GetReflection(float hity);
	float GetReflection2(float hity);

	bool StageChange;

	Ogre::StringVector Scores;
	Ogre::StringVector Scores2;

	OgreBites::ParamsPanel*  ScorePanel;
	OgreBites::ParamsPanel*  Score2Panel;

	OgreBites::Label* Stage_Label;
	OgreBites::Label* Score_Label;
	OgreBites::Label* Score_Label2;
	OgreBites::Label* Mode_Label;

	bool AImode;

	void Plus_LeftScore();
	void Plus_RightScore();


private:


	bool processUnbufferedInput(const Ogre::FrameEvent& evt);

};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
