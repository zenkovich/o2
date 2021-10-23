#import "RendererView.h"

#import "ShaderTypes.h"
#include "o2/Render/Render.h"
#include "o2/Render/Mac/MetalWrappers.h"
#include "o2/Application/Application.h"
#include "o2/Application/Input.h"
#include "o2/Application/Mac/ApplicationPlatformWrapper.h"
#include "o2/Utils/Debug/Debug.h"

@implementation RendererView

-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    self = [super init];
    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
	o2Application.Update();
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
	o2::ApplicationPlatformWrapper::OnWindowRsized(o2::Vec2I(size.width, size.height));
}

@end

@implementation ViewController

- (void)updateTrackingAreas {
	[self initTrackingArea];
}

-(void)initTrackingArea {
	NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
									 NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
	
	NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:[self bounds]
														options:options
														  owner:self
													   userInfo:nil];
	
	[self addTrackingArea:area];
}

- (void)keyDown:(NSEvent *)event
{
	o2Input.OnKeyPressed([event keyCode]);
}

- (void)keyUp:(NSEvent *)event
{
	o2Input.OnKeyReleased([event keyCode]);
}

- (o2::Vec2F)getMousePos:(NSEvent *)event
{
	NSPoint pt = [self convertPoint:[event locationInWindow] fromView:nil];
	NSRect viewRectPoints = [self bounds];
	NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
	
	float scale = o2Application.GetGraphicsScale();
	auto screenPoint = o2::Vec2F(o2::Math::Floor(pt.x*scale - viewRectPixels.size.width/2),
								 o2::Math::Floor(pt.y*scale - viewRectPixels.size.height/2));
	
	return screenPoint;
}

- (void)mouseDown:(NSEvent *)event
{
	o2Input.OnCursorPressed([self getMousePos:event]);
}

- (void)mouseDragged:(NSEvent *)event
{
	o2Input.OnCursorMoved([self getMousePos:event]);
}

- (void)mouseMoved:(NSEvent *)event
{
	o2Input.OnCursorMoved([self getMousePos:event], 0);
}

- (void)mouseUp:(NSEvent *)event
{
	o2Input.OnCursorReleased();
}

- (void)rightMouseDown:(NSEvent *)event
{
	o2Input.OnAltCursorPressed([self getMousePos:event]);
}

- (void)rightMouseUp:(NSEvent *)event
{
	o2Input.OnAltCursorReleased();
}

- (void)scrollWheel:(NSEvent *)event
{
	o2Input.OnMouseWheel((float)event.scrollingDeltaY);
}

@end
