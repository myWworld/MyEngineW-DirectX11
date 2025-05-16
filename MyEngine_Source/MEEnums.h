#pragma once

namespace ME::enums
{
	enum class eLayerType
	{
		None,
		BackGround,
		Block,
		Floor,
		Items,
		Player,
		Particle,
		Max = 20
	};

	enum class eModelType
	{
		Static,
		StaticBone,
		SkinnedMesh,
		End,
	};

	enum class eResourceType
	{
		Texture,
		Mesh,
		Material,
		Animation,
		Animation3D,
		Skeleton,
		AudioClip,
		Shader,
		Prefab,
		End
	};

	enum class eColliderType
	{
		Rect2D,
		Circle2D,
	};

	enum class eUIType
	{
		HpBar,
		Score,
		Button,
		End,
	};

	enum class eComponentType
	{
		Trasnform,
		Collider,
		Rigidbody,
		Script,
		SpriteRenderer,
		Animator,
		ModelRenderer,
		Animator3D,
		Camera,
		AudioListener,
		AudioSource,
		End,
	};
}