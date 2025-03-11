#include "joj/resources/key_frame.h"

joj::KeyFrame::KeyFrame()
	: time_pos{ 0.0f },
	translation{ 0.0f, 0.0f, 0.0f },
	scale{ 1.0f, 1.0f, 1.0f },
	rotation_quat{ 0.0f, 0.0f, 0.0f, 1.0f }
{
}

joj::KeyFrame::~KeyFrame()
{
}