#include "Game.h"

const int DEAD_LINE = 260;

const int SUTTLE_SPEED = 7;

const int ENEMY_CREATE_SPEED  = 100;
const int ENEMY_CREATE_MAX  = 7;
const int ENEMY_SPEED_MIN = 5;
const int ENEMY_SPEED_MAX = 12;

const int MONEY_CREATE_SPEED  = 200;
const int MONEY_CREATE_MAX  = 7;

const int MONEY_PATTERN_COUNT  	= 3;
const int MONEY_PATTERN_GAP  	= 230;

const int MONEY_BIG_SCORE 	= 100;
const int MONEY_SMALL_SCORE = 30;

const int MONEY_SPEED = 7;

const int MAX_LIFE = 5;

// func
int randRange(int nMin, int nMax)
{
	return Math::Rand()%(nMax-nMin) + nMin;
}


////////////////////////////////////////////////////////////
// BaseObject
////////////////////////////////////////////////////////////

// ------------------------------------------------------------

BaseObject::BaseObject()
{
	m_pos =  Point(0, 0);
	m_fSize = 0.0f;
	m_bDead = false;
	m_nDeadLine = DEAD_LINE;
	m_nAngle = 0;
}
// ------------------------------------------------------------
BaseObject::~BaseObject()
{

}

// ------------------------------------------------------------

void BaseObject::setPos(Point pos)
{
	m_pos = pos;
}

// ------------------------------------------------------------

Point BaseObject::getPos()
{
	return m_pos;
}

// ------------------------------------------------------------

void BaseObject::setSize(float fSize)
{
	m_fSize = fSize;
}

// ------------------------------------------------------------

float BaseObject::getSize()
{
	return m_fSize;
}

void BaseObject::setDead(bool bDead)
{
	m_bDead = bDead;
}
bool BaseObject::getDead()
{
	return m_bDead;
}
void BaseObject::setAngle(int nAngle)
{
	m_nAngle = nAngle;
}

int BaseObject::getAngle()
{
	return m_nAngle;
}

void BaseObject::setSpeed(int nSpeed)
{
	m_nSpeed = nSpeed;
}
int BaseObject::getSpeed()
{
	return m_nSpeed;
}

void BaseObject::setBitmap(Bitmap* pBitmap)
{
	if(!pBitmap)
		return;

	m_pBitmap = pBitmap;
}

Bitmap* BaseObject::getBitmap()
{
	return m_pBitmap;
}


void BaseObject::setDeadLine(int nDeadLine)
{
	m_nDeadLine = nDeadLine;
}

bool BaseObject::isHit(BaseObject* pTarget)
{
	if(!pTarget)
		return false;

	float fX = this->getPos().x - pTarget->getPos().x;
	// 이게임만 적용.. 가로로 가까워 질때만 체크
	if(0 <= fX - m_fSize/2 - pTarget->getSize()/2)
	{
		return false;
	}

	float fY = this->getPos().y - pTarget->getPos().y;

	float fDistance = Math::Sqrt((fX * fX) + (fY * fY)) - this->getSize()/2 - pTarget->getSize()/2;

	if(fDistance < 0)
	{
		return true;
	}
	return false;
}

void BaseObject::drawBitmap(Canvas* pCanvas)
{
	if(!pCanvas)
		return;
	if(!m_pBitmap)
		return;

	Point pos;
	pos.x = m_pos.x - m_pBitmap->GetWidth()/2;
	pos.y = m_pos.y - m_pBitmap->GetHeight()/2;

	pCanvas->DrawBitmap(pos, *m_pBitmap);
}

void BaseObject::drawBitmap(Canvas* pCanvas, int nAngle)
{
	if(!pCanvas)
		return;
	if(!m_pBitmap)
		return;

	Point pos;
	pos.x = m_pos.x - m_pBitmap->GetWidth()/2;
	pos.y = m_pos.y - m_pBitmap->GetHeight()/2;

	pCanvas->DrawBitmap(pos, *m_pBitmap, Point(0, 0), nAngle);
}

void BaseObject::leftWallCheck()
{
	if(m_pos.x < m_nDeadLine)
	{
		m_bDead = true;
	}
}

void BaseObject::checkWall()
{
	if(m_pos.y <= m_fSize/2)
		m_pos.y = m_fSize/2;

	if(SCREEN_HEIGHT-m_fSize/2 <= m_pos.y)
		m_pos.y = SCREEN_HEIGHT-m_fSize/2;
}


