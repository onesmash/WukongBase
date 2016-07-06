//
//  Time.h
//  AppCore
//
//  Created by Xuhui on 15/5/23.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Base__Time__
#define __Base__Time__

#include <chrono>

namespace WukongBase {

namespace Base {
    
extern const int kMicroSecondsPerSecond;
    
typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
typedef std::chrono::microseconds TimeDelta;
typedef std::chrono::system_clock Time;
    
}
    
}

#define timeDeltaFromSeconds(seconds) (WukongBase::Base::TimeDelta((long long)(seconds * WukongBase::Base::kMicroSecondsPerSecond)))

#define timeDeltaToMilliseconds(timeDelta) (std::chrono::duration_cast<std::chrono::milliseconds>(timeDelta).count())

#endif /* defined(__Base__Time__) */
