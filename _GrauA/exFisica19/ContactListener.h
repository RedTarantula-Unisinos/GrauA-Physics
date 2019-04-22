#pragma once

#include "Spaceship.h"
#include "Platforms.h"

using namespace std;
class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) {

		b2Fixture* fa = contact->GetFixtureA();
		b2Fixture* fb = contact->GetFixtureB();

		b2Body* ba = fa->GetBody();
		b2Body* bb = fb->GetBody();

		Spaceship* ss = (Spaceship*)ba->GetUserData();
		Platforms* plt = (Platforms*)bb->GetUserData();
	}

};