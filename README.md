# NTL_AgoraFPA

这个仓库是为了给AgoraFPA生成XCFramework使用

使用到了 [arm64-to-sim](https://github.com/bogo/arm64-to-sim)

同时修复了CFBundleShortVersionString缺失导致的Xcode13.3.1打包失败

由于没有i386的架构，最低版本支持为iOS 11

## 如何更新版本

1. 运行 `sh create_fpa_xcframework.sh`
2. 将当前目录的xcframework打包后发布到cdn
3. 更新podspec