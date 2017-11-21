/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
	GameStage[0] = "Ready";
	GameStage[1] = "Play";
	GameStage[2] = "Ready2";
	GameStage[3] = "Play2";

	Stage = GameStage[0];


	BALL_SPEED = 330;
	dirx = 0.48*BALL_SPEED;
	diry = 0.48*BALL_SPEED;

	PanelSleep=false;

	Left_Score = 0;
	Right_Score = 0;

	StageChange = false;

	Replay = false;

	AImode=true;

}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{

	mSceneMgr->destroyQuery(mRaySceneQuery);
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// Set ambient light 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Create a light 
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20, 80, 50);

	//카메라 셋업
	mCamera->setPosition(Ogre::Vector3(0, 400, 800));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));

	mSceneMgr->setSkyBox(true, "Examples/StormySkyBox", 5000);


	//오른쪽 컨트롤박스 
	Ogre::Entity* ogreEntity3 = mSceneMgr->createEntity("LacketEnt", "cube.mesh");
	Ogre::SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode("Lacket");
	ogreNode3->setPosition(250, 0, 0);
	ogreNode3->setScale(0.1, 0.1, 0.8);
	ogreNode3->attachObject(ogreEntity3);
	//ogreNode3->showBoundingBox(true);


	//왼쪽 컨트롤박스
	Ogre::Entity* ogreEntity4 = mSceneMgr->createEntity("Lacket2Ent", "cube.mesh");
	Ogre::SceneNode *ogreNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode("Lacket2");
	ogreNode4->setPosition(-250, 0, 0);
	ogreNode4->setScale(0.1, 0.1, 0.8);
	ogreNode4->attachObject(ogreEntity4);
	//ogreNode4->showBoundingBox(true);


	//공
	Ogre::Entity *SphereEnt = mSceneMgr->createEntity("SphereEnt", "sphere.mesh");
	Ogre::SceneNode *Sphere = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sphere");
	Sphere->setPosition(0, 0, 0);
	Sphere->setScale(0.15, 0.15, 0.15);
	Sphere->attachObject(SphereEnt);
	//Sphere->showBoundingBox(true);
	SphereBox = SphereEnt->getWorldBoundingBox();

	// 그라운드
	Ogre::Entity* Ground = mSceneMgr->createEntity("name", Ogre::SceneManager::PT_PLANE);
	Ground->setMaterialName("Examples/TransparentTest");
	Ogre::SceneNode *GroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	GroundNode->pitch(Ogre::Degree(-90));
	GroundNode->setPosition(0, -30, 0);
	GroundNode->setScale(4, 2, 5);
	GroundNode->attachObject(Ground);




}

void TutorialApplication::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	Score_Label2 = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOPLEFT, "ScoreLabel2", "LeftScore : " + Ogre::StringConverter::toString(Left_Score), 200);
	Score_Label = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOPRIGHT, "ScoreLabel", "RightScore : " + Ogre::StringConverter::toString(Right_Score), 200);
	Stage_Label = mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOP, "StageLabel", "Stage 1", 400);
	Mode_Label =mTrayMgr->createLabel(OgreBites::TrayLocation::TL_TOP, "ModeLabel", "AI MODE", 200);

	Scores.push_back("~~~~~Left Win~~~~~");
	ScorePanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "ScorePanel", 200, Scores);

	if (ScorePanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(ScorePanel, OgreBites::TL_CENTER, 0);
			mTrayMgr->removeWidgetFromTray(ScorePanel);
			ScorePanel->hide();
		}

	Scores2.push_back("~~~~~Right Win~~~~~");
	Score2Panel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "Score2Panel", 200, Scores2);

	if (Score2Panel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(Score2Panel, OgreBites::TL_CENTER, 0);
			Score2Panel->show();
			mTrayMgr->removeWidgetFromTray(Score2Panel);
			Score2Panel->hide();
		}

		
	

}

void TutorialApplication::Change_GameStage(char* a)
{
	if (a == "Play")
		Stage = GameStage[1];
	else if (a == "Ready2")
		Stage = GameStage[2];
	else if (a == "Ready")
		Stage = GameStage[0];
	else if (a == "Play2")
		Stage = GameStage[3];
}


bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

	bool ret = BaseApplication::frameRenderingQueued(evt);

	Score_Label2->show();
	Score_Label->show();
	Stage_Label->show();
	Mode_Label->show();

	if (!processUnbufferedInput(evt)) return false;

	return ret;
}

void TutorialApplication::SetDirection(float newdirx, float newdiry)
{
	float length = sqrt(newdirx * newdirx + newdiry * newdiry);
	dirx = BALL_SPEED * (newdirx / length);
	diry = BALL_SPEED * (newdiry / length);
}