Suttle::Suttle()
{
	m_pBitSittle = null;
	m_pBitSittle2 = null;
	m_pBitSittle3 = null;
}
Suttle::~Suttle()
{
	if(m_pBitSittle)
	{
		delete m_pBitSittle;
		m_pBitSittle = null;
	}
	if(m_pBitSittle2)
	{
		delete m_pBitSittle2;
		m_pBitSittle2 = null;
	}
	if(m_pBitSittle3)
	{
		delete m_pBitSittle3;
		m_pBitSittle3 = null;
	}
}

void Suttle::init()
{
	setPos(Point(DEAD_LINE, 400));
	setSize(70);

	setSpeed(SUTTLE_SPEED);
	m_nGravity = 10;

	m_nLife = MAX_LIFE;

	// bitmap
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitSittle = pAppResource->GetBitmapN(L"game/suttle.png");
	m_pBitSittle2 = pAppResource->GetBitmapN(L"game/suttle2.png");
	m_pBitSittle3 = pAppResource->GetBitmapN(L"game/suttle3.png");
	setBitmap(m_pBitSittle);
}
void Suttle::dispose()
{
}
void Suttle::update()
{
	//m_pos.y += m_nGravity;
	static int nChange = 0;
	if(nChange == 0)
		setBitmap(m_pBitSittle);
	else if(nChange == 1)
		setBitmap(m_pBitSittle2);
	else if(nChange == 2)
	{
		setBitmap(m_pBitSittle3);
		nChange = 0;
	}
	nChange++;
	checkWall();
}

void Suttle::event(int nEventID)
{
	if(nEventID == ID_BUTTON_UP)
	{
		m_pos.y -= m_nSpeed;
	}
	else
	{
		m_pos.y += m_nSpeed;
	}
}
void Suttle::draw(Canvas* pCanvas)
{
	if(!pCanvas)
		return;

	drawBitmap(pCanvas, m_nAngle);
	DebugDraw::DrawCicle(pCanvas, m_pos, m_fSize, Color(255,0,0,255), 3);
}

void Suttle::hit()
{
	m_nLife--;
	if(m_nLife <= 0)
		setDead(true);
}

void Suttle::setLife(int nLife)
{
	m_nLife = nLife;
}
int Suttle::getLife()
{
	return m_nLife;
}
void Suttle::none()
{
	m_nAngle = 0;
}

void Suttle::up()
{
	m_nAngle = 3;
	m_pos.y -= m_nSpeed;
}
void Suttle::down()
{
	m_nAngle = -3;
	m_pos.y += m_nSpeed;
}

////////////////////////////////////
// Enemy
////////////////////////////////////

Enemy::Enemy() {

	m_nAngle = Math::Rand()%360;
}

Enemy::~Enemy() {

}

void Enemy::init()
{
	setPos(Point(SCREEN_WIDTH + getSize(), Math::Rand()%SCREEN_HEIGHT));
	checkWall();
	setSize(50);
	setSpeed(randRange(ENEMY_SPEED_MIN, ENEMY_SPEED_MAX));
}
void Enemy::update()
{
	m_pos.x -= m_nSpeed;
	m_nAngle += 10;
	leftWallCheck();
}
void Enemy::dispose()
{

}
void Enemy::draw(Canvas* pCanvas)
{
	drawBitmap(pCanvas);
	DebugDraw::DrawCicle(pCanvas, m_pos, m_fSize, Color(255,255,0,255), 3);
}

void Enemy::hit(Game* pGame)
{
	if(!pGame)
		return;

	Suttle* pSuttle =  pGame->getSuttle();
	if(pSuttle)
	{
		pSuttle->hit();
	}

	// particle
	pGame->getEffectManager()->make(EFFECT_BOOM, pSuttle->getPos(), Math::Rand()%2+3, 30, 10, 10, true);
	pGame->getEffectManager()->make(EFFECT_ROCK, this->getPos(), Math::Rand()%2+3, 30, 10, 10, true);
}

EnemyManager::EnemyManager()
{
	m_makeEnemyTime = 0;

	m_pBitStone1 = null;
	m_pBitStone2 = null;
	m_pBitStone3 = null;
}
EnemyManager::~EnemyManager()
{
	if(m_pBitStone1)
	{
		delete m_pBitStone1;
		m_pBitStone1 = null;
	}
	if(m_pBitStone2)
	{
		delete m_pBitStone2;
		m_pBitStone2 = null;
	}
	if(m_pBitStone2)
	{
		delete m_pBitStone2;
		m_pBitStone2 = null;
	}
}

