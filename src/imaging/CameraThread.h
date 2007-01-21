//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2006 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#ifndef _CameraThread_H_
#define _CameraThread_H_

#include "../avgconfig.h"
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include "Camera.h"

#include "../graphics/Rect.h"
#include "../graphics/Bitmap.h"
#include "../graphics/Pixel24.h"

#include "../base/Queue.h"
#include "../base/WorkerThread.h"

#ifdef AVG_ENABLE_1394
#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>
#endif
#ifdef AVG_ENABLE_1394_2
#include <dc1394/control.h>
#endif
//#ifndef AVG_ENABLE_1394_2
//typedef unsigned int dc1394feature_t;
//#endif

#include <string>

namespace avg {

typedef Queue<BitmapPtr> BitmapQueue;

class CameraThread: public WorkerThread<CameraThread> {
public:
    CameraThread(BitmapQueue& BitmapQ, CmdQueue& CmdQ, 
            std::string sDevice, double FrameRate, std::string sMode, bool bColor);

    bool init();
    bool work();
    void deinit();

    void setFeature(dc1394feature_t Feature, int Value);

private:
    void captureImage();

    Camera m_Camera;
    BitmapQueue& m_BitmapQ;
};

}

#endif
