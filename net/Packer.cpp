//
//  Packer.cpp
//  AppCore
//
//  Created by Xuhui on 15/5/31.
//  Copyright (c) 2015年 Xuhui. All rights reserved.
//

#include "net/Packer.h"

namespace WukongBase {
namespace Net {

Packet Packer::pack() const
{
    return Packet();
}

bool Packer::unpack(Packet& packet)
{
    return true;
}

}
}