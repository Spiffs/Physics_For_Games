#pragma once

#include "Application.h"
#include "Renderer2D.h"

#include "PhysicsScene.h"
#include "Billiards.h"

class PhysicsProjectApp : public aie::Application {
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	glm::vec2 ScreenToWorld(glm::vec2 a_screenPos);

	void DrawRect();
	void SphereAndPlane();
	void SpringTest(int a_amount);
	void TriggerTest();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	PhysicsScene* m_physicsScene;
	Billiards* m_billiards;

	const float m_aspectRatio = 16.f / 9.f;
	const float m_extents = 100;

};