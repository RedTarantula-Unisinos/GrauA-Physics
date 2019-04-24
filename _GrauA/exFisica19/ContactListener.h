#pragma once

#include "Spaceship.h"
#include "Platforms.h"
#include <typeinfo>

using namespace std;
class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		b2Fixture* fa = contact->GetFixtureA();
		b2Fixture* fb = contact->GetFixtureB();

		b2Body* ba = fa->GetBody();
		b2Body* bb = fb->GetBody();

		Platforms* obj1 = (Platforms*)ba->GetUserData();
		Spaceship* obj2 = (Spaceship*)bb->GetUserData();

		if (obj1 != NULL && obj2 != NULL)
		{
			int i = obj1->GetMultiplier();
			obj2->CheckShipWithPlatform(i);
		}
		else
		{
			obj2->Die();
		}

	}

};