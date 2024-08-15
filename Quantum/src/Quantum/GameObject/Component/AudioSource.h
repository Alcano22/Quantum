#pragma once

#include "Component.h"

namespace Quantum
{
	class SoundPlayer : public Component
	{
	public:
		void Init() override;
		void Update(float dt) override;
		void Render() override;
	};
}