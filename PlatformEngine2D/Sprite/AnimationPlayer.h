#pragma once

#include <irrlicht.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

#include "ISpriteObj.h"
#include "BitMap.h"
#include "FModule.h"
#include "FMkRect.h"
#include "FMkPoint.h"
#include "Module.h"
#include "MkRect.h"
#include "MkPoint.h"
#include "Frame.h"
#include "Animation.h"
#include "Sprite.h"

class AnimationPlayer
{
private:

	Sprite* m_sprite;
	bool m_isReversed;

public:
	
	int CurrentAnimIdx;
	bool m_isFrameHasChanged;

	irr::u32			SpriteEffects;
	irr::video::SColor	TintColor;
	irr::f32			Speed;

    /// <summary>
    /// Indicates if animation frame was changed in last update
    /// </summary>
    bool IsFrameHasChanged() { return m_isFrameHasChanged; }

    AnimationPlayer(Sprite* sprite_) 
	: SpriteEffects(0)
	, TintColor( 255, 255, 255, 255)
	, Speed( 1 )
	, CurrentAnimIdx(-1)
	, m_isFrameHasChanged(false)
	, m_sprite( sprite_ )
	, m_isReversed( false )
    { }

    void Update(irr::f32 frameDT)
    {
        if (CurrentAnimIdx >= 0)
        {	
			//! NOTE> Animations run like speed is positive always. When drawing m_isPlayedBack value will decide what frame draw ( see Draw )
			irr::f32 finalFrameDt = frameDT * Speed;
			if( finalFrameDt < 0 ) finalFrameDt = -finalFrameDt;

            int iCurrentFrame = m_sprite->Animations[CurrentAnimIdx]->CurrentAFrameIdx();
            m_sprite->Animations[CurrentAnimIdx]->Update( finalFrameDt );
            m_isFrameHasChanged = iCurrentFrame != m_sprite->Animations[CurrentAnimIdx]->CurrentAFrameIdx();
        }
    }

    void Draw()
    {
		irr::core::vector2df vZero(0, 0);
        Draw(vZero);
    }

    void Draw( irr::core::vector2df position_ )
    {
        if
        (
            (CurrentAnimIdx >= 0) &&
			(m_sprite->Animations[CurrentAnimIdx]->AFrames.size() > 0)     
        )
        {
            Animation* objAnim = m_sprite->Animations[CurrentAnimIdx];

            int iFrameIdx = objAnim->CurrentAFrameIdx();

			if( m_isReversed )
			{
				std::size_t frameNb = objAnim->AFrames.size();
				iFrameIdx = (frameNb - iFrameIdx) % frameNb;
			}

            // Set Sprite Effect
            m_sprite->SpriteEffects = SpriteEffects;

			// Set tint color
			m_sprite->TintColor = TintColor;

            // Draw Sprite frame
            m_sprite->DrawAFrame(objAnim, iFrameIdx, position_);

        }
    }

	Sprite* GetSprite()	{ return m_sprite; }

	bool GetIsReversed()				{ return m_isReversed; }
	void SetIsReversed( bool value )	{ m_isReversed = value; }

    Animation* CurrentAnimation()
    {
		return ( CurrentAnimIdx >= 0 ) ? m_sprite->Animations[CurrentAnimIdx] : 0;
    }

    Frame* CurrentFrame()
    {
		return (CurrentAnimIdx >= 0) ? m_sprite->GetFrame(m_sprite->Animations[CurrentAnimIdx]->CurrentAFrame()->Id) : 0;
    }

	void SetSprite( Sprite* sprite )
	{
		m_sprite = sprite;

		ResetAnim();
	}

    void SetAnimationByTag( const std::string& sAnimTag_, int nLoops_)
    {

		int iAnimId = std::find( m_sprite->Animations.begin(), m_sprite->Animations.end(), m_sprite->GetAnimByTag(sAnimTag_)) - m_sprite->Animations.begin();

		ASSERT( iAnimId >= 0, "SetAnimation wrong animId for animation " << sAnimTag_ );

        SetAnimation( iAnimId , nLoops_);
    }

