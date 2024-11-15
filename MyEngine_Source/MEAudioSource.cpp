#include "MEAudioSource.h"
#include "METransform.h"
#include "MEGameObject.h"
#include "MEFmod.h"

namespace ME
{
	AudioSource::AudioSource()
		:Component(enums::eComponentType::AudioSource)
	{
	}
	AudioSource::~AudioSource()
	{
	}
	void AudioSource::Initialize()
	{
	}
	void AudioSource::Update()
	{
	}
	void AudioSource::LateUpdate()
	{
		
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector2 pos = tr->GetPosition();


		mAudioClip->Set3DAttributes(pos);
	}
	void AudioSource::Render(HDC hdc)
	{
	}
	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}
}