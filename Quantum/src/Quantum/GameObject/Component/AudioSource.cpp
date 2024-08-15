#include "qtmpch.h"
#include "SoundPlayer.h"

#include "Quantum/Input/Input.h"
#include "Quantum/Audio/AudioEngine.h"

#include <GLFW/glfw3.h>

namespace Quantum
{
	void SoundPlayer::Init()
	{
		AudioEngine& audioEngine = AudioEngine::GetInstance();
		audioEngine.LoadSound("explosion", "assets/sounds/explosion.wav");
		audioEngine.SetVolume("explosion", 0.5f);
	}

	void SoundPlayer::Update(float dt)
	{
		if (Input::GetKeyDown(GLFW_KEY_SPACE))
			AudioEngine::GetInstance().PlaySound("explosion");
	}

	void SoundPlayer::Render()
	{
		
	}
}
