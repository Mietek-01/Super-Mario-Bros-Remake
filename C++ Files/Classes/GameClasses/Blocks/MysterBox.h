#pragma once
#include "Block.h"
#include "../PhysicalObject.h"

class CMysterBox:public CDynamicBlock
{

    class CMushroom: public CPhysicaltObject
    {
        void updateForCollisionWithBlock(KindCollision,CBlock*)override;

    public:

        CMushroom(sf::Vector2f);
        ~CMushroom(){}

        void actOnObject(CGameObject*,KindCollision)override;
        void actOnMe(KindAction)override;
    };

    class CSpecialFlower: public CGameObject
    {
    public:

        CSpecialFlower(sf::Vector2f);
        ~CSpecialFlower(){}

        void actOnObject(CGameObject*,KindCollision)override;
        void actOnMe(KindAction)override;

        void update()override{};
    };

public:

    enum class MyItem
    {
        COIN,
        MUSHROOM,
        FLOWER
    };

    CMysterBox(sf::Vector2f,MyItem);
    ~CMysterBox(){}

    void update()override;
    void draw(const unique_ptr<sf::RenderWindow>&) override;
    void actOnMe(KindAction)override;

    static void setStaticAnimation();
    static void updateStaticAnimation(){s_private_animator.update();}

private:

    const MyItem m_rand_item;

    bool m_to_rand=true;
    bool m_createing_item=false;
    bool m_draw_question_mark=true;

    const float m_leaving_speed=-0.2f;

    CGameObject* m_my_obj=nullptr;

    static CAnimations s_private_animator;
};


