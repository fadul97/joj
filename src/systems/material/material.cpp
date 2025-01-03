#include "systems/material/material.h"

joj::Material::Material()
    : ambient{ 0.0f, 0.0f, 0.0f, 1.0f },
      diffuse{ 0.0f, 0.0f, 0.0f, 1.0f },
     specular{ 0.0f, 0.0f, 0.0f, 1.0f },
      reflect{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}