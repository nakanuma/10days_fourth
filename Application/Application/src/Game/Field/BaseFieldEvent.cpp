#include "BaseFieldEvent.h"

void BaseFieldEvent::EventTimeUpdate() {
	eventTimer_ -= 1.0f / 60.0f;
}
