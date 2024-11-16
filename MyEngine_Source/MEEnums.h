#pragma once

namespace ME::enums
{
	enum class eLayerType
	{
		None,
		BackGround,
		Tile,
		Block,
		Floor,
		Items,
		Bullet,
		Monster,
		Obstacle,
		Aura,
		Player,
		Particle,
		Max = 20
	};

	enum class eResourceType
	{
		Texture,
		Animation,
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
		Camera,
		AudioListener,
		AudioSource,
		End,
	};
}