void EnemyManager::init(Game* pGame)
{
	m_pGame = pGame;
	m_list.Construct(100);


	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitStone1 = pAppResource->GetBitmapN(L"game/stone1.png");
	m_pBitStone2 = pAppResource->GetBitmapN(L"game/stone2.png");
	m_pBitStone3 = pAppResource->GetBitmapN(L"game/stone3.png");
}
void EnemyManager::update()
{
	checkSuttle();
	makeEnemy();
	removeDeadEnemy();

	// update
	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Enemy* pEnemy = (Enemy*)m_list.GetAt(i);
		if(pEnemy)
		{
			pEnemy->update();
		}
	}
}
void EnemyManager::dispose()
{
	removeAll();
}
void EnemyManager::draw(Canvas* pCanvas)
{
	if(!pCanvas)
		return;

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Enemy* pEnemy = (Enemy*)m_list.GetAt(i);
		if(pEnemy)
		{
			pEnemy->draw(pCanvas);
		}
	}
}


void EnemyManager::checkSuttle()
{
	if(!m_pGame)
		return;

	Suttle * pSuttle = m_pGame->getSuttle();
	if(!pSuttle)
		return;

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Enemy* pEnemy = (Enemy*)m_list.GetAt(i);
		if(pEnemy
		&& pEnemy->isHit(pSuttle))
		{
			pEnemy->hit(m_pGame);
			pEnemy->setDead(true);
		}
	}
}

void EnemyManager::makeEnemy()
{
	if(!m_pGame)
		return;

	long nTime = m_pGame->getAccTime() - m_makeEnemyTime;
	if(ENEMY_CREATE_SPEED < nTime && m_list.GetCount() < ENEMY_CREATE_MAX)
	{
		Enemy* pTemp = new Enemy();
		if(pTemp)
		{
			pTemp->init();
			int nRand = Math::Rand()%3;
			if(nRand == 0)		pTemp->setBitmap(m_pBitStone1);
			else if(nRand == 1)	pTemp->setBitmap(m_pBitStone2);
			else 				pTemp->setBitmap(m_pBitStone3);

			m_list.Add(*pTemp);

			m_makeEnemyTime = m_pGame->getAccTime();
		}
	}
}

void EnemyManager::removeDeadEnemy()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Enemy* pEnemy = (Enemy*)m_list.GetAt(i);
		if(pEnemy)
		{
			if(pEnemy->getDead() == true)
			{
				m_list.RemoveAt(i);
				delete pEnemy;
			}
		}
	}
}

void EnemyManager::removeAll()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Enemy* pEnemy = (Enemy*)m_list.GetAt(i);
		if(pEnemy)
		{
			m_list.RemoveAt(i);
			delete pEnemy;
		}
	}
}

////////////////////////////////////////////////////////////
// Item
////////////////////////////////////////////////////////////

Item::Item()
{

}

Item::~Item()
{

}

////////////////////////////////////////////////////////////
// ItemMoney
////////////////////////////////////////////////////////////

ItemMoney::ItemMoney(eMoneyType type)
{
	m_type = type;
}
ItemMoney::~ItemMoney()
{

}

void ItemMoney::init()
{
	setSize(30);
	setPos(Point(SCREEN_WIDTH + getSize(), Math::Rand()%SCREEN_HEIGHT));
	checkWall();
	setSpeed(MONEY_SPEED);
}
void ItemMoney::dispose()
{

}
void ItemMoney::update()
{
	m_pos.x -= m_nSpeed;
	leftWallCheck();
	//checkSuttle();
}
void ItemMoney::draw(Canvas* pCanvas)
{
	drawBitmap(pCanvas);
	DebugDraw::DrawCicle(pCanvas, m_pos, m_fSize, Color(0,255,0,100), 10);
}

void ItemMoney::effect(Game* pGame)
{
	if(!pGame)
		return;

	if(m_type == MONEY_SMALL)
	{
		pGame->addScore(MONEY_SMALL_SCORE);
	}
	else
	{
		pGame->addScore(MONEY_BIG_SCORE);
	}
	pGame->getEffectManager()->make(EFFECT_LIGHT, this->getPos(), 1, 0, 5, 5 , false);
}

