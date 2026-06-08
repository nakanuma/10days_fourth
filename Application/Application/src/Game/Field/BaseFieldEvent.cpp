#include "BaseFieldEvent.h"

void BaseFieldEvent::EventTimeUpdate(float deltaTime) {
	eventTimer_ -= deltaTime;
}
