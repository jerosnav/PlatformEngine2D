#pragma once

#include <irrlicht.h>
#include <vector>
#include <string>

#include "ISpriteObj.h"
#include "AFrame.h"

class Animation : public ISpriteObj
{
	friend class AnimationPlayer;

private:
	
	int m_nCurrentAnimFrame;
	irr::f32 m_fFrameTime; // count current frame time
    int m_nLoops; //-1 for infinite

public:

	//+++ DATA MEMBERS +++
	std::vector<AFrame*> AFrames;
	//---

public:

	 Animation( const std::string& Tag, const std::string& Id ) : ISpriteObj(Id, Tag),
		m_nCurrentAnimFrame(0),
		m_fFrameTime(0),
		m_nLoops(-1),
		AFrames()
    {   }

	~Animation()
	{
		std::vector<AFrame*>::iterator iter_end( AFrames.end() );
		for( std::vector<AFrame*>::iterator iter( AFrames.begin() ); iter != iter_end; ++iter ) delete (*iter);
	}

	virtual eSpriteObjType GetType() { return eSpriteObjType::SPR_TYPE_ANIMATION; } 

    int CurrentAFrameIdx() { return m_nCurrentAnimFrame; }
    
	std::string CurrentSpriteFrameKey()
    {
		if (AFrames.size() <= 0) return "error!";
		return AFrames[m_nCurrentAnimFrame % AFrames.size()]->Id; 
    }

    AFrame* CurrentAFrame() { return (AFrames.size() > 0) ? AFrames[m_nCurrentAnimFrame] : 0; }
	
	/// <summary>
    /// Setup animation for a specific number of loops or infinite
    /// </summary>
    /// <param name="nLoops_">Number of animation loop or infinite if equal to -1</param>
    void StartAnimation( int nLoops_ )
    {
        m_nLoops = nLoops_;
        m_fFrameTime = 0;
    }

    void ResetAnimation()
    {
        m_nCurrentAnimFrame = 0;
        m_fFrameTime = 0;
    }

    bool IsAnimOver()
    {
        return (m_nLoops == 0);
    }

	void Update(irr::f32 frameDT)
    {
        if 
        (
			(AFrames.size() > 0) &&
            (false == IsAnimOver())
        )
        {
            m_fFrameTime += frameDT;

            //fix: skip frames with invalid times
            if (AFrames[m_nCurrentAnimFrame]->TimeMs <= 0)
            {
//return>
                return;
            }
            //---

            while (m_fFrameTime >= AFrames[m_nCurrentAnimFrame]->TimeMs)
            {
                // Update time counter and set next frame
                m_fFrameTime -= AFrames[m_nCurrentAnimFrame]->TimeMs;

                // Check if last frame
                if (m_nCurrentAnimFrame == (AFrames.size() - 1))
                {
                    if (m_nLoops >= 1)
                    {
                        m_nLoops--;

                        // Go back to first frame only if loops remains, otherwise keep in last frame
                        if (m_nLoops > 0)
                        {
                            m_nCurrentAnimFrame = 0;
                            //m_fFrameTime = 0; //!NOTE> not tested, but this should synchonize properly when looping
                        }
                    }
                    else if (m_nLoops < 0) // loop infinite
                    {
                        m_nCurrentAnimFrame = 0;
                    }
                }
                else
                {
                    m_nCurrentAnimFrame++; m_nCurrentAnimFrame %= AFrames.size();
                }
            }
        }
    }

    /// <summary>
    /// Set a frame for specify animation frame id ( position innert animation frame array )
    /// </summary>
    /// <param name="animFrameId_"> Position inner animation frame array </param>
    /// <param name="frameId_">Frame id</param>
    /// <param name="timeMS_">Time in millisecs for this frame</param>
	void AddAFrame( const std::string& frameKey_, irr::core::vector2di offset_, int timeMS_ )
    {
		AFrame *aframe = new AFrame(frameKey_, offset_, timeMS_);
		AFrames.push_back(aframe);
    }
};
