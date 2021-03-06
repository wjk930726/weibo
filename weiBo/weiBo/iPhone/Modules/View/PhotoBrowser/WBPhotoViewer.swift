//
//  WBPhotoViewer.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/3.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import Kingfisher
import UIKit

class WBPhotoViewer: UIViewController {
    let index: Int
    let url: String
    lazy var scrollView: UIScrollView = UIScrollView(frame: self.view.bounds)
    lazy var imageView: UIImageView = {
        let iv = UIImageView(imageName: "new_feature_4")
        iv.frame = self.view.bounds
        return iv
    }()

    init(index: Int, url: String) {
        self.index = index
        self.url = url
        super.init(nibName: nil, bundle: nil)
    }

    required init?(coder _: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    override func viewDidLoad() {
        super.viewDidLoad()

        view.addSubview(scrollView)
        scrollView.addSubview(imageView)

        downLoadImage()
    }

    func downLoadImage() {
        guard let url = URL(string: self.url) else {
            console.debug("download error")
            return
        }

        ImageDownloader.default.downloadImage(with: url) { downloadImage, err, _, _ in
            if err != nil {
                print(err!)
                return
            }
            let size = downloadImage!.size
            let height = size.height
            self.imageView.image = downloadImage!
            let newSize = CGSize(width: screenWidth, height: height * screenWidth / size.width)
            self.scrollView.contentSize = newSize
            self.imageView.frame = CGRect(origin: CGPoint.zero, size: newSize)
            if newSize.height < screenHeight {
                self.imageView.center = self.view.center
            }
        }
    }
}
