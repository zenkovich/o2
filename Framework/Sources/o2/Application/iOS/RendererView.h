#import <MetalKit/MetalKit.h>

#ifdef PLATFORM_IOS

@interface RendererViewDelegate : NSObject <MTKViewDelegate>
@end

@interface RenderViewController : UIViewController
@end

@interface RenderView : MTKView
@end

#endif
