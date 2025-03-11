#include "joj/systems/light/light.h"

joj::DirectionalLight::DirectionalLight()
    : ambient{ 0.0f, 0.0f, 0.0f, 1.0f }, diffuse{ 0.0f, 0.0f, 0.0f, 1.0f },
    specular{ 0.0f, 0.0f, 0.0f, 1.0f },
    direction{ 0.0f, -1.0f, 0.0f }, pad{ -1.0f }
{
}

joj::PointLight::PointLight()
    : ambient{ 0.0f, 0.0f, 0.0f, 1.0f }, diffuse{ 0.0f, 0.0f, 0.0f, 1.0f },
    specular{ 0.0f, 0.0f, 0.0f, 1.0f },
    position{ 0.0f, 0.0f, 0.0f }, range{ 1.0f },
    att{ 0.0f, 0.0f, 0.0f }, pad{ -1.0f }
{
}

joj::SpotLight::SpotLight()
    : ambient{ 0.0f, 0.0f, 0.0f, 1.0f }, diffuse{ 0.0f, 0.0f, 0.0f, 1.0f },
    specular{ 0.0f, 0.0f, 0.0f, 1.0f },
    position{ 0.0f, 0.0f, 0.0f }, range{ 1.0f },
    direction{ 0.0f, -1.0f, 0.0f }, spot{ 1.0f },
    att{ 0.0f, 0.0f, 0.0f }, pad{ -1.0f }
{
}