float TutorialApplication::GetReflection(float hity)
{
	LacketBox = mSceneMgr->getEntity("LacketEnt")->getWorldBoundingBox();

	Ogre::Vector3 Lacket_Z1 = LacketBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket_Z2 = LacketBox.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);

	float length = Lacket_Z2.z - Lacket_Z1.z;

	if (hity<0)
		hity = 0;
	else if (hity > length)
		hity = length;


	hity -= length / 2.0f;

	return 2.0f * (hity / (length / 2.0f));
}

float TutorialApplication::GetReflection2(float hity)
{
	LacketBox = mSceneMgr->getEntity("Lacket2Ent")->getWorldBoundingBox();

	Ogre::Vector3 Lacket_Z1 = Lacket2Box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket_Z2 = Lacket2Box.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);

	float length = Lacket_Z2.z - Lacket_Z1.z;

	if (hity<0)
		hity = 0;
	else if (hity > length)
		hity = length;


	hity -= length / 2.0f;

	return 2.0f * (hity / (length / 2.0f));
}


void TutorialApplication::Plus_LeftScore()
{
	if (Left_Score == 0)
		Left_Score = 1;
	else if (Left_Score == 1)
		Left_Score = 2;
	else if (Left_Score == 2)
	{
		Left_Score = 3;


		if(Stage=="Play")
		{
			if (ScorePanel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(ScorePanel, OgreBites::TL_CENTER, 0);
			ScorePanel->show();
			}

			PanelSleep=false;
			StageChange = true;
		}

		else if(Stage=="Play2")
		{
			if (ScorePanel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(ScorePanel, OgreBites::TL_CENTER, 0);
			ScorePanel->show();
			}
			PanelSleep=false;

			Replay=true;
			StageChange=false;
		}

	}
}

void TutorialApplication::Plus_RightScore()
{
	if (Right_Score == 0)
		Right_Score = 1;
	else if (Right_Score == 1)
		Right_Score = 2;
	else if (Right_Score == 2)
	{
		Right_Score = 3;

		if(Stage=="Play")
		{
			if (Score2Panel->getTrayLocation() == OgreBites::TL_NONE)
			{
			mTrayMgr->moveWidgetToTray(Score2Panel, OgreBites::TL_CENTER, 0);
			Score2Panel->show();
			}
		
			PanelSleep=false;
			StageChange = true;
		}

		else if(Stage=="Play2")
		{
			if (Score2Panel->getTrayLocation() == OgreBites::TL_NONE)
			{
				mTrayMgr->moveWidgetToTray(Score2Panel, OgreBites::TL_CENTER, 0);
				Score2Panel->show();
			}
			PanelSleep=false;
			Replay=true;
			StageChange=false;
		}
	}
}




bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	static bool mMouseDown = false;     //If a mouse button is depressed 

	static Ogre::Real mMove = 200;      // The movement constant



	LacketBox = mSceneMgr->getEntity("LacketEnt")->getWorldBoundingBox();
	Lacket2Box = mSceneMgr->getEntity("Lacket2Ent")->getWorldBoundingBox();
	SphereBox = mSceneMgr->getEntity("SphereEnt")->getWorldBoundingBox();

	Ogre::Vector3 LacketLocation = mSceneMgr->getSceneNode("Lacket")->getPosition();
	Ogre::Vector3 Lakcet2Location = mSceneMgr->getSceneNode("Lacket2")->getPosition();
	Ogre::Vector3 SphereLocation = mSceneMgr->getSceneNode("Sphere")->getPosition();


	Ogre::Vector3 Lacket_Z1 = LacketBox.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket_Z2 = LacketBox.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket_ZMiddle(Lacket_Z1.x, Lacket_Z1.y, (Lacket_Z1.z + Lacket_Z2.z) / 2);

	Ogre::Vector3 Lacket2_Z1 = Lacket2Box.getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket2_Z2 = Lacket2Box.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 Lacket2_ZMiddle(Lacket2_Z1.x, Lacket2_Z1.y, (Lacket2_Z1.z + Lacket2_Z2.z) / 2);


	Ogre::Vector3 Sphere_Z1 = SphereBox.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM);
	Ogre::Vector3 Sphere_Z2 = SphereBox.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
	Ogre::Vector3 Sphere_ZMiddle(Sphere_Z1.x, Sphere_Z2.y, (Sphere_Z1.z + Sphere_Z2.z) / 2);


	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVector2 = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVector3 = Ogre::Vector3::ZERO;




	if (Stage == "Ready")
	{
			if (mKeyboard->isKeyDown(OIS::KC_LSHIFT)&0x0001) // AI모드 전환
		{
			if(AImode==false)
			{
			Mode_Label->setCaption("AI MODE");
				AImode=true;
			}
			else if(AImode==true)
			{
				AImode=false;
			Mode_Label->setCaption("PVP MODE");
			}
		}


		mSceneMgr->getSceneNode("Sphere")->setPosition(0, 0, 0);
		mSceneMgr->getSceneNode("Lacket")->setPosition(200, 0, 0);
		mSceneMgr->getSceneNode("Lacket2")->setPosition(-200, 0, 0);

		if(Replay==true)
		{
			Right_Score = 0;
			Left_Score = 0;

			Stage_Label->setCaption("Stage 1");
			Score_Label->setCaption("RightScore :" + Ogre::StringConverter::toString(Right_Score));
			Score_Label2->setCaption("LeftScore :" + Ogre::StringConverter::toString(Left_Score));

			Replay=false;
		}


		if (StageChange == true)
		{
			Right_Score = 0;
			Left_Score = 0;

			Stage_Label->setCaption("Stage 2");
			Score_Label->setCaption("RightScore :" + Ogre::StringConverter::toString(Right_Score));
			Score_Label2->setCaption("LeftScore :" + Ogre::StringConverter::toString(Left_Score));
		}
		

		if (mKeyboard->isKeyDown(OIS::KC_SPACE))
		{
			if(PanelSleep==false)
			{
				mTrayMgr->removeWidgetFromTray(ScorePanel);
				ScorePanel->hide();
				mTrayMgr->removeWidgetFromTray(Score2Panel);
				Score2Panel->hide();
				PanelSleep=true;
			}

			if (StageChange == true)
			{
				if(PanelSleep==false)
				{
					mTrayMgr->removeWidgetFromTray(ScorePanel);
					ScorePanel->hide();
					mTrayMgr->removeWidgetFromTray(Score2Panel);
					Score2Panel->hide();

					PanelSleep=true;
				}

				Change_GameStage("Play2");
				
			}
			else if(StageChange == false)
				Change_GameStage("Play");
		}
	}

	if (Stage == "Ready2")
	{
			if (mKeyboard->isKeyDown(OIS::KC_LSHIFT)&0x0001) // AI모드 전환
		{
			if(AImode==false)
			{
			Mode_Label->setCaption("AI MODE");
				AImode=true;
			}
			else if(AImode==true)
			{
				AImode=false;
			Mode_Label->setCaption("PVP MODE");
			}
		}
		mSceneMgr->getSceneNode("Sphere")->setPosition(0, 0, 0);
		mSceneMgr->getSceneNode("Lacket")->setPosition(200, 0, 0);
		mSceneMgr->getSceneNode("Lacket2")->setPosition(-200, 0, 0);

		if (mKeyboard->isKeyDown(OIS::KC_SPACE))
		{	
			if(PanelSleep==false)
			{
				mTrayMgr->removeWidgetFromTray(ScorePanel);
				ScorePanel->hide();
				mTrayMgr->removeWidgetFromTray(Score2Panel);
				Score2Panel->hide();
				PanelSleep=true;
			}

			Change_GameStage("Play2");
		}
	}

	if (Stage == "Play" || Stage == "Play2")
	{


		transVector2.x += dirx;
		transVector2.z += diry;

		if (SphereBox.intersects(LacketBox)) // 라켓 1과의 충돌
		{
			SetDirection(-1, GetReflection(Sphere_Z2.z - Lacket_ZMiddle.z));
		}
		else if (SphereBox.intersects(Lacket2Box))
		{

			SetDirection(1, GetReflection2(Sphere_Z2.z - Lacket2_ZMiddle.z));
		}

		else if (SphereLocation.x>370) // 놀이판의 RIGHT
		{
			Plus_LeftScore();
			Score_Label2->setCaption("LeftScore :" + Ogre::StringConverter::toString(Left_Score));

			if (Stage == "Play2")
			{
				if(Replay==true)
					Change_GameStage("Ready");
				else if(Replay==false)
					Change_GameStage("Ready2");
			}
			else if(Stage == "Play")
				Change_GameStage("Ready");
		}

		else if (SphereLocation.z>190) // 놀이판의 FRONT
		{
			if (dirx>0)
				SetDirection(1, -1);
			else
				SetDirection(-1, -1);
		}

		else if (SphereLocation.z<-160) // 놀이판의 BACK
		{
			if (dirx>0)
				SetDirection(1, 1);
			else
				SetDirection(-1, 1);
		}

		else if (SphereLocation.x<-370) // 놀이판의 LEFT
		{
			Plus_RightScore();
			Score_Label->setCaption("RightScore :" + Ogre::StringConverter::toString(Right_Score));

			if (Stage == "Play2")
			{
				if(Replay==true)
					Change_GameStage("Ready");
				else if(Replay==false)
				Change_GameStage("Ready2");
			}
			else if(Stage=="Play")
				Change_GameStage("Ready");
		}


		//라켓 1의 이동 
		if (mKeyboard->isKeyDown(OIS::KC_UP))
		{
			if (LacketLocation.z<-100)
			{
				transVector.z += 0;

			}// Forward 
			else
				transVector.z -= mMove;
		}

		if (mKeyboard->isKeyDown(OIS::KC_RIGHT)) // Backward 
		{
			if (LacketLocation.x>370)
			{
				transVector.x += 0;
			}// Forward 
			else
				transVector.x += mMove;
		}

		if (mKeyboard->isKeyDown(OIS::KC_DOWN)) // Up 
		{
			if (LacketLocation.z>190)
			{
				transVector.z += 0;
			}// Forward 
			else
				transVector.z += mMove;
		}

		if (mKeyboard->isKeyDown(OIS::KC_LEFT)) // Down
		{
			if (LacketLocation.x<0)
			{
				transVector.x -= 0;
			}// Forward 
			else
				transVector.x -= mMove;
		}


		// 라켓 2의 이동

		if(AImode==true)
		{
			if(SphereLocation.x <0 && dirx < 0)
			{
				if(Lacket2_Z2.z < Sphere_Z2.z-(mMove*evt.timeSinceLastFrame))
				{
					if(Lakcet2Location.z>-100 && Lakcet2Location.z<190 &&Lakcet2Location.x<0 &&Lakcet2Location.x>-370)
					{
						transVector3.x += mMove;
						transVector3.z += mMove;
					}
					else if(Lakcet2Location.z<=-100)
					{
						//transVector3.x -= 0;
					transVector3.z += mMove;
					}

					else if(Lakcet2Location.x<=-370)
						transVector3.x +=  mMove;
				}

				else if(Lacket2_Z2.z > Sphere_Z2.z +(mMove* evt.timeSinceLastFrame))
				{
					if(Lakcet2Location.z>-100 && Lakcet2Location.z<190 &&Lakcet2Location.x<0 &&Lakcet2Location.x>-370)
					{
						transVector3.x -= mMove;
						transVector3.z -= mMove;
					}
					else if(Lakcet2Location.z<=-100)
					{
						//transVector3.x -= 0;
					transVector3.z += mMove;
					}

					else if(Lakcet2Location.x<=-370)
						transVector3.x += mMove;
				}

			}

			else
			{
				if(Lacket2_Z2.z < 175-(mMove*evt.timeSinceLastFrame))
				{
					if(Lakcet2Location.z>-100 && Lakcet2Location.z<190 &&Lakcet2Location.x<0 &&Lakcet2Location.x>-370)
					{
						transVector3.x += mMove;
						transVector3.z += mMove;
					}
					else if(Lakcet2Location.z<=-100)
					{
						//transVector3.x -= 0;
						transVector3.z += mMove;
					}
					else if(Lakcet2Location.x<=-370)
						transVector3.x += mMove;

				}
				
				else if(Lacket2_Z2.z > 175+(mMove*evt.timeSinceLastFrame))
				{
					if(Lakcet2Location.z>-100 && Lakcet2Location.z<190 &&Lakcet2Location.x<0 &&Lakcet2Location.x>-370)
					{
						transVector3.x -= mMove;
						transVector3.z -= mMove;
					}
					else if(Lakcet2Location.z<=-100)
						transVector3.z += mMove;
					else if(Lakcet2Location.x<=-370)
						transVector3.x += mMove;
				}
			}
		}

	else if(AImode==false)
	{
		if (mKeyboard->isKeyDown(OIS::KC_W))
		{
			if (Lakcet2Location.z<-100)
			{
				transVector3.z += 0;
	
			}// Forward 
			else
				transVector3.z -= mMove;
		}
	
		if (mKeyboard->isKeyDown(OIS::KC_S)) // Up 
		{
			if (Lakcet2Location.z>190)
			{
				transVector3.z += 0;
			}// Forward 
			else
				transVector3.z += mMove;
		}
	
		if (mKeyboard->isKeyDown(OIS::KC_A)) // Down
		{
			if (Lakcet2Location.x<-370)
			{
				transVector3.x -= 0;
			}// Forward 
			else
				transVector3.x -= mMove;
		}
	
		if (mKeyboard->isKeyDown(OIS::KC_D)) // Backward 
		{
			if (Lakcet2Location.x>0)
			{
				transVector3.x += 0;
			}// Forward 
			else
				transVector3.x += mMove;
		}
	}

		mSceneMgr->getSceneNode("Lacket")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mSceneMgr->getSceneNode("Lacket2")->translate(transVector3 * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		mSceneMgr->getSceneNode("Sphere")->translate(transVector2 * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	}



	return true;
}


//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		TutorialApplication app;

		try {
			app.go();

		}
		catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
