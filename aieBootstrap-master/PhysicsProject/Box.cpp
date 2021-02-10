#include "Box.h"
#include <Gizmos.h>

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height)
	: Rigidbody(BOX, a_position, a_velocity, a_rotation, a_mass), m_extents(a_width, a_height)
{
	m_color = glm::vec4(1, 0, 0, 1);
}

Box::Box(glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass, float a_width, float a_height, glm::vec4 a_color)
	: Rigidbody(BOX, a_position, a_velocity, a_rotation, a_mass), m_extents(a_width, a_height)
{
	m_color = a_color;
}

Box::~Box()
{
}

void Box::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	Rigidbody::FixedUpdate(a_gravity, a_timeStep);

	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));


}

void Box::MakeGizmo()
{
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;

	aie::Gizmos::add2DTri(p1, p2, p4, m_color);
	aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}

bool Box::CheckBoxCorners(const Box& a_box, glm::vec2& a_contact, int& a_numContacts, float& a_pen, glm::vec2& a_edgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = a_box.GetExtents().x * 2;
	float boxH = a_box.GetExtents().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	for (float x = -a_box.GetExtents().x; x < boxW; x += boxW)
	{
		for (float y = -a_box.GetExtents().y; y < boxH; y += boxH)
		{
			glm::vec2 p = a_box.GetPosition() + x * a_box.m_localX + y * a_box.m_localY;

			glm::vec2 p0(glm::dot(p - m_position, m_localX), glm::dot(p - m_position, m_localY));

			if (first || p0.x < minX)
				minX = p0.x;
			if (first || p0.x > maxX)
				maxX = p0.x;
			if (first || p0.y < minY)
				minY = p0.y;
			if (first || p0.y > maxY)
				maxY = p0.y;

			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			return false;
		}
	}


	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.x || minY >= m_extents.x)
		return false;

	if (numLocalContacts == 0)
		return false;

	bool res = false;
	a_contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
		(float)numLocalContacts;
	a_numContacts++;

	// find the minimun penetration vector as a penetration amount and normal
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localX;
		a_pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = -m_localX;
		a_pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localY;
		a_pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.y;
	if (pen0 > 0 && (pen0 < a_pen || a_pen == 0))
	{
		a_edgeNormal = m_localY;
		a_pen = pen0;
		res = true;
	}

}