    void SetAnimation(int animId_, int nLoops_)
    {
		ASSERT( (animId_ >= 0) && (animId_ < m_sprite->Animations.size()), "SetAnimation wrong animId " << animId_ );

		// Avoid set twice
		if( animId_ != CurrentAnimIdx )
		{
			CurrentAnimIdx = animId_;

			if (CurrentAnimIdx >= 0)
			{
				m_sprite->Animations[CurrentAnimIdx]->StartAnimation(nLoops_);
				ResetAnim();
			}
		}
		else
		{
			// in case we set same animation just use loops argument
			m_sprite->Animations[CurrentAnimIdx]->m_nLoops = nLoops_;
		}
    }

    void ResetAnim()
    {
        if (CurrentAnimIdx >= 0)
        {
            m_sprite->Animations[CurrentAnimIdx]->ResetAnimation();
            m_isFrameHasChanged = true;
        }
    }

    bool IsAnimOver()
    {
        return ((CurrentAnimIdx >= 0) && (m_sprite->Animations[CurrentAnimIdx]->IsAnimOver()));
    }

    irr::core::vector2di GetFMkPoint(int iFMkPointIdx_)
    {
        return GetFMkPoint(CurrentAnimIdx, m_sprite->Animations[CurrentAnimIdx]->CurrentAFrameIdx(), iFMkPointIdx_);
    }

    irr::core::vector2di GetFMkPoint(int iAnimIdx_, int iAnimFrameIdx_, int iFMkPointIdx_ )
    {
        Animation* objAnim = m_sprite->Animations[iAnimIdx_];
        AFrame* objAFrame = objAnim->AFrames[iAnimFrameIdx_];
        std::string frameId = objAFrame->Id;
        Frame* objFrame = m_sprite->GetFrame(frameId);
        FMkPoint* objFMkPoint = objFrame->MkPoints[iFMkPointIdx_];

        irr::core::vector2di finalOffset = objAFrame->Offset + objFMkPoint->Offset;

        return m_sprite->GetMkPointData(objFMkPoint->Id, SpriteEffects, finalOffset);
    }

    irr::core::vector2di GetFMkPointSafe(int iFMkPointIdx_)
    {
        return GetFMkPointSafe(CurrentAnimIdx, m_sprite->Animations[CurrentAnimIdx]->CurrentAFrameIdx(), iFMkPointIdx_);
    }

    irr::core::vector2di GetFMkPointSafe(int iAnimIdx_, int iAnimFrameIdx_, int iFMkPointIdx_)
    {
        //if (m_sprite->Animations.size() <= iAnimIdx_) return null;
        //if (Sprite.Animations[iAnimIdx_].AFrames.Count <= iAnimFrameIdx_) return null;
        //if (Sprite.GetFrame(Sprite.Animations[iAnimIdx_].AFrames[iAnimFrameIdx_].Id).MkPoints.Count <= iFMkPointIdx_) return null;

        return GetFMkPoint(iAnimIdx_, iAnimFrameIdx_, iFMkPointIdx_  );
    }

	irr::core::recti GetFMkRect(int iFMkRectIdx_)
    {
        return GetFMkRect(CurrentAnimIdx, m_sprite->Animations[CurrentAnimIdx]->CurrentAFrameIdx(), iFMkRectIdx_);
    }

    irr::core::recti GetFMkRect(int iAnimIdx_, int iAnimFrameIdx_, int iFMkRectIdx_)
    {
        Animation* objAnim = m_sprite->Animations[iAnimIdx_];
        AFrame* objAFrame = objAnim->AFrames[iAnimFrameIdx_];
        std::string frameId = objAFrame->Id;
        Frame* objFrame = m_sprite->GetFrame(frameId);
        FMkRect* objFMkRect = objFrame->MkRects[iFMkRectIdx_];

        irr::core::vector2di finalOffset = objAFrame->Offset + objFMkRect->Offset;

        return m_sprite->GetMkRectData(objFMkRect->Id, SpriteEffects, finalOffset);
    }
};