////////////////////////////////////////////////////////////
// ItemManager
////////////////////////////////////////////////////////////

ItemManager::ItemManager()
{
	m_makeMoneyTime = 0;
	m_makePatternTime = 0;

	m_bMoneyMaking = false;

	//bit
	m_pBitItemSmall = null;
	m_pBitItemBig = null;
}
ItemManager::~ItemManager()
{
	if(m_pBitItemSmall)
	{
		delete m_pBitItemSmall;
		m_pBitItemSmall = null;
	}
	if(m_pBitItemBig)
	{
		delete m_pBitItemBig;
		m_pBitItemBig = null;
	}
}

void ItemManager::init(Game* pGame)
{
	m_pGame = pGame;
	m_list.Construct(100);

	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitItemBig = pAppResource->GetBitmapN(L"game/itemBig.png");
	m_pBitItemSmall = pAppResource->GetBitmapN(L"game/itemSmall.png");
}
void ItemManager::update()
{
	checkSuttle();
	makeMoney();
	makePattern();
	removeDeadItem();

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Item* pItem = (Item*)m_list.GetAt(i);
		if(pItem)
		{
			pItem->update();
		}
	}
}
void ItemManager::dispose()
{
	removeAll();
}
void ItemManager::draw(Canvas* pCanvas)
{
	if(!pCanvas)
		return;

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Item* pItem = (Item*)m_list.GetAt(i);
		if(pItem)
		{
			pItem->draw(pCanvas);
		}
	}
}

void ItemManager::checkSuttle()
{
	if(!m_pGame)
		return;

	Suttle * pSuttle = m_pGame->getSuttle();
	if(!pSuttle)
		return;

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Item* pItem = (Item*)m_list.GetAt(i);
		if(	pItem
		&&	pItem->isHit(pSuttle))
		{
			pItem->effect(m_pGame);
			pItem->setDead(true);
		}
	}
}

void ItemManager::makeMoney()
{
	if(!m_pGame)
		return;

	long nTime = m_pGame->getAccTime() - m_makeMoneyTime;
	if(MONEY_CREATE_SPEED < nTime && m_list.GetCount() < MONEY_CREATE_MAX)
	{
		if(Math::Rand()%2 == 0)
		{
			Item* pTemp = new ItemMoney(MONEY_BIG);
			if(pTemp)
			{
				pTemp->init();
				pTemp->setBitmap(m_pBitItemBig);
				m_list.Add(*pTemp);

				m_makeMoneyTime = m_pGame->getAccTime();
			}
		}
		else
		{
			if(m_bMoneyMaking == false)
			{
				m_bMoneyMaking = true;
				m_nMoneyMakingCount = 0;
				m_pMoneyStart = Point(SCREEN_WIDTH, Math::Rand()%SCREEN_HEIGHT);

				m_makeMoneyTime = m_pGame->getAccTime();
			}
		}
	}
}

void ItemManager::makePattern()
{
	if(!m_pGame)
		return;

	if(m_bMoneyMaking == false)
		return;

	long nTime = m_pGame->getAccTime() - m_makePatternTime;
	if(MONEY_PATTERN_GAP < nTime)
	{
		Item* pTemp = new ItemMoney(MONEY_SMALL);
		if(pTemp)
		{
			pTemp->init();
			pTemp->setPos(m_pMoneyStart);
			pTemp->setBitmap(m_pBitItemSmall);
			m_list.Add(*pTemp);

			m_makeMoneyTime = m_pGame->getAccTime();
		}

		m_nMoneyMakingCount++;
		if(MONEY_PATTERN_COUNT < m_nMoneyMakingCount)
		{
			m_bMoneyMaking = false;
		}
		m_makePatternTime = m_pGame->getAccTime();
	}
}

void ItemManager::removeDeadItem()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Item* pItem = (Item*)m_list.GetAt(i);
		if(pItem)
		{
			if(pItem->getDead() == true)
			{
				m_list.RemoveAt(i);
				delete pItem;
			}
		}
	}
}
void ItemManager::removeAll()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Item* pItem = (Item*)m_list.GetAt(i);
		if(pItem)
		{
			m_list.RemoveAt(i);
			delete pItem;
		}
	}
}

////////////////////////////////////////////////////////////
// Effect
////////////////////////////////////////////////////////////

Effect::Effect()
{

}

