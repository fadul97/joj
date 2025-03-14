#include "resources/bone_animation.h"

f32 joj::BoneAnimation::get_start_time() const
{
    // keyframes are sorted by time, so first keyframe gives start time.
    return keyframes.front().time_pos;
}

f32 joj::BoneAnimation::get_end_time() const
{
    // keyframes are sorted by time, so last keyframe gives end time.
    return keyframes.back().time_pos;
}

void joj::BoneAnimation::interpolate(f32 t, JFloat4x4& M) const
{
	if (t <= keyframes.front().time_pos)
	{
		JXMVector4 S = XMLoadFloat3(&keyframes.front().scale);
		JXMVector4 P = XMLoadFloat3(&keyframes.front().translation);
		JXMVector4 Q = XMLoadFloat4(&keyframes.front().rotation_quat);

		JXMVector4 zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (t >= keyframes.back().time_pos)
	{
		JXMVector4 S = XMLoadFloat3(&keyframes.back().scale);
		JXMVector4 P = XMLoadFloat3(&keyframes.back().translation);
		JXMVector4 Q = XMLoadFloat4(&keyframes.back().rotation_quat);

		JXMVector4 zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (u32 i = 0; i < keyframes.size() - 1; ++i)
		{
			if (t >= keyframes[i].time_pos && t <= keyframes[i + 1].time_pos)
			{
				f32 lerpPercent = (t - keyframes[i].time_pos) / (keyframes[i + 1].time_pos - keyframes[i].time_pos);

				JXMVector4 s0 = DirectX::XMLoadFloat3(&keyframes[i].scale);
				JXMVector4 s1 = DirectX::XMLoadFloat3(&keyframes[i + 1].scale);

				JXMVector4 p0 = DirectX::XMLoadFloat3(&keyframes[i].translation);
				JXMVector4 p1 = DirectX::XMLoadFloat3(&keyframes[i + 1].translation);

				JXMVector4 q0 = DirectX::XMLoadFloat4(&keyframes[i].rotation_quat);
				JXMVector4 q1 = DirectX::XMLoadFloat4(&keyframes[i + 1].rotation_quat);

				JXMVector4 S = DirectX::XMVectorLerp(s0, s1, lerpPercent);
				JXMVector4 P = DirectX::XMVectorLerp(p0, p1, lerpPercent);
				JXMVector4 Q = DirectX::XMQuaternionSlerp(q0, q1, lerpPercent);

				JXMVector4 zero = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, DirectX::XMMatrixAffineTransformation(S, zero, Q, P));

				break;
			}
		}
	}
}