//
//  WBUser.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/29.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import HandyJSON

struct WBUser: HandyJSON {
    var id: String?
    var screen_name: String?
    var verified_type: Int = -1
    var avatar_large: String?
    var verified_level: Int = 0
}