Effect::~Effect()
{

}

void Effect::init(int nRandRange, int nLife, bool bRandPos)
{
	m_nLife = nLife;

	m_fStepX = (float)(Math::Rand()%nRandRange-nRandRange/2)/10;
	m_fStepY = (float)(Math::Rand()%nRandRange-nRandRange/2)/10;

	m_ptOriginal = getPos();
	if(bRandPos == true)
	{
		if(getBitmap())
		{
			int nWidth = getBitmap()->GetWidth()/3;
			int nHeight = getBitmap()->GetHeight()/3;
			m_ptOriginal.x = m_ptOriginal.x + Math::Rand()%nWidth - nWidth/2;
			m_ptOriginal.y = m_ptOriginal.y + Math::Rand()%nHeight - nHeight/2;
		}
	}

	m_fX = 0;
	m_fY = 0;
}

void Effect::init()
{


}
void Effect::dispose()
{

}

void Effect::update()
{
	m_fX += m_fStepX;
	m_fY += m_fStepY;

	Point pt;
	pt.x = m_ptOriginal.x + m_fX;
	pt.y = m_ptOriginal.y + m_fY;
	setPos(pt);
	m_nLife--;
	if(m_nLife < 0)
	{
		setDead(true);
	}
}

void Effect::draw(Canvas* pCanvas)
{
	drawBitmap(pCanvas);
}

////////////////////////////////////////////////////////////
// EffectManager
////////////////////////////////////////////////////////////

EffectManager::EffectManager()
{

}
EffectManager::~EffectManager()
{
	if(m_pBitLight)
	{
		delete m_pBitLight;
		m_pBitLight = null;
	}

	if(m_pBitBoom)
	{
		delete m_pBitBoom;
		m_pBitBoom = null;
	}

	if(m_pBitRock)
	{
		delete m_pBitRock;
		m_pBitRock = null;
	}
}

void EffectManager::init()
{
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitLight = pAppResource->GetBitmapN(L"game/light.png");
	m_pBitBoom = pAppResource->GetBitmapN(L"game/boom.png");
	m_pBitRock = pAppResource->GetBitmapN(L"game/rock.png");

}
void EffectManager::update()
{
	removeDeadEffect();

	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Effect* pEffect = (Effect*)m_list.GetAt(i);
		if(pEffect)
		{
			pEffect->update();
		}
	}
}
void EffectManager::dispose()
{
	removeAll();
}
void EffectManager::draw(Canvas* pCanvas)
{
	int nSize = m_list.GetCount();
	for(int i=0; i<nSize; i++)
	{
		Effect* pEffect = (Effect*)m_list.GetAt(i);
		if(pEffect)
		{
			pEffect->draw(pCanvas);
		}
	}
}

void EffectManager::make(EffectType type, Point pos, int nCount,
		int nRandRage, int nLifeMin, int nLifeMax, bool bRandPos)
{
	for(int i=0; i<nCount; i++)
	{
		Effect* pEffect = new Effect();
		if(type == EFFECT_LIGHT)
			pEffect->setBitmap(m_pBitLight);
		else if(type == EFFECT_BOOM)
			pEffect->setBitmap(m_pBitBoom);
		else if(type == EFFECT_ROCK)
			pEffect->setBitmap(m_pBitRock);

		pEffect->setPos(pos);
		pEffect->init(nRandRage, Math::Rand()%nLifeMin+nLifeMax, bRandPos);
		m_list.Add(*pEffect);
	}
}

void EffectManager::removeDeadEffect()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Effect* pEffect = (Effect*)m_list.GetAt(i);
		if(pEffect)
		{
			if(pEffect->getDead() == true)
			{
				m_list.RemoveAt(i);
				delete pEffect;
			}
		}
	}
}

void EffectManager::removeAll()
{
	int nSize = m_list.GetCount();

	for(int i=nSize-1; 0<=i; i--)
	{
		Effect* pEffect = (Effect*)m_list.GetAt(i);
		if(pEffect)
		{
			m_list.RemoveAt(i);
			delete pEffect;
		}
	}
}


////////////////////////////////////
// TutorialManager
////////////////////////////////////

