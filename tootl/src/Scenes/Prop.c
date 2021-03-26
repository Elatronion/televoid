#include "Prop.h"

void system_prop(hge_entity* entity, hge_transform* transform, hge_material* material, prop_component* prop) {
	hgeRenderSprite(hgeResourcesQueryShader("basic"), *material, *transform);
}
