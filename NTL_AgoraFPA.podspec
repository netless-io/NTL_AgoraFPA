Pod::Spec.new do |s|
  s.name             = 'NTL_AgoraFPA'
  s.version          = '1.2.1'
  s.summary          = 'XCFramework for AgoraFPA.'

  s.description      = <<-DESC
  XCFramework for AgoraFPA.
                       DESC

  s.homepage         = 'https://github.com/netless-io/NTL_AgoraFPA'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'vince' => 'zjxuyunshi@gmail.com' }

  s.ios.deployment_target = '8.0'

  s.source = { :http => 'https://sdk.netless.link/fpa/1.0.0/AgoraFPA.zip' }
  s.vendored_frameworks = 'AgoraFpaProxyService.xcframework', 'AgoraFPA.xcframework'
  s.requires_arc          = true
end