TutorialManager::TutorialManager()
{
	m_eStep = TUTORIAL_FACE_RECO;
}
TutorialManager::~TutorialManager()
{
	if(m_pBitBack)
	{
		delete m_pBitBack;
		m_pBitBack = null;
	}
}
void TutorialManager::init(Game* pGame)
{
	m_pGame = pGame;

	//bitmap
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitBack = pAppResource->GetBitmapN(L"game/tutorial_back.png");

}
void TutorialManager::update()
{
	if(!m_pGame)
		return;

	if(m_eStep == TUTORIAL_FACE_RECO)
	{
		if(m_pGame->getExpression() != FACIAL_EXPRESSION_NONE)
		{
			m_eStep = TUTORIAL_UP;
		}
	}
	else if(m_eStep == TUTORIAL_UP)
	{
		Point ptTarget = Point(250, 100);

		Point ptNow = m_pGame->getSuttle()->getPos();
		float fSize = m_pGame->getSuttle()->getSize();
		if(ptNow.y+fSize/2 < ptTarget.y)
		{
			m_eStep = TUTORIAL_DOWN;
		}
	}
	else if(m_eStep == TUTORIAL_DOWN)
	{
		Point ptTarget = Point(250, 400);
		Point ptNow = m_pGame->getSuttle()->getPos();
		float fSize = m_pGame->getSuttle()->getSize();
		if(ptTarget.y < ptNow.y-fSize/2)
		{
			m_eStep = TUTORIAL_INFO;
		}
	}
}
void TutorialManager::dispose()
{

}

void TutorialManager::draw(Canvas* pCanvas)
{
	if(!pCanvas)
		return;

	if(m_eStep == TUTORIAL_FACE_RECO)
	{

		String strText;
		Point poLeftTop;
		strText = L"김치 치즈 스마일은 얼굴을 이용하여 플레이하는 게임이에요!";
		poLeftTop = Point(300, 200);
		//pCanvas->DrawText(poLeftTop, strText);

		strText = L"옆 카메라에 얼굴을 인식해 주세요";
		poLeftTop = Point(400, 250);
		pCanvas->DrawText(poLeftTop, strText);
	}
	else if(m_eStep == TUTORIAL_UP)
	{

		String strText;
		Point poLeftTop;
		strText = L"잘했어요!! 이번에는 빨간선 위까지 이동해볼거에요";
		poLeftTop = Point(300, 200);
		pCanvas->DrawText(poLeftTop, strText);

		strText = L"비행기를 상승시키려면 웃어야 해요";
		poLeftTop = Point(400, 250);
		//pCanvas->DrawText(poLeftTop, strText);

		strText = L"활짝 웃으세요 김치~:D";
		poLeftTop = Point(500, 300);
		//pCanvas->DrawText(poLeftTop, strText);
		//pCanvas->DrawBitmap(m_ptTarget, *m_pBitmapLine);
	}
	else if(m_eStep == TUTORIAL_DOWN)
	{

		String strText;
		Point poLeftTop;
		strText = L"잘했어요!! 이번에는 빨간선 아래까지 이동해볼거에요!!";
		poLeftTop = Point(300, 100);
		//pCanvas->DrawText(poLeftTop, strText);

		strText = L"비행기를 하강하려면 놀란 표정을 지어야 해요";
		poLeftTop = Point(400, 250);
		//pCanvas->DrawText(poLeftTop, strText);

		strText = L"눈을 크게 뜨고 입을 벌려주는게 포인트! 8-ㅇ";
		poLeftTop = Point(300, 300);
		//pCanvas->DrawText(poLeftTop, strText);

		//bitmap
		//pCanvas->DrawBitmap(m_ptTarget, *m_pBitmapLine);
	}
	else if(m_eStep == TUTORIAL_INFO)
	{
		String strText;
		Point poLeftTop;
		strText = L"게임에 등장하는 요소는 아래와 같아요. 아이템을 획득하고 돌멩이를 피해서 최고의 점수를 얻어보세요!";
		poLeftTop = Point(300, 200);
		pCanvas->DrawText(poLeftTop, strText);

		strText = L"게임할 준비가 되었으면 튜토리얼 종료 버튼을 눌러주세요!!";
		poLeftTop = Point(400, 250);
		//pCanvas->DrawText(poLeftTop, strText);

		//bitmap
		//pCanvas->DrawBitmap(Point(300,300), *m_pBitmapInfo);
	}
}


////////////////////////////////////
// Game
////////////////////////////////////

Game::Game()
{

}

