#include "Floater.h"

void system_floater(hge_entity* entity, hge_transform* transform, component_floater* floater) {
	float t = 3 * floater->original_transform.position.x + hgeRuntime() * floater->frequency;
	float x = cos(t / 2);
	float y = sin(t) * floater->amplitude;
	float r = cos(t / 1.5f) * floater->angle_of_rotation;

	transform->position.x = x + floater->original_transform.position.x;
	transform->position.y = y + floater->original_transform.position.y;
	transform->rotation = hgeQuaternionInitRotation(hgeVec3(0, 0, 1), r);
}
