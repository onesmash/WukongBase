//
//  Packer.h
//  AppCore
//
//  Created by Xuhui on 15/5/31.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#ifndef __Net__Packer__
#define __Net__Packer__

#include <memory>
#include "net/Packet.h"

namespace WukongBase {
namespace Net {
    
class Packer {
public:
    virtual Packet pack() const = 0;
    virtual bool unpack(Packet& packet) = 0;
    
};
}
}

#endif /* defined(__Net__Packer__) */