Game::~Game()
{
	if(m_pBitHeart)
	{
		delete m_pBitHeart;
		m_pBitHeart = null;
	}
	if(m_pBitFaceSmile)
	{
		delete m_pBitFaceSmile;
		m_pBitFaceSmile = null;
	}
	if(m_pBitFaceSmileBlack)
	{
		delete m_pBitFaceSmileBlack;
		m_pBitFaceSmileBlack = null;
	}
	if(m_pBitFaceSurprise)
	{
		delete m_pBitFaceSurprise;
		m_pBitFaceSurprise = null;
	}
	if(m_pBitFaceSurpriseBlack)
	{
		delete m_pBitFaceSurpriseBlack;
		m_pBitFaceSurpriseBlack = null;
	}
	if(m_pBitBack)
	{
		delete m_pBitBack;
		m_pBitBack = null;
	}
	if(m_pBitScreen)
	{
		delete m_pBitScreen;
		m_pBitScreen = null;
	}
	if(m_pBitGameOver)
	{
		delete m_pBitGameOver;
		m_pBitGameOver = null;
	}
}

long Game::getAccTime()
{
	return m_lAccTime;
}

void Game::init()
{
	m_tutorial.init(this);
	m_suttle.init();
	m_enemyManager.init(this);
	m_itemManager.init(this);
	m_effect.init();

	m_eGameMode = MODE_TUTORIAL;
	//m_eGameMode = MODE_GAME;
	m_eExpression = FACIAL_EXPRESSION_NONE;
	m_nScore = 0;

	//bitmap
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	if(!pAppResource)
		return;

	m_pBitHeart = pAppResource->GetBitmapN(L"game/heart.png");
	m_pBitFaceSmile = pAppResource->GetBitmapN(L"game/smile.png");
	m_pBitFaceSmileBlack = pAppResource->GetBitmapN(L"game/smile_black.png");
	m_pBitFaceSurprise = pAppResource->GetBitmapN(L"game/surprise.png");
	m_pBitFaceSurpriseBlack = pAppResource->GetBitmapN(L"game/surprise_black.png");
	m_pBitBack = pAppResource->GetBitmapN(L"game/back.png");
	m_pBitScreen = pAppResource->GetBitmapN(L"game/screen.png");
	m_pBitGameOver = pAppResource->GetBitmapN(L"game/gameover.png");
}
void Game::dispose()
{
	m_tutorial.dispose();
	m_suttle.dispose();
	m_enemyManager.dispose();
	m_itemManager.dispose();
	m_effect.dispose();
}
void Game::update()
{
	m_lAccTime += 1000/GAME_FRAME;

	if(m_suttle.getDead() == false)
	{
		m_suttle.update();
		if(m_eGameMode == MODE_TUTORIAL)
		{
			m_tutorial.update();
		}
		else
		{
			addScore(1);
			m_enemyManager.update();
			m_itemManager.update();

			if(m_suttle.getLife() <= 0)
			{
				m_eGameMode = MODE_OVER;
			}
		}

		// face
		if(m_eExpression == FACIAL_EXPRESSION_HAPPY)
		{
			m_suttle.up();
		}
		else if(m_eExpression == FACIAL_EXPRESSION_SURPRISED)
		{
			m_suttle.down();
		}
		else
		{
			m_suttle.none();
		}

		m_effect.update();
	}

	m_effect.update();
}

void Game::event(int nEventID)
{
	if(m_eGameMode == MODE_TUTORIAL
	||m_eGameMode == MODE_GAME)
	{
		m_suttle.event(nEventID);
	}
}
void Game::draw(Canvas* pCanvas)
{
	drawBitmap(pCanvas);
	//TODO
	drawText(pCanvas);
	drawBitmapAfter(pCanvas);
	//drawFaceInfo(pCanvas);
	// DebugDraw::DrawCicle(pCanvas, Point(0,0), 50, Color(255,255,255,255), 3);
}

void Game::addScore(int nScore)
{
	m_nScore += nScore;
}

Suttle * Game::getSuttle()
{
	return &m_suttle;
}

EnemyManager * Game::getEnemyManager()
{
	return &m_enemyManager;
}
ItemManager * Game::getItemManager()
{
	return &m_itemManager;
}

EffectManager* Game::getEffectManager()
{
	return &m_effect;
}

void Game::setFaceMode(FacialExpression mode)
{
	m_eExpression = mode;
}

void Game::setGameMode(eGameMode eMode)
{
	m_eGameMode = eMode;
}

