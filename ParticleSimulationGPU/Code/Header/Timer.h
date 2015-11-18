#ifndef _TIMER_H_
#define _TIMER_H_

#include "../Header/3DLibs.h"

class Timer
{
	private:
		__int64		mCntsPerSec;
		double		mSecsPerCnt;
		__int64		mPrevTimeStamp;
		float		mFps;

	public:
		float		GetDeltaTime();
		float		GetFPS();

		HRESULT		Initialize();
		void		Release();
					Timer();
		virtual		~Timer();
};
#endif