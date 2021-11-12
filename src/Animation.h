#pragma once

#include "Core.h"


enum class ANIM_STATE
{
  ACTIVE,
  INACTIVE
};

struct AnimInfo
{
  s32 anim_frame = 0;
  bool anim_flipped = false;
  TimerInfo anim_timer;
  ANIM_STATE anim_state = ANIM_STATE::INACTIVE;
};