FacialExpression Game::getExpression()
{
	return m_eExpression;
}

void Game::drawBitmap(Canvas* pCanvas)
{
	// back
	pCanvas->DrawBitmap(Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), *m_pBitBack);
	// screen
	if(m_eExpression == FACIAL_EXPRESSION_NONE)
		pCanvas->DrawBitmap(Rectangle(10, 10, 240, 240), *m_pBitScreen);

	// life
	int nLife = m_suttle.getLife();
	int nLifeX = 50;
	int nLifeY = 250;
	int nLifeXOff = 25;
	for(int i=0; i<nLife; i++)
	{
		pCanvas->DrawBitmap(Point(nLifeX+(i*nLifeXOff), nLifeY), *m_pBitHeart);
	}
	// face
	Point ptHappy = Point(10, 300);
	Point ptSurprise = Point(110, 300);
	if(m_eExpression == FACIAL_EXPRESSION_HAPPY)
	{
		pCanvas->DrawBitmap(ptHappy, *m_pBitFaceSmile);
	}
	else
	{
		pCanvas->DrawBitmap(ptHappy, *m_pBitFaceSmileBlack);
	}
	if(m_eExpression == FACIAL_EXPRESSION_SURPRISED)
	{
		pCanvas->DrawBitmap(ptSurprise, *m_pBitFaceSurprise);
	}
	else
	{
		pCanvas->DrawBitmap(ptSurprise, *m_pBitFaceSurpriseBlack);
	}
	// manager
	if(m_suttle.getDead() == false)
		m_suttle.draw(pCanvas);

	if(m_eGameMode != MODE_TUTORIAL)
	{
		m_enemyManager.draw(pCanvas);
		m_itemManager.draw(pCanvas);
		m_effect.draw(pCanvas);
	}
}

void Game::drawBitmapAfter(Canvas* pCanvas)
{
	// game over
	if(m_eGameMode == MODE_OVER)
	{
		pCanvas->DrawBitmap(Point(300, 150), *m_pBitGameOver);
	}
}

void Game::drawText(Canvas* pCanvas)
{
	Font* font = new Font();
	font->Construct(FONT_STYLE_PLAIN | FONT_STYLE_BOLD, 30);
	pCanvas->SetForegroundColor(Color(0,0,0));
	pCanvas->SetFont(*font);

	String strText;
	Point poLeftTop;
	if(m_eGameMode == MODE_TUTORIAL)
	{
		strText.Format(50, L"튜토리얼 모드입니다");
		poLeftTop = Point(200, 400);
	}
	if(m_eGameMode == MODE_GAME)
	{
		strText.Format(50, L"Score : %d", m_nScore);
		poLeftTop = Point(10, 400);
	}
	pCanvas->DrawText(poLeftTop, strText);

	{
		String strText2;
		Point poLeftTop2;

		strText2 = "으으ㅏ아아아악";
		poLeftTop2 = Point(100, 200);
		pCanvas->DrawText(poLeftTop2, strText2);
	}

	{
		String strText2;
		Point poLeftTop2;

		strText2 = "ㅁ머ㅓ머머머멈머ㅓ2@@asdfasdf!M<<>";
		poLeftTop2 = Point(200, 300);
		pCanvas->DrawText(poLeftTop2, strText2);
	}


	if(m_eGameMode == MODE_TUTORIAL)
	{
		//m_tutorial.draw(pCanvas);
	}

	delete font;
}

void Game::drawFaceInfo(Canvas* pCanvas)
{
	Font font;
	font.Construct(FONT_STYLE_PLAIN | FONT_STYLE_BOLD, 30);
	pCanvas->SetFont(font);

	String strText;
	if(m_eExpression == FACIAL_EXPRESSION_HAPPY)
	{
		strText.Format(50, L"Happy");
	}
	else if(m_eExpression == FACIAL_EXPRESSION_NEUTRAL ||
			m_eExpression == FACIAL_EXPRESSION_ANGRY)
	{
		strText.Format(50, L"Normal");
	}
	else if(m_eExpression == FACIAL_EXPRESSION_SURPRISED)
	{
		strText.Format(50, L"Surprise");
	}
	else if(m_eExpression == FACIAL_EXPRESSION_NONE)
	{
		strText.Format(50, L"None");
	}

	Point poLeftTop = Point(300, 200);
	pCanvas->DrawText(poLeftTop, strText);